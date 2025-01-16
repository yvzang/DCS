#ifndef __ICMP_H__
#define __ICMP_H__

extern "C"{
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/timerfd.h>
#include <sys/time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <errno.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
}
#include <string>
#include <functional>
#include <vector>

struct _ICMP_Callback_t{
	std::function<void(void* args)> icmp_succ_callback;
	std::function<void(void* args)> icmp_fail_callback;
	void* data;
	void success(){if(icmp_succ_callback)icmp_succ_callback(data);}
	void fail(){if(icmp_fail_callback)icmp_fail_callback(data);}
};

enum _ICMP_PING_RESULT{
	ICMP_PING_SUCC,
	ICMP_PING_FAIL
};

using ICMP_Callback_t = _ICMP_Callback_t;
using ICMP_PING_RESULT = _ICMP_PING_RESULT;

class ICMP{
private:
	int _sock_fd;
	struct sockaddr_in _dst_addr, _from_addr;
    int _ntx, _nrx;   //number of send pkg and recv pkg
    int _tx, _rx;
    int _rslt; //__ICMP_PING_XXXX
	ICMP_Callback_t _callback;
private:
	int icmp_socket();
	unsigned short icmp_gen_chksum(unsigned short* data, int len);
	int icmp_dst_addr(const std::string & addrHost, struct sockaddr_in * dst_addr);
	int icmp_pkg_pack(std::vector<char> & buffer, int pack_no, const std::string & data, int data_size);
	int icmp_send_pkg(const std::string & data, int size);
	int icmp_recv_pkg(void *recvbuf, int size);
	int icmp_pkg_unpack(char * buf, int len);
public:
    int timeout_s;  //time out second
	int ntry;
	ICMP() = delete;
	ICMP(const std::string & ipv4, ICMP_Callback_t cb);
	~ICMP();
	int icmp_ping(unsigned int timeout, int ntry);
	int result(){return _rslt;}

};

#endif
