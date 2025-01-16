#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "serial.h"
#include "limlog.h"

Serial::Serial(const std::string & devname, uint32_t speed){
    this->speed = _getSpeed(speed);
    F_ID = openPort(devname);
}

Serial::Serial(const Serial & src){
    this->speed = src.speed;
    F_ID = src.F_ID;
}

Serial::~Serial(){
    close(F_ID);
}

int Serial::openPort(const std::string & COM_name)
{        
        int fd = open(COM_name.c_str(), O_RDWR | O_NOCTTY );
                
        if (fd == -1)
        {
            //char *errmsg = strerror(errno);
            //printf("!!! Error UART open port: %s\n", errmsg);                
            LOG_ERROR << "Open serial port ERROR: " << strerror(errno);
            return -1;
        }
        else
        { 
            LOG_DEBUG << "Open serial fd: " << fd;
        }
        struct termios options; /*структура для установки порта*/
        
        tcgetattr(fd, &options); /*читает пораметры порта*/

        //options.c_cflag &= ~CBAUD;    //Remove current BAUD rate
        //options.c_cflag |= BOTHER;
        
        //ioctl( fd, TIOCGSERIAL, &options );
  	//options.flags |= ASYNC_LOW_LATENCY;
  	//ioctl( fd, TIOCSSERIAL, &options );
        
        cfsetispeed(&options, this->speed); /*установка read скорости порта*/
        cfsetospeed(&options, this->speed); /*установка write скорости порта*/

        
        
        options.c_cc[VTIME] = 5; /*Время ожидания байта 500 мс. Время задержки между пакетами. Silent interval. */
        options.c_cc[VMIN] = 0; /*минимальное число байт для чтения*/


        options.c_cflag &= ~PARENB; /*бит четности не используется*/
        options.c_cflag &= ~CSTOPB; /*1 стоп бит */
        options.c_cflag &= ~CSIZE;  /*Размер байта*/
        options.c_cflag |= CS8;  /*8 бит*/

        options.c_lflag = 0;
        options.c_oflag &= ~OPOST;  /*Обязательно отключить постобработку*/



         //
         // Input flags - Turn off input processing
         //
         // convert break to null byte, no CR to NL translation,
         // no NL to CR translation, don't mark parity errors or breaks
         // no input parity check, don't strip high bit off,
         // no XON/XOFF software flow control
         //
         options.c_iflag &= ~(IGNBRK | BRKINT | ICRNL |
                             INLCR | PARMRK | INPCK | ISTRIP | IXON);
         
         //
         // Output flags - Turn off output processing
         //
         // no CR to NL translation, no NL to CR-NL translation,
         // no NL to CR translation, no column 0 CR suppression,
         // no Ctrl-D suppression, no fill characters, no case mapping,
         // no local output processing
         //
         // config.c_oflag &= ~(OCRNL | ONLCR | ONLRET |
         //                     ONOCR | ONOEOT| OFILL | OLCUC | OPOST);
         options.c_oflag = 0;        
         
         //
         // No line processing
         //
         // echo off, echo newline off, canonical mode off, 
         // extended input processing off, signal chars off
         //
         options.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);         
        
        
        
        tcsetattr(fd, TCSANOW, &options); /*сохранения параметров порта*/
        return fd;
}

int Serial::readByte(char & ch){
    int ret = read(F_ID, &ch, 1);
    return ret;
}

int Serial::readData(std::string & buff, int timeout_value)
{
        fd_set set;
        struct timeval timeout;    
        int n = 0;
        int total = 0;
        char readbuf[1024]{0};
        
        FD_ZERO(&set); /* clear the set */
        FD_SET(F_ID, &set); /* add our file descriptor to the set */
        
        timeout.tv_sec = 0;
        timeout.tv_usec = timeout_value;
        
        do
        {   
            n = select(F_ID + 1, &set, NULL, NULL, &timeout);                                

            if (n == -1)
            {
                //char *errmsg = strerror(errno);
                //printf("!!! read uart error: %s \r\n", errmsg);  
                LOG_ERROR << "Read uart ERROR: " << strerror(errno);
                break;
                
            }
            else if(n == 0)
            {         

                break;
            }
            else
            {                
                n = read(F_ID, readbuf + total, 1); /* there was data to read */                
                total += n;

                if(n == -1 || n == 0) { break; }                                                  
            }

            FD_ZERO(&set);
            FD_SET(F_ID, &set);

        }while(FD_ISSET(F_ID, &set));
        buff.assign(readbuf, total);
        return total;
        //return (n==-1 ? -1 : total);
}

int Serial::sendData(char* buff, size_t size)
{    
        int n = write(F_ID, buff, size);
         
        if (n < 0)
        {
                //char *errmsg = strerror(errno);
                //printf("uart send error: %s \r\n", errmsg);
                LOG_ERROR << "Send uart ERROR: " << strerror(errno);
        }        
                
        return n;
}

int Serial::sendData(const std::string & buff)
{    
        int n = write(F_ID, buff.c_str(), buff.length());
         
        if (n < 0)
        {
                //char *errmsg = strerror(errno);
                //printf("uart send error: %s \r\n", errmsg);
                LOG_ERROR << "Send uart ERROR: " << strerror(errno);
        }        
                
        return n;
}

Serial Serial::operator=(const Serial & src){
    speed = src.speed;
    F_ID = src.F_ID;
    return *this;
}

speed_t Serial::_getSpeed(uint32_t sp){
    speed_t res;
    switch (sp)
    {
    case 2400:
        res = B2400;
        break;
    case 4800:
        res = B4800;
        break;
    case 9600:
        res = B9600;
        break;
    case 19200:
        res = B19200;
        break;
    case 38400:
        res = B38400;
        break;
    case 57600:
        res = B57600;
        break;
    case 115200:
        res = B115200;
        break;
    case 230400:
        res = B230400;
        break;
    case 460800:
        res = B460800;
        break;
    
    default:
        res = 9600;
        break;
    }
    return res;
}