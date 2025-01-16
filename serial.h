#ifndef __SERIAL_H__
#define __SERIAL_H__
#include <termios.h>
#include <string>

class Serial{
private:
	speed_t speed;
	int F_ID;

public:
	Serial() = delete;
	Serial(const std::string & devname, uint32_t speed);
	Serial(const Serial &);
	int openPort(const std::string & COM_name);
	int readData(std::string & buff, int timeout_value);
	int readByte(char & ch);
	int sendData(const std::string & buff);
	int sendData(char* buff, size_t size);

	int getFD(){return F_ID;};

	speed_t _getSpeed(uint32_t speed);

	Serial operator=(const Serial &);
	virtual ~Serial();
};


#endif