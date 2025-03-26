#ifndef __UTILS_H__
#define __UTILS_H__
#include <string>
#include <sstream>

#define CONFIG_PLC_DEVICE_ID				"mc_plc"

template<class T>
std::string toString(const T & any){
	std::stringstream ss;
	ss << any;
	return ss.str();
}

uint32_t str2uint32(const std::string strValue);
float str2float(const std::string & strValue);

bool file_read(const std::string & path, std::string & str);
bool file_write(const std::string & path, const std::string & str);

#endif