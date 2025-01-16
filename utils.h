#ifndef __UTILS_H__
#define __UTILS_H__
#include <string>
#include <sstream>

template<class T>
std::string toString(const T & any){
	std::stringstream ss;
	ss << any;
	return ss.str();
}

bool file_read(const std::string & path, std::string & str);
bool file_write(const std::string & path, const std::string & str);

#endif