#include <utils.h>
#include <unistd.h>

bool file_read(const std::string & path, std::string & str){
	if(access(path.c_str(), F_OK) != 0){
		return false;
	}
	char readbuf[1024]{0};
	int ret;
	FILE* fp = fopen(path.c_str(), "r");
	if(fp == NULL){
		return false;
	}
	else{
		ret = fread(readbuf, 1, sizeof(readbuf), fp);
		if(ret <= 0){
			fclose(fp);
			return false;
		}
	}
	str.assign(readbuf);
	fclose(fp);
	return true;
}

bool file_write(const std::string & path, const std::string & str){
	FILE* fp = fopen(path.c_str(), "w");
	int ret;
	if(fp == NULL){
		fclose(fp);
		return false;
	}
	else{
		ret = fwrite(str.c_str(), 1, str.length(), fp);
		if(ret <= 0){
			fclose(fp);
			return false;
		}
	}
	fclose(fp);
	return true;
}