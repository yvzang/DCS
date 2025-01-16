#include <vector>
#include "ICMP.h"
#include "limlog.h"
#ifndef offsetof
#define offsetof(type, member)    ( (int) & ((type*)0) -> member )
#endif

#define ICMP_BUF_SIZE           256
#define ICMP_RECVBUF_SIZE       (50 * 1024)
#define ICMP_MAGIC_ID           0xf1f2
#define ICMP_PROTO_NAME         "icmp"
#define ICMP_DATA               "Rong Tao's ICMP Ping"


static void __icmp_timeval_sub(struct timeval * out, struct timeval * in)
{
    if ((out->tv_usec -= in->tv_usec) < 0)
    {
        --out->tv_sec;
        out->tv_usec += 1000000;
    }
    out->tv_sec -= in->tv_sec;
}

/* create a socke to icmp */
int ICMP::icmp_socket()
{
    int sockfd;
    int size = ICMP_RECVBUF_SIZE;
    
    struct protoent * protocol  = NULL;
    
    if ((protocol = getprotobyname(ICMP_PROTO_NAME)) == NULL)
    {
        LOG_ERROR << "getprotobyname error.";
        return -1;
    }
    if ((sockfd = socket(AF_INET, SOCK_RAW, protocol->p_proto)) < 0)
    {
        LOG_ERROR << "socket error. " << strerror(errno);
        return -1;
    }
    if(setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size))!=0)
    {
        LOG_ERROR << "setsockopt SO_RCVBUF error.";
        close(sockfd);
        return -1;
    }
#if 1    
    struct timeval timeout = {1,0}; //sec
    if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval))!=0)
    {
        LOG_ERROR << "setsockopt SO_RCVTIMEO error.";
        close(sockfd);
        return -1;
    }
#endif    
    return sockfd;
}

/* get dst address */
int ICMP::icmp_dst_addr(const std::string & addrHost, struct sockaddr_in * dst_addr)
{
    struct hostent  * host      = NULL;
    unsigned long   inaddr      = 0;
 
    bzero(dst_addr, sizeof(struct sockaddr_in));
    dst_addr->sin_family = AF_INET;
 
    if ((inaddr = inet_addr(addrHost.c_str())) == INADDR_NONE)
    {
        if ((host = gethostbyname(addrHost.c_str())) == NULL)
        {
            LOG_ERROR << "gethostbyname error.";
            return (-1);
        }
        memcpy((char *) &dst_addr->sin_addr, host->h_addr, host->h_length);
    }
    else 
    {
        memcpy((char *) &dst_addr->sin_addr, (char *) &inaddr, sizeof(dst_addr->sin_addr));
    }
    
    return 0;
}

/* set icmp hdr */
int ICMP::icmp_pkg_pack(std::vector<char> & buffer, int pack_no, const std::string & data, int data_size)
{
    struct icmp * icmp  = (struct icmp*)malloc(sizeof(struct icmp));
    icmp->icmp_type     = ICMP_ECHO;
    icmp->icmp_code     = 0;
    icmp->icmp_cksum    = 0;
    icmp->icmp_seq      = htons(pack_no);
    icmp->icmp_id       = htons(ICMP_MAGIC_ID);
 
    gettimeofday((struct timeval *) &icmp->icmp_data, NULL);
 
	char* buf_ptr = reinterpret_cast<char*>(&icmp);
	std::copy(buf_ptr, buf_ptr + sizeof(struct icmp) - 1, 
			std::back_inserter(buffer));
 
    if(data.length() && data_size)
    {
        std::copy(data.begin(), data.begin()+data_size-1,
				std::back_inserter(buffer));
    }
 
    return buffer.size();
}

/* alogrithm of checksum */
unsigned short ICMP::icmp_gen_chksum(unsigned short* data, int len)
{
    int             nleft   = len;
    int             sum     = 0;
    unsigned short  *w      = (unsigned short*)data;
    unsigned short  answer  = 0;
 
    while (nleft > 1)
    {
        sum += *w++;
        nleft -= 2;
    }
 
    if (nleft == 1)
    {
        * (unsigned char *) (&answer) = *(unsigned char *)w;
        sum += answer;
    }
 
    sum     = (sum >> 16) + (sum & 0xffff);
    sum     += (sum >> 16);
    answer  = ~sum;
    
    return answer;
}

/* send icmp package */
int ICMP::icmp_send_pkg(const std::string & data, int size)
{
    int             packetsize;
    unsigned short  checksum = 0;
    int             n = 0;
    
    std::vector<char> pkg_buffer;
    
    packetsize  = icmp_pkg_pack(pkg_buffer, _ntx, data, size);
    checksum    = icmp_gen_chksum((unsigned short *)pkg_buffer.data(), packetsize);
    
#define ICMP_PKG_CHKSUM_OFFSET  offsetof(struct icmp, icmp_cksum)
#define ICMP_PKG_CHKSUM_SIZE    2
 
	unsigned char* checksum_ptr = (unsigned char*)&checksum;
	std::copy(checksum_ptr, checksum_ptr + 2, 
			pkg_buffer.begin() + ICMP_PKG_CHKSUM_OFFSET);
 
    if ((n = sendto(_sock_fd, pkg_buffer.data(), pkg_buffer.size(), 0, 
                     (struct sockaddr *) &_dst_addr, sizeof(struct sockaddr_in)))< 0)
    {
        LOG_ERROR << "sendto error. n = " << n;
        return 0;
    }
    _ntx++;
                     
    return n;
}

/* send icmp package */
int ICMP::icmp_recv_pkg(void *recvbuf, int size)
{
    int n, fromlen;
    
    fromlen = sizeof(struct sockaddr_in);
 
    if ((n = recvfrom(_sock_fd, recvbuf, size, 0, 
                      (struct sockaddr *) &_from_addr, (socklen_t*)&fromlen)) < 0)
    {
        LOG_ERROR << "recvfrom error.n = " << n;
        return 0;
    }
	printf("receive data: size: %d, [%s]\n", n, recvbuf);
    _nrx++;
    
    return n;
}

/* unpack icmp pkg */
int ICMP::icmp_pkg_unpack(char * buf, int len)
{
    int     i, iphdrlen;
 
    struct  ip * ip = NULL;
    struct  icmp * icmp = NULL;
    struct  timeval * tvsend = NULL;
    double  rtt;
 
    ip          = (struct ip *)buf;
    iphdrlen    = ip->ip_hl << 2;
    icmp        = (struct icmp *) (buf + iphdrlen);
    len        -= iphdrlen;
 
    if (len < 8)
    {
        LOG_ERROR << "ICMP packet\'s length is less than 8";
        return - 1;
    }
 
    /* ensure icmp reply is mine response msg */
    if ((icmp->icmp_type == ICMP_ECHOREPLY) && (icmp->icmp_id = ntohs(ICMP_MAGIC_ID)))
    {
        tvsend  = (struct timeval *)icmp->icmp_data;
 
        struct timeval tvrecv;
        gettimeofday(&tvrecv, NULL);
        __icmp_timeval_sub(&tvrecv, tvsend);
        rtt = tvrecv.tv_sec * 1000 + tvrecv.tv_usec / 1000;
		printf("%d byte from %s: icmp_req=%u ttl=%d rtt=%.3f ms\n\r", 
                        len, inet_ntoa(_from_addr.sin_addr), ntohs(icmp->icmp_seq), ip->ip_ttl, rtt);
    }
    else 
    {
        return - 1;
    }
    return 0;
}


ICMP::ICMP(const std::string & ipv4, ICMP_Callback_t cb)
:_callback(cb), _ntx(0), _tx(0){
	_sock_fd = icmp_socket();
}

ICMP::~ICMP(){
	close(_sock_fd);
}

/* PING */
int ICMP::icmp_ping(unsigned int timeout, int ntry)
{
    this->timeout_s = timeout;
    this->ntry      = ntry;
 
    char pkg_buffer[ICMP_BUF_SIZE];
 
    _rslt =	ICMP_PING_FAIL;
 
    while(_ntx < ntry)
    {
        _tx = icmp_send_pkg(ICMP_DATA, sizeof(ICMP_DATA));
        if(_tx < 8) 
        {
            _rslt = ICMP_PING_FAIL;
			LOG_ERROR << "send error";
            break;
        }
        
        _rx = icmp_recv_pkg(pkg_buffer, _tx);
        if(_rx<8) 
        {
            _rslt = ICMP_PING_FAIL;
			LOG_ERROR << "recv error";
            break;
        }
        if (icmp_pkg_unpack(pkg_buffer, _rx) == -1)
            continue;
        sleep(1);
    }

    if(_ntx == ntry)
        _rslt = ICMP_PING_SUCC;

    if(_rslt == ICMP_PING_FAIL)
		_callback.fail();
	else
		_callback.success();
    return _rslt;
}