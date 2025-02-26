#include <string.h>
#include <errno.h>
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <memory>
#include <new>
#include <cmath>
#include "limlog.h"
#include "plc_protocol.h"

PLCProtocol::PLCProtocol(const std::string & ipaddr, int port)
:ipaddr(ipaddr), port(port)
{
	pthread_mutex_init(&rw_lock, NULL);
}

bool PLCProtocol::reconnect(){
	if(is_connected()){
		disconnect();
	}
	return connect();
}

bool PLCProtocol::reconnect(const std::string & ipaddr, int port){
	this->ipaddr = ipaddr;
	this->port = port;
	return this->reconnect();
}

/*==================MCProtocol=====================*/

MCProtocol::MCProtocol(const std::string & ipaddr, int port = 554)
:PLCProtocol(ipaddr, port)
{
	//connect(ipaddr, port);
}

MCProtocol::~MCProtocol(){
	disconnect();
}

bool MCProtocol::connect(){
	return this->connect(this->ipaddr, this->port);
}

bool MCProtocol::connect(const std::string & ipaddr, int port){
	_sock_fd = mc_connect((char*)ipaddr.c_str(), port, 0, 0);
	if(_sock_fd < 0){
		printf("mc protocol connection error.\n%s\n", strerror(errno));
		return false;
	}
	_is_connected = true;
	return true;
}

bool MCProtocol::disconnect(){
	_is_connected = false;
	return mc_disconnect(_sock_fd);
}

err_code_t MCProtocol::read_bool(const reg_address_t address, bool* val) const{
	return mc_read_bool(_sock_fd, address.c_str(), val);
}

err_code_t MCProtocol::read_byte(const reg_address_t address, byte* val) const{
	return -1;
}

err_code_t MCProtocol::read_short(const reg_address_t address, short* val) const{
	return mc_read_short(_sock_fd, address.c_str(), val);
}

err_code_t MCProtocol::read_ushort(const reg_address_t address, ushort* val) const{
	return mc_read_ushort(_sock_fd, address.c_str(), val);
}

err_code_t MCProtocol::read_int32(const reg_address_t address, int32* val) const{
	return mc_read_int32(_sock_fd, address.c_str(), val);
}

err_code_t MCProtocol::read_uint32(const reg_address_t address, uint32* val) const{
	return mc_read_uint32(_sock_fd, address.c_str(), val);
}

err_code_t MCProtocol::read_int64(const reg_address_t address, int64* val) const{
	return mc_read_int64(_sock_fd, address.c_str(), val);
}

err_code_t MCProtocol::read_uint64(const reg_address_t address, uint64* val) const{
	return mc_read_uint64(_sock_fd, address.c_str(), val);
}

err_code_t MCProtocol::read_float(const reg_address_t address, float* val) const{
	return mc_read_float(_sock_fd, address.c_str(), val);
}

err_code_t MCProtocol::read_double(const reg_address_t address, double* val) const{
	return mc_read_double(_sock_fd, address.c_str(), val);
}

err_code_t MCProtocol::read_string(const reg_address_t address, int length, std::string & val) const{
	char *readbuf;
	int ret = mc_read_string(_sock_fd, address.c_str(), length, &readbuf);
	if(ret == MC_ERROR_CODE_SUCCESS){
		val.assign(readbuf, length);
		free(readbuf);
	}
	return ret;
}

err_code_t MCProtocol::batch_read_bool(const reg_address_t address, std::vector<bool> & val, size_t size) const{
	if(size == 0) return MC_ERROR_CODE_SUCCESS;
	bool readbuf[PLC_READ_MAXLEN] = {0};
	int ret = mc_batch_read_bool(_sock_fd, address.c_str(), readbuf, size);
	if(ret == MC_ERROR_CODE_SUCCESS){
		val.assign(readbuf, readbuf+size);
	}
	return ret;
}

err_code_t MCProtocol::batch_read_byte(const reg_address_t address, std::vector<byte> & val, size_t size) const{
	return -1;
}

err_code_t MCProtocol::batch_read_short(const reg_address_t address, std::vector<short> & val, size_t size) const{
	if(size == 0) return MC_ERROR_CODE_SUCCESS;
	short readbuf[PLC_READ_MAXLEN] = {0};
	int ret = mc_batch_read_short(_sock_fd, address.c_str(), readbuf, size);
	if(ret == MC_ERROR_CODE_SUCCESS){
		val.assign(readbuf, readbuf+size);
	}
	return ret;
}

err_code_t MCProtocol::batch_read_ushort(const reg_address_t address, std::vector<ushort> & val, size_t size) const{
	if(size == 0) return MC_ERROR_CODE_SUCCESS;
	ushort readbuf[PLC_READ_MAXLEN] = {0};
	int ret = mc_batch_read_ushort(_sock_fd, address.c_str(), readbuf, size);
	if(ret == MC_ERROR_CODE_SUCCESS){
		val.assign(readbuf, readbuf+size);
	}
	return ret;
}

err_code_t MCProtocol::batch_read_int32(const reg_address_t address, std::vector<int32> & val, size_t size) const{
	if(size == 0) return MC_ERROR_CODE_SUCCESS;
	int32 readbuf[PLC_READ_MAXLEN] = {0};
	int ret = mc_batch_read_int32(_sock_fd, address.c_str(), readbuf, size);
	if(ret == MC_ERROR_CODE_SUCCESS){
		val.assign(readbuf, readbuf+size);
	}
	return ret;
}

err_code_t MCProtocol::batch_read_uint32(const reg_address_t address, std::vector<uint32> & val, size_t size) const{
	if(size == 0) return MC_ERROR_CODE_SUCCESS;
	uint32 readbuf[PLC_READ_MAXLEN] = {0};
	int ret = mc_batch_read_uint32(_sock_fd, address.c_str(), readbuf, size);
	if(ret == MC_ERROR_CODE_SUCCESS){
		val.assign(readbuf, readbuf+size);
	}
	return ret;
}

err_code_t MCProtocol::batch_read_int64(const reg_address_t address, std::vector<int64> & val, size_t size) const{
	if(size == 0) return MC_ERROR_CODE_SUCCESS;
	int64 readbuf[PLC_READ_MAXLEN] = {0};
	int ret = mc_batch_read_int64(_sock_fd, address.c_str(), readbuf, size);
	if(ret == MC_ERROR_CODE_SUCCESS){
		val.assign(readbuf, readbuf+size);
	}
	return ret;
}

err_code_t MCProtocol::batch_read_uint64(const reg_address_t address, std::vector<uint64> & val, size_t size) const{
	if(size == 0) return MC_ERROR_CODE_SUCCESS;
	uint64 readbuf[PLC_READ_MAXLEN] = {0};
	int ret = mc_batch_read_uint64(_sock_fd, address.c_str(), readbuf, size);
	if(ret == MC_ERROR_CODE_SUCCESS){
		val.assign(readbuf, readbuf+size);
	}
	return ret;
}

err_code_t MCProtocol::batch_read_float(const reg_address_t address, std::vector<float> & val, size_t size) const{
	if(size == 0) return MC_ERROR_CODE_SUCCESS;
	float readbuf[PLC_READ_MAXLEN] = {0};
	int ret = mc_batch_read_float(_sock_fd, address.c_str(), readbuf, size);
	if(ret == MC_ERROR_CODE_SUCCESS){
		val.assign(readbuf, readbuf+size);
	}
	return ret;
}

err_code_t MCProtocol::batch_read_double(const reg_address_t address, std::vector<double> & val, size_t size) const{
	if(size == 0) return MC_ERROR_CODE_SUCCESS;
	double readbuf[PLC_READ_MAXLEN] = {0};
	int ret = mc_batch_read_double(_sock_fd, address.c_str(), readbuf, size);
	if(ret == MC_ERROR_CODE_SUCCESS){
		val.assign(readbuf, readbuf+size);
	}
	return ret;
}

err_code_t MCProtocol::write_bool(const reg_address_t address, bool val){
	return mc_write_bool(_sock_fd, address.c_str(), val);
}

err_code_t MCProtocol::write_byte(const reg_address_t address, byte val){
	return -1;
}

err_code_t MCProtocol::write_short(const reg_address_t address, short val){
	return mc_write_short(_sock_fd, address.c_str(), val);
}

err_code_t MCProtocol::write_ushort(const reg_address_t address, ushort val){\
	return mc_write_ushort(_sock_fd, address.c_str(), val);
}

err_code_t MCProtocol::write_int32(const reg_address_t address, int32 val){
	return mc_write_int32(_sock_fd, address.c_str(), val);
}

err_code_t MCProtocol::write_uint32(const reg_address_t address, uint32 val){
	return mc_write_uint32(_sock_fd, address.c_str(), val);
}

err_code_t MCProtocol::write_int64(const reg_address_t address, int64 val){
	return mc_write_int64(_sock_fd, address.c_str(), val);
}

err_code_t MCProtocol::write_uint64(const reg_address_t address, uint64 val){
	return mc_write_uint64(_sock_fd, address.c_str(), val);
}

err_code_t MCProtocol::write_float(const reg_address_t address, float val){
	return mc_write_float(_sock_fd, address.c_str(), val);
}

err_code_t MCProtocol::write_double(const reg_address_t address, double val){
	return mc_write_double(_sock_fd, address.c_str(), val);
}

err_code_t MCProtocol::write_string(const reg_address_t address, const std::string & val){
	return mc_write_string(_sock_fd, address.c_str(), val.length(), val.c_str());
}

err_code_t MCProtocol::batch_write_bool(const reg_address_t address, const std::vector<bool> & val){
	if(val.size() == 0) return MC_ERROR_CODE_SUCCESS;
	bool write_buf[128]{0};
	std::copy(val.begin(), val.end(), write_buf);
	return mc_batch_write_bool(_sock_fd, address.c_str(), write_buf, val.size());
}

err_code_t MCProtocol::batch_write_byte(const reg_address_t address, const std::vector<byte> & val){
	return -1;
}

err_code_t MCProtocol::batch_write_short(const reg_address_t address, const std::vector<short> & val){
	if(val.size() == 0) return MC_ERROR_CODE_SUCCESS;
	short write_buf[128]{0};
	std::copy(val.begin(), val.end(), write_buf);
	return mc_batch_write_short(_sock_fd, address.c_str(), write_buf, val.size());
}

err_code_t MCProtocol::batch_write_ushort(const reg_address_t address, const std::vector<ushort> & val){
	if(val.size() == 0) return MC_ERROR_CODE_SUCCESS;
	ushort write_buf[128]{0};
	std::copy(val.begin(), val.end(), write_buf);
	return mc_batch_write_ushort(_sock_fd, address.c_str(), write_buf, val.size());
}

err_code_t MCProtocol::batch_write_int32(const reg_address_t address, const std::vector<int32> & val){
	if(val.size() == 0) return MC_ERROR_CODE_SUCCESS;
	int32 write_buf[128]{0};
	std::copy(val.begin(), val.end(), write_buf);
	return mc_batch_write_int32(_sock_fd, address.c_str(), write_buf, val.size());
}

err_code_t MCProtocol::batch_write_uint32(const reg_address_t address, const std::vector<uint32> & val){
	if(val.size() == 0) return MC_ERROR_CODE_SUCCESS;
	uint32 write_buf[128]{0};
	std::copy(val.begin(), val.end(), write_buf);
	return mc_batch_write_uint32(_sock_fd, address.c_str(), write_buf, val.size());
}

err_code_t MCProtocol::batch_write_int64(const reg_address_t address, const std::vector<int64> & val){
	if(val.size() == 0) return MC_ERROR_CODE_SUCCESS;
	int64 write_buf[128]{0};
	std::copy(val.begin(), val.end(), write_buf);
	return mc_batch_write_int64(_sock_fd, address.c_str(), write_buf, val.size());
}

err_code_t MCProtocol::batch_write_uint64(const reg_address_t address, const std::vector<uint64> & val){
	if(val.size() == 0) return MC_ERROR_CODE_SUCCESS;
	uint64 write_buf[128]{0};
	std::copy(val.begin(), val.end(), write_buf);
	return mc_batch_write_uint64(_sock_fd, address.c_str(), write_buf, val.size());
}

err_code_t MCProtocol::batch_write_float(const reg_address_t address, const std::vector<float> & val){
	if(val.size() == 0) return MC_ERROR_CODE_SUCCESS;
	float write_buf[128]{0};
	std::copy(val.begin(), val.end(), write_buf);
	return mc_batch_write_float(_sock_fd, address.c_str(), write_buf, val.size());
}

err_code_t MCProtocol::batch_write_double(const reg_address_t address, const std::vector<double> & val){
	if(val.size() == 0) return MC_ERROR_CODE_SUCCESS;
	double write_buf[128]{0};
	std::copy(val.begin(), val.end(), write_buf);
	return mc_batch_write_double(_sock_fd, address.c_str(), write_buf, val.size());
}

/*===================S7Protocol=========================*/

S7Protocol::S7Protocol(const std::string & ipaddr, int port, siemens_plc_types_e plc_type)
:PLCProtocol(ipaddr, port), plcType(plc_type)
{
	connect(ipaddr, port, plc_type);
}

S7Protocol::~S7Protocol(){
	disconnect();
}

bool S7Protocol::connect(){
	return this->connect(this->ipaddr, this->port, plcType);
}

bool S7Protocol::connect(const std::string & ipaddr, int port, siemens_plc_types_e plc_type){
	int ret = s7_connect((char*)ipaddr.c_str(), port, plc_type, &_sock_fd);
	if(_sock_fd < 0){
		printf("s7 protocol connection error.\n");
		_is_connected = false;
		return false;
	}
	_is_connected = true;
	return true;
}

bool S7Protocol::connect(const std::string & ipaddr, int port){
	return connect(ipaddr, port, plcType);
}

bool S7Protocol::disconnect(){
	return s7_disconnect(_sock_fd);
}

err_code_t S7Protocol::read_bool(const reg_address_t address, bool* val) const{
	return s7_read_bool(_sock_fd, address.c_str(), val);
}

err_code_t S7Protocol::read_byte(const reg_address_t address, byte* val) const{
	return s7_read_byte(_sock_fd, address.c_str(), val);
}

err_code_t S7Protocol::read_short(const reg_address_t address, short* val) const{
	return s7_read_short(_sock_fd, address.c_str(), val);
}

err_code_t S7Protocol::read_ushort(const reg_address_t address, ushort* val) const{
	return s7_read_ushort(_sock_fd, address.c_str(), val);
}

err_code_t S7Protocol::read_int32(const reg_address_t address, int32* val) const{
	return s7_read_int32(_sock_fd, address.c_str(), val);
}

err_code_t S7Protocol::read_uint32(const reg_address_t address, uint32* val) const{
	return s7_read_uint32(_sock_fd, address.c_str(), val);
}

err_code_t S7Protocol::read_int64(const reg_address_t address, int64* val) const{
	return s7_read_int64(_sock_fd, address.c_str(), val);
}

err_code_t S7Protocol::read_uint64(const reg_address_t address, uint64* val) const{
	return s7_read_uint64(_sock_fd, address.c_str(), val);
}

err_code_t S7Protocol::read_float(const reg_address_t address, float* val) const{
	return s7_read_float(_sock_fd, address.c_str(), val);
}

err_code_t S7Protocol::read_double(const reg_address_t address, double* val) const{
	return s7_read_double(_sock_fd, address.c_str(), val);
}

err_code_t S7Protocol::read_string(const reg_address_t address, int length, std::string & val) const{
	char *readbuf;
	int ret = s7_read_string(_sock_fd, address.c_str(), length, &readbuf);
	if(ret == MC_ERROR_CODE_SUCCESS){
		val.assign(readbuf, length);
		free(readbuf);
	}
	return ret;
}

//******batch read*******/

err_code_t S7Protocol::batch_read_bool(const reg_address_t address, std::vector<bool> & val, size_t size) const{
	if(size == 0) return S7_ERROR_CODE_SUCCESS;
	bool readbuf[PLC_READ_MAXLEN] = {0};
	int ret = s7_batch_read_bool(_sock_fd, address.c_str(), readbuf, size);
	if(ret == S7_ERROR_CODE_SUCCESS){
		val.assign(readbuf, readbuf+size);
	}
	return ret;
}

err_code_t S7Protocol::batch_read_byte(const reg_address_t address, std::vector<byte> & val, size_t size) const{
	if(size == 0) return S7_ERROR_CODE_SUCCESS;
	byte readbuf[PLC_READ_MAXLEN] = {0};
	int ret = s7_batch_read_byte(_sock_fd, address.c_str(), readbuf, size);
	if(ret == S7_ERROR_CODE_SUCCESS){
		val.assign(readbuf, readbuf+size);
	}
	return ret;
}

err_code_t S7Protocol::batch_read_short(const reg_address_t address, std::vector<short> & val, size_t size) const{
	if(size == 0) return S7_ERROR_CODE_SUCCESS;
	short readbuf[PLC_READ_MAXLEN] = {0};
	int ret = s7_batch_read_short(_sock_fd, address.c_str(), readbuf, size);
	if(ret == S7_ERROR_CODE_SUCCESS){
		val.assign(readbuf, readbuf+size);
	}
	return ret;
}

err_code_t S7Protocol::batch_read_ushort(const reg_address_t address, std::vector<ushort> & val, size_t size) const{
	if(size == 0) return S7_ERROR_CODE_SUCCESS;
	ushort readbuf[PLC_READ_MAXLEN] = {0};
	int ret = s7_batch_read_ushort(_sock_fd, address.c_str(), readbuf, size);
	if(ret == S7_ERROR_CODE_SUCCESS){
		val.assign(readbuf, readbuf+size);
	}
	return ret;
}

err_code_t S7Protocol::batch_read_int32(const reg_address_t address, std::vector<int32> & val, size_t size) const{
	if(size == 0) return S7_ERROR_CODE_SUCCESS;
	int32 readbuf[PLC_READ_MAXLEN] = {0};
	int ret = s7_batch_read_int32(_sock_fd, address.c_str(), readbuf, size);
	if(ret == S7_ERROR_CODE_SUCCESS){
		val.assign(readbuf, readbuf+size);
	}
	return ret;
}

err_code_t S7Protocol::batch_read_uint32(const reg_address_t address, std::vector<uint32> & val, size_t size) const{
	if(size == 0) return S7_ERROR_CODE_SUCCESS;
	uint32 readbuf[PLC_READ_MAXLEN] = {0};
	int ret = s7_batch_read_uint32(_sock_fd, address.c_str(), readbuf, size);
	if(ret == S7_ERROR_CODE_SUCCESS){
		val.assign(readbuf, readbuf+size);
	}
	return ret;
}

err_code_t S7Protocol::batch_read_int64(const reg_address_t address, std::vector<int64> & val, size_t size) const{
	if(size == 0) return S7_ERROR_CODE_SUCCESS;
	int64 readbuf[PLC_READ_MAXLEN] = {0};
	int ret = s7_batch_read_int64(_sock_fd, address.c_str(), readbuf, size);
	if(ret == S7_ERROR_CODE_SUCCESS){
		val.assign(readbuf, readbuf+size);
	}
	return ret;
}

err_code_t S7Protocol::batch_read_uint64(const reg_address_t address, std::vector<uint64> & val, size_t size) const{
	if(size == 0) return S7_ERROR_CODE_SUCCESS;
	uint64 readbuf[PLC_READ_MAXLEN] = {0};
	int ret = s7_batch_read_uint64(_sock_fd, address.c_str(), readbuf, size);
	if(ret == S7_ERROR_CODE_SUCCESS){
		val.assign(readbuf, readbuf+size);
	}
	return ret;
}

err_code_t S7Protocol::batch_read_float(const reg_address_t address, std::vector<float> & val, size_t size) const{
	if(size == 0) return S7_ERROR_CODE_SUCCESS;
	float readbuf[PLC_READ_MAXLEN] = {0};
	int ret = s7_batch_read_float(_sock_fd, address.c_str(), readbuf, size);
	if(ret == S7_ERROR_CODE_SUCCESS){
		val.assign(readbuf, readbuf+size);
	}
	return ret;
}

err_code_t S7Protocol::batch_read_double(const reg_address_t address, std::vector<double> & val, size_t size) const{
	if(size == 0) return S7_ERROR_CODE_SUCCESS;
	double readbuf[PLC_READ_MAXLEN] = {0};
	int ret = s7_batch_read_double(_sock_fd, address.c_str(), readbuf, size);
	if(ret == S7_ERROR_CODE_SUCCESS){
		val.assign(readbuf, readbuf+size);
	}
	return ret;
}


err_code_t S7Protocol::write_bool(const reg_address_t address, bool val){
	return s7_write_bool(_sock_fd, address.c_str(), val);
}

err_code_t S7Protocol::write_byte(const reg_address_t address, byte val){
	return s7_write_byte(_sock_fd, address.c_str(), val);
}

err_code_t S7Protocol::write_short(const reg_address_t address, short val){
	return s7_write_short(_sock_fd, address.c_str(), val);
}

err_code_t S7Protocol::write_ushort(const reg_address_t address, ushort val){\
	return s7_write_ushort(_sock_fd, address.c_str(), val);
}

err_code_t S7Protocol::write_int32(const reg_address_t address, int32 val){
	return s7_write_int32(_sock_fd, address.c_str(), val);
}

err_code_t S7Protocol::write_uint32(const reg_address_t address, uint32 val){
	return s7_write_uint32(_sock_fd, address.c_str(), val);
}

err_code_t S7Protocol::write_int64(const reg_address_t address, int64 val){
	return s7_write_int64(_sock_fd, address.c_str(), val);
}

err_code_t S7Protocol::write_uint64(const reg_address_t address, uint64 val){
	return s7_write_uint64(_sock_fd, address.c_str(), val);
}

err_code_t S7Protocol::write_float(const reg_address_t address, float val){
	return s7_write_float(_sock_fd, address.c_str(), val);
}

err_code_t S7Protocol::write_double(const reg_address_t address, double val){
	return s7_write_double(_sock_fd, address.c_str(), val);
}

err_code_t S7Protocol::write_string(const reg_address_t address, const std::string & val){
	return s7_write_string(_sock_fd, address.c_str(), val.length(), val.c_str());
}


/*******batch write*******/

err_code_t S7Protocol::batch_write_bool(const reg_address_t address, const std::vector<bool> & val){
	if(val.size() == 0) return S7_ERROR_CODE_SUCCESS;
	bool write_buf[128]{0};
	std::copy(val.begin(), val.end(), write_buf);
	return s7_batch_write_bool(_sock_fd, address.c_str(), write_buf, val.size());
}

err_code_t S7Protocol::batch_write_byte(const reg_address_t address, const std::vector<byte> & val){
	if(val.size() == 0) return S7_ERROR_CODE_SUCCESS;
	byte write_buf[128]{0};
	std::copy(val.begin(), val.end(), write_buf);
	return s7_batch_write_byte(_sock_fd, address.c_str(), write_buf, val.size());
}

err_code_t S7Protocol::batch_write_short(const reg_address_t address, const std::vector<short> & val){
	if(val.size() == 0) return S7_ERROR_CODE_SUCCESS;
	short write_buf[128]{0};
	std::copy(val.begin(), val.end(), write_buf);
	return s7_batch_write_short(_sock_fd, address.c_str(), write_buf, val.size());
}

err_code_t S7Protocol::batch_write_ushort(const reg_address_t address, const std::vector<ushort> & val){
	if(val.size() == 0) return S7_ERROR_CODE_SUCCESS;
	ushort write_buf[128]{0};
	std::copy(val.begin(), val.end(), write_buf);
	return s7_batch_write_ushort(_sock_fd, address.c_str(), write_buf, val.size());
}

err_code_t S7Protocol::batch_write_int32(const reg_address_t address, const std::vector<int32> & val){
	if(val.size() == 0) return S7_ERROR_CODE_SUCCESS;
	int32 write_buf[128]{0};
	std::copy(val.begin(), val.end(), write_buf);
	return s7_batch_write_int32(_sock_fd, address.c_str(), write_buf, val.size());
}

err_code_t S7Protocol::batch_write_uint32(const reg_address_t address, const std::vector<uint32> & val){
	if(val.size() == 0) return S7_ERROR_CODE_SUCCESS;
	uint32 write_buf[128]{0};
	std::copy(val.begin(), val.end(), write_buf);
	return s7_batch_write_uint32(_sock_fd, address.c_str(), write_buf, val.size());
}

err_code_t S7Protocol::batch_write_int64(const reg_address_t address, const std::vector<int64> & val){
	if(val.size() == 0) return S7_ERROR_CODE_SUCCESS;
	int64 write_buf[128]{0};
	std::copy(val.begin(), val.end(), write_buf);
	return s7_batch_write_int64(_sock_fd, address.c_str(), write_buf, val.size());
}

err_code_t S7Protocol::batch_write_uint64(const reg_address_t address, const std::vector<uint64> & val){
	if(val.size() == 0) return S7_ERROR_CODE_SUCCESS;
	uint64 write_buf[128]{0};
	std::copy(val.begin(), val.end(), write_buf);
	return s7_batch_write_uint64(_sock_fd, address.c_str(), write_buf, val.size());
}

err_code_t S7Protocol::batch_write_float(const reg_address_t address, const std::vector<float> & val){
	if(val.size() == 0) return S7_ERROR_CODE_SUCCESS;
	float write_buf[128]{0};
	std::copy(val.begin(), val.end(), write_buf);
	return s7_batch_write_float(_sock_fd, address.c_str(), write_buf, val.size());
}

err_code_t S7Protocol::batch_write_double(const reg_address_t address, const std::vector<double> & val){
	if(val.size() == 0) return S7_ERROR_CODE_SUCCESS;
	double write_buf[128]{0};
	std::copy(val.begin(), val.end(), write_buf);
	return s7_batch_write_double(_sock_fd, address.c_str(), write_buf, val.size());
}

#ifdef CONFIG_PROTOCOL_MODBUS
/*=============ModBusProtocolImpl==================*/

template<class T>
std::vector<T> convertBigEndianPairsToLittleEndian32(const std::vector<uint16_t>& littleEndianList) {
    std::vector<T> result;
	T* pTemp;
    for (size_t i = 0; i < littleEndianList.size(); i += 2) {
        if(i+1 >= littleEndianList.size()) break; // Skip last if odd
        uint16_t lowWord = littleEndianList[i];
        uint16_t highWord = littleEndianList[i+1];
        uint32_t bigEndianInt = (static_cast<uint32_t>(highWord) << 16) | lowWord;
		pTemp = reinterpret_cast<T*>(&bigEndianInt);
        result.push_back(*pTemp);
    }
    return result;
}

template<class T>
std::vector<T> convertBigEndianPairsToLittleEndian64(const std::vector<uint16_t>& littleEndianList) {
    std::vector<T> result;
	T* pTemp;
    for (size_t i = 0; i < littleEndianList.size(); i += 4) {
        if(i+3 >= littleEndianList.size()) break; // Skip last if odd
        uint16_t lowWord = littleEndianList[i];
        uint16_t highWord = littleEndianList[i+1];
		uint16_t hlowWord = littleEndianList[i+2];
		uint16_t hhighWord = littleEndianList[i+3];
        uint64_t bigEndianInt = static_cast<uint64_t>(hhighWord) << 48 |
								static_cast<uint64_t>(hlowWord) << 32 |
								static_cast<uint64_t>(highWord) << 16 |
								static_cast<uint64_t>(lowWord);
		pTemp = reinterpret_cast<T*>(&bigEndianInt);
        result.push_back(*pTemp);
    }
    return result;
}

template<class T>
std::vector<uint16_t> convertLittleEndianToBigEndianPairs32(const std::vector<T> & bigEndianList){
	std::vector<uint16_t> result;
	uint32_t* pTemp;
	for(auto iter : bigEndianList){
		pTemp = reinterpret_cast<uint32_t*>(&iter);
		uint32_t uint32Low = (*pTemp) & 0xffff;
		uint32_t uint32High = (*pTemp) >> 16;
		result.push_back(static_cast<uint16_t>(uint32Low));
		result.push_back(static_cast<uint16_t>(uint32High));
	}
	return result;
}

template<class T>
std::vector<uint16_t> convertLittleEndianToBigEndianPairs64(const std::vector<T> & bigEndianList){
	std::vector<uint16_t> result;
	uint64_t* pTemp;
	for(auto iter : bigEndianList){
		pTemp = reinterpret_cast<uint64_t*>(&iter);
		uint64_t uint321st = (*pTemp) & 0xffff;
		uint64_t uint322st = ((*pTemp) >> 16) & 0xffff;
		uint64_t uint323st = ((*pTemp) >> 32) & 0xffff;
		uint64_t uint324st = ((*pTemp) >> 48) & 0xffff;
		result.push_back(static_cast<uint16_t>(uint321st));
		result.push_back(static_cast<uint16_t>(uint322st));
		result.push_back(static_cast<uint16_t>(uint323st));
		result.push_back(static_cast<uint16_t>(uint324st));
	}
	return result;
}

ModBusProtocolImpl::ModBusAddressType addressStrAddr2ModBusAddr(const std::string & address){
	ModBusProtocolImpl::ModBusAddressType result = atoi(address.c_str());
	if(result < 0){
		throw std::runtime_error("Bad Modbus address type!");
	}
	return result;
}

void ModBusProtocolImpl::setModbusTimeout(int sec, int usec){
    modbus_set_byte_timeout(modbusContext_, 0, sec);
    modbus_set_response_timeout(modbusContext_, 0, usec);
}

ModBusRegisterType ModBusProtocolImpl::getModbusRegisterTypeByAddress(const ModBusAddressType address) const{
	if(address < 10000)
		return MODBUS_ADDR_TYPE_COIL;
	else if(address >= 10000 && address < 20000)
		return MODBUS_ADDR_TYPE_POINT;
	else if(address >= 30000 && address < 40000)
		return MODBUS_ADDR_TYPE_INPUT;
	else if(address >= 40000 && address < 50000)
		return MODBUS_ADDR_TYPE_OUTPUT;
	return MODBUS_ADDR_TYPE_INVALID;
}

bool ModBusProtocolImpl::chechModbusAddress(const std::string & addressStr, size_t length) const{
	int result = atoi(addressStr.c_str());
	if(result < 0)
		return false;
	if(getModbusRegisterTypeByAddress(static_cast<ModBusAddressType>(result)) !=
		getModbusRegisterTypeByAddress(static_cast<ModBusAddressType>(result) + length))
		return false;
	return true;
}

	//read
err_code_t ModBusProtocolImpl::read_bool(const reg_address_t address, bool* val) const{
	std::vector<bool> read_res(1);
	return batch_read_bool(address, read_res, 1);
}
err_code_t ModBusProtocolImpl::read_byte(const reg_address_t address, byte* val) const{
	return -1;
}

err_code_t ModBusProtocolImpl::read_short(const reg_address_t address, short* val) const{
	std::vector<short> read_res(1);
	return batch_read_short(address, read_res, 1);
}

err_code_t ModBusProtocolImpl::read_ushort(const reg_address_t address, ushort* val) const{
	std::vector<ushort> read_res(1);
	return batch_read_ushort(address, read_res, 1);
}

err_code_t ModBusProtocolImpl::read_int32(const reg_address_t address, int32* val) const{
	std::vector<int32> read_res(1);
	return batch_read_int32(address, read_res, 1);
}

err_code_t ModBusProtocolImpl::read_uint32(const reg_address_t address, uint32* val) const{
	std::vector<uint32> read_res(1);
	return batch_read_uint32(address, read_res, 1);
}

err_code_t ModBusProtocolImpl::read_int64(const reg_address_t address, int64* val) const{
	std::vector<int64> read_res(1);
	return batch_read_int64(address, read_res, 1);
}

err_code_t ModBusProtocolImpl::read_uint64(const reg_address_t address, uint64* val) const{
	std::vector<uint64> read_res(1);
	return batch_read_uint64(address, read_res, 1);
}

err_code_t ModBusProtocolImpl::read_float(const reg_address_t address, float* val) const{
	std::vector<float> read_res(1);
	return batch_read_float(address, read_res, 1);
}

err_code_t ModBusProtocolImpl::read_double(const reg_address_t address, double* val) const{
	std::vector<double> read_res(1);
	return batch_read_double(address, read_res, 1);
}

err_code_t ModBusProtocolImpl::read_string(const reg_address_t address, int length, std::string & val) const{
	int read_len = static_cast<int>(std::ceil(static_cast<double>(length)/2));
	std::vector<uint16_t> readBuf(read_len);
	if(!chechModbusAddress(address, read_len)) return PLCPROTOCOL_ERROR_INVALID_ADDR;
	auto modbusAddr = addressStrAddr2ModBusAddr(address);

	int rc;
	switch (getModbusRegisterTypeByAddress(modbusAddr)){
		case MODBUS_ADDR_TYPE_INPUT:
			rc = modbus_read_input_registers(modbusContext_, modbusAddr, read_len, readBuf.data());
			break;
		case MODBUS_ADDR_TYPE_OUTPUT:
			rc = modbus_read_registers(modbusContext_, modbusAddr, read_len, readBuf.data());
			break;
		default:
			return PLCPROTOCOL_ERROR_INVALID_ADDR;
	}

	if(rc < 0) return rc;
	int rcLength = rc * 2;
	val.resize(rcLength);
	uint8_t* pTemp = reinterpret_cast<uint8_t*>(readBuf.data());
	std::copy(pTemp, pTemp + rcLength, std::back_inserter(val));
	return rcLength;
}


	//batch read
err_code_t ModBusProtocolImpl::batch_read_bool(const reg_address_t address, std::vector<bool> & val, size_t size) const{
	std::vector<uint8_t> readBuf(size);
	if(!chechModbusAddress(address, size)) return PLCPROTOCOL_ERROR_INVALID_ADDR;
	auto modbusAddr = addressStrAddr2ModBusAddr(address);

	int rc;
	switch (getModbusRegisterTypeByAddress(modbusAddr)){
		case MODBUS_ADDR_TYPE_COIL:
			rc = modbus_read_bits(modbusContext_, modbusAddr, size, readBuf.data());
			break;
		case MODBUS_ADDR_TYPE_POINT:
			rc = modbus_read_input_bits(modbusContext_, modbusAddr, size, readBuf.data());
			break;
		default:
			return PLCPROTOCOL_ERROR_INVALID_ADDR;
	}
	if(rc < 0) return rc;
	val.resize(rc);
	std::transform(readBuf.data(), readBuf.data()+rc, val.begin(), [&](uint8_t value){return value & 0x01;});
	return PLCPROTOCOL_ERROR_OK;
}

err_code_t ModBusProtocolImpl::batch_read_byte(const reg_address_t address, std::vector<byte> & val, size_t size) const{
	return -1;
}

err_code_t ModBusProtocolImpl::batch_read_short(const reg_address_t address, std::vector<short> & val, size_t size) const{
	std::vector<uint16_t> readBuf(size);
	if(!chechModbusAddress(address, size)) return PLCPROTOCOL_ERROR_INVALID_ADDR;
	auto modbusAddr = addressStrAddr2ModBusAddr(address);

	int rc;
	switch (getModbusRegisterTypeByAddress(modbusAddr)){
		case MODBUS_ADDR_TYPE_INPUT:
			rc = modbus_read_input_registers(modbusContext_, modbusAddr, size, readBuf.data());
			break;
		case MODBUS_ADDR_TYPE_OUTPUT:
			rc = modbus_read_registers(modbusContext_, modbusAddr, size, readBuf.data());
			break;
		default:
			return PLCPROTOCOL_ERROR_INVALID_ADDR;
	}

	if(rc < 0) return rc;
	val.resize(rc);
	std::transform(readBuf.data(), readBuf.data()+rc, val.begin(), [&](uint16_t value){return (short)value;});
	return PLCPROTOCOL_ERROR_OK;
}

err_code_t ModBusProtocolImpl::batch_read_ushort(const reg_address_t address, std::vector<ushort> & val, size_t size) const{
	std::vector<uint16_t> readBuf(size);
	if(!chechModbusAddress(address, size)) return PLCPROTOCOL_ERROR_INVALID_ADDR;
	auto modbusAddr = addressStrAddr2ModBusAddr(address);

	int rc;
	switch (getModbusRegisterTypeByAddress(modbusAddr)){
		case MODBUS_ADDR_TYPE_INPUT:
			rc = modbus_read_input_registers(modbusContext_, modbusAddr, size, readBuf.data());
			break;
		case MODBUS_ADDR_TYPE_OUTPUT:
			rc = modbus_read_registers(modbusContext_, modbusAddr, size, readBuf.data());
			break;
		default:
			return PLCPROTOCOL_ERROR_INVALID_ADDR;
	}

	if(rc < 0) return rc;
	val.assign(readBuf.data(), readBuf.data()+rc);
	return PLCPROTOCOL_ERROR_OK;
}


err_code_t ModBusProtocolImpl::batch_read_int32(const reg_address_t address, std::vector<int32> & val, size_t size) const{
	int read_len = size * 2;
	std::vector<uint16_t> readBuf(read_len);
	if(!chechModbusAddress(address, read_len)) return PLCPROTOCOL_ERROR_INVALID_ADDR;
	auto modbusAddr = addressStrAddr2ModBusAddr(address);

	int rc = 0;
	switch (getModbusRegisterTypeByAddress(modbusAddr)){
		case MODBUS_ADDR_TYPE_INPUT:
			rc = modbus_read_input_registers(modbusContext_, modbusAddr, read_len, readBuf.data());
			break;
		case MODBUS_ADDR_TYPE_OUTPUT:
			rc = modbus_read_registers(modbusContext_, modbusAddr, read_len, readBuf.data());
			break;
		default:
			return PLCPROTOCOL_ERROR_INVALID_ADDR;
	}
	if(rc < 0) return rc;
	std::vector<uint16_t> new_vec;
	std::copy(readBuf.data(), readBuf.data() + rc, std::back_inserter(new_vec));
	auto littleEndian = convertBigEndianPairsToLittleEndian32<int32>(new_vec);
	val.assign(littleEndian.begin(), littleEndian.end());
	return PLCPROTOCOL_ERROR_OK;
}

err_code_t ModBusProtocolImpl::batch_read_uint32(const reg_address_t address, std::vector<uint32> & val, size_t size) const{
	int read_len = size * 2;
	std::vector<uint16_t> readBuf(read_len);
	if(!chechModbusAddress(address, read_len)) return PLCPROTOCOL_ERROR_INVALID_ADDR;
	auto modbusAddr = addressStrAddr2ModBusAddr(address);

	int rc;
	switch (getModbusRegisterTypeByAddress(modbusAddr)){
		case MODBUS_ADDR_TYPE_INPUT:
			rc = modbus_read_input_registers(modbusContext_, modbusAddr, read_len, readBuf.data());
			break;
		case MODBUS_ADDR_TYPE_OUTPUT:
			rc = modbus_read_registers(modbusContext_, modbusAddr, read_len, readBuf.data());
			break;
		default:
			return PLCPROTOCOL_ERROR_INVALID_ADDR;
	}

	if(rc < 0) return rc;
	LOG_DEBUG << "Read data: " << rc;
	for(int i = 0; i < rc; i++)
		printf("[%x]", readBuf.data()[i]);
	std::vector<uint16_t> new_vec;
	std::copy(readBuf.data(), readBuf.data() + rc, std::back_inserter(new_vec));
	auto littleEndian = convertBigEndianPairsToLittleEndian32<uint32>(new_vec);
	val.assign(littleEndian.begin(), littleEndian.end());
	return PLCPROTOCOL_ERROR_OK;
}

err_code_t ModBusProtocolImpl::batch_read_int64(const reg_address_t address, std::vector<int64> & val, size_t size) const{
	int read_len = size * 4;
	std::vector<uint16_t> readBuf(read_len);
	if(!chechModbusAddress(address, read_len)) return PLCPROTOCOL_ERROR_INVALID_ADDR;
	auto modbusAddr = addressStrAddr2ModBusAddr(address);

	int rc;
	switch (getModbusRegisterTypeByAddress(modbusAddr)){
		case MODBUS_ADDR_TYPE_INPUT:
			rc = modbus_read_input_registers(modbusContext_, modbusAddr, read_len, readBuf.data());
			break;
		case MODBUS_ADDR_TYPE_OUTPUT:
			rc = modbus_read_registers(modbusContext_, modbusAddr, read_len, readBuf.data());
			break;
		default:
			return PLCPROTOCOL_ERROR_INVALID_ADDR;
	}

	if(rc < 0) return rc;
	std::vector<uint16_t> new_vec;
	std::copy(readBuf.data(), readBuf.data() + rc, std::back_inserter(new_vec));
	auto littleEndian = convertBigEndianPairsToLittleEndian64<int64>(new_vec);
	val.assign(littleEndian.begin(), littleEndian.end());
	return PLCPROTOCOL_ERROR_OK;
}

err_code_t ModBusProtocolImpl::batch_read_uint64(const reg_address_t address, std::vector<uint64> & val, size_t size) const{
	int read_len = size * 4;
	std::vector<uint16_t> readBuf(read_len);
	if(!chechModbusAddress(address, read_len)) return PLCPROTOCOL_ERROR_INVALID_ADDR;
	auto modbusAddr = addressStrAddr2ModBusAddr(address);

	int rc;
	switch (getModbusRegisterTypeByAddress(modbusAddr)){
		case MODBUS_ADDR_TYPE_INPUT:
			rc = modbus_read_input_registers(modbusContext_, modbusAddr, read_len, readBuf.data());
			break;
		case MODBUS_ADDR_TYPE_OUTPUT:
			rc = modbus_read_registers(modbusContext_, modbusAddr, read_len, readBuf.data());
			break;
		default:
			return PLCPROTOCOL_ERROR_INVALID_ADDR;
	}

	if(rc < 0) return rc;
	std::vector<uint16_t> new_vec;
	std::copy(readBuf.data(), readBuf.data() + rc, std::back_inserter(new_vec));
	auto littleEndian = convertBigEndianPairsToLittleEndian64<uint64>(new_vec);
	val.assign(littleEndian.begin(), littleEndian.end());
	return PLCPROTOCOL_ERROR_OK;
}

err_code_t ModBusProtocolImpl::batch_read_float(const reg_address_t address, std::vector<float> & val, size_t size) const{
	int read_len = size * 2;
	std::vector<uint16_t> readBuf(read_len);
	if(!chechModbusAddress(address, read_len)) return PLCPROTOCOL_ERROR_INVALID_ADDR;
	auto modbusAddr = addressStrAddr2ModBusAddr(address);

	int rc;
	switch (getModbusRegisterTypeByAddress(modbusAddr)){
		case MODBUS_ADDR_TYPE_INPUT:
			rc = modbus_read_input_registers(modbusContext_, modbusAddr, read_len, readBuf.data());
			break;
		case MODBUS_ADDR_TYPE_OUTPUT:
			rc = modbus_read_registers(modbusContext_, modbusAddr, read_len, readBuf.data());
			break;
		default:
			return PLCPROTOCOL_ERROR_INVALID_ADDR;
	}

	if(rc < 0) return rc;
	for(auto iter : readBuf)
		printf("[%d]", iter);
	printf("\n");
	std::vector<uint16_t> new_vec;
	std::copy(readBuf.data(), readBuf.data() + rc, std::back_inserter(new_vec));
	auto littleEndian = convertBigEndianPairsToLittleEndian32<float>(new_vec);
	val.assign(littleEndian.begin(), littleEndian.end());
	return PLCPROTOCOL_ERROR_OK;
}

err_code_t ModBusProtocolImpl::batch_read_double(const reg_address_t address, std::vector<double> & val, size_t size) const{
	int read_len = size * 4;
	std::vector<uint16_t> readBuf(read_len);
	if(!chechModbusAddress(address, read_len)) return PLCPROTOCOL_ERROR_INVALID_ADDR;
	auto modbusAddr = addressStrAddr2ModBusAddr(address);

	int rc;
	switch (getModbusRegisterTypeByAddress(modbusAddr)){
		case MODBUS_ADDR_TYPE_INPUT:
			rc = modbus_read_input_registers(modbusContext_, modbusAddr, read_len, readBuf.data());
			break;
		case MODBUS_ADDR_TYPE_OUTPUT:
			rc = modbus_read_registers(modbusContext_, modbusAddr, read_len, readBuf.data());
			break;
		default:
			return PLCPROTOCOL_ERROR_INVALID_ADDR;
	}

	if(rc < 0) return rc;
	std::vector<uint16_t> new_vec;
	std::copy(readBuf.data(), readBuf.data() + rc, std::back_inserter(new_vec));
	auto littleEndian = convertBigEndianPairsToLittleEndian64<double>(new_vec);
	val.assign(littleEndian.begin(), littleEndian.end());
	return PLCPROTOCOL_ERROR_OK;
}

	//write
err_code_t ModBusProtocolImpl::write_bool(const reg_address_t address, bool val){
	std::vector<bool> write_val;
	write_val.push_back(val);
	return batch_write_bool(address, write_val);
}

err_code_t ModBusProtocolImpl::write_byte(const reg_address_t address, byte val){
	return -1;
}

err_code_t ModBusProtocolImpl::write_short(const reg_address_t address, short val){
	std::vector<short> write_val;
	write_val.push_back(val);
	return batch_write_short(address, write_val);
}

err_code_t ModBusProtocolImpl::write_ushort(const reg_address_t address, ushort val){
	std::vector<ushort> write_val;
	write_val.push_back(val);
	return batch_write_ushort(address, write_val);
}

err_code_t ModBusProtocolImpl::write_int32(const reg_address_t address, int32 val){
	std::vector<int32> write_val;
	write_val.push_back(val);
	return batch_write_int32(address, write_val);
}

err_code_t ModBusProtocolImpl::write_uint32(const reg_address_t address, uint32 val){
	std::vector<uint32> write_val;
	write_val.push_back(val);
	return batch_write_uint32(address, write_val);
}

err_code_t ModBusProtocolImpl::write_int64(const reg_address_t address, int64 val){
	std::vector<int64> write_val;
	write_val.push_back(val);
	return batch_write_int64(address, write_val);
}

err_code_t ModBusProtocolImpl::write_uint64(const reg_address_t address, uint64 val){
	std::vector<uint64> write_val;
	write_val.push_back(val);
	return batch_write_uint64(address, write_val);
}

err_code_t ModBusProtocolImpl::write_float(const reg_address_t address, float val){
	std::vector<float> write_val;
	write_val.push_back(val);
	return batch_write_float(address, write_val);
}

err_code_t ModBusProtocolImpl::write_double(const reg_address_t address, double val){
	std::vector<double> write_val;
	write_val.push_back(val);
	return batch_write_double(address, write_val);
}

err_code_t ModBusProtocolImpl::write_string(const reg_address_t address, const std::string & val){
	int write_len = (val.length()%2)==0 ? val.length()/2 : (val.length()+1)/2;
	std::vector<uint16_t> wordBuf(write_len);
	if(!chechModbusAddress(address, write_len)) return PLCPROTOCOL_ERROR_INVALID_ADDR;
	auto modbusAddr = addressStrAddr2ModBusAddr(address);

	uint8_t* pTemp = reinterpret_cast<uint8_t*>(wordBuf.data());
	std::copy(val.begin(), val.end(), pTemp);

	int rc;
	switch (getModbusRegisterTypeByAddress(modbusAddr))
	{
	case MODBUS_ADDR_TYPE_OUTPUT:
		rc = modbus_write_registers(modbusContext_, modbusAddr, write_len, wordBuf.data());
		break;
	
	default:
		return PLCPROTOCOL_ERROR_INVALID_ADDR;
	}
	if(rc < 0)return rc;
	return rc;
}


	//batch write
err_code_t ModBusProtocolImpl::batch_write_bool(const reg_address_t address, const std::vector<bool> & val){
	int write_len = val.size();
	std::vector<uint8_t> bitBuf(write_len);
	if(!chechModbusAddress(address, write_len)) return PLCPROTOCOL_ERROR_INVALID_ADDR;
	auto modbusAddr = addressStrAddr2ModBusAddr(address);

	std::transform(val.begin(), val.end(), bitBuf.data(), 
					[&](bool value){return uint8_t(value);});

	int rc;
	switch (getModbusRegisterTypeByAddress(modbusAddr))
	{
	case MODBUS_ADDR_TYPE_COIL:
		rc = modbus_write_bits(modbusContext_, modbusAddr, write_len, bitBuf.data());
		break;
	
	default:
		return PLCPROTOCOL_ERROR_INVALID_ADDR;
	}
	if(rc < 0)return rc;
	return PLCPROTOCOL_ERROR_OK;
}

err_code_t ModBusProtocolImpl::batch_write_byte(const reg_address_t address, const std::vector<byte> & val){
	return -1;
}

err_code_t ModBusProtocolImpl::batch_write_short(const reg_address_t address, const std::vector<short> & val){
	int write_len = val.size();
	std::vector<uint16_t> bitBuf(write_len);
	if(!chechModbusAddress(address, write_len)) return PLCPROTOCOL_ERROR_INVALID_ADDR;
	auto modbusAddr = addressStrAddr2ModBusAddr(address);

	std::transform(val.begin(), val.end(), bitBuf.data(),
					[&](short value){return uint16_t(value);});

	int rc;
	switch (getModbusRegisterTypeByAddress(modbusAddr))
	{
	case MODBUS_ADDR_TYPE_OUTPUT:
		rc = modbus_write_registers(modbusContext_, modbusAddr, write_len, bitBuf.data());
		break;
	
	default:
		return PLCPROTOCOL_ERROR_INVALID_ADDR;
	}
	if(rc < 0)return rc;
	return PLCPROTOCOL_ERROR_OK;
}

err_code_t ModBusProtocolImpl::batch_write_ushort(const reg_address_t address, const std::vector<ushort> & val){
	int write_len = val.size();
	std::vector<uint16_t> bitBuf(write_len);
	if(!chechModbusAddress(address, write_len)) return PLCPROTOCOL_ERROR_INVALID_ADDR;
	auto modbusAddr = addressStrAddr2ModBusAddr(address);

	std::transform(val.begin(), val.end(), bitBuf.data(),
					[&](short value){return uint16_t(value);});

	int rc;
	switch (getModbusRegisterTypeByAddress(modbusAddr))
	{
	case MODBUS_ADDR_TYPE_OUTPUT:
		rc = modbus_write_registers(modbusContext_, modbusAddr, write_len, bitBuf.data());
		break;
	
	default:
		return PLCPROTOCOL_ERROR_INVALID_ADDR;
	}
	if(rc < 0)return rc;
	return PLCPROTOCOL_ERROR_OK;
}

err_code_t ModBusProtocolImpl::batch_write_int32(const reg_address_t address, const std::vector<int32> & val){
	int write_len = val.size()*2;
	if(!chechModbusAddress(address, write_len)) return PLCPROTOCOL_ERROR_INVALID_ADDR;
	auto modbusAddr = addressStrAddr2ModBusAddr(address);

	auto write_buf = convertLittleEndianToBigEndianPairs32(val);

	int rc;
	switch (getModbusRegisterTypeByAddress(modbusAddr))
	{
	case MODBUS_ADDR_TYPE_OUTPUT:
		rc = modbus_write_registers(modbusContext_, modbusAddr, write_len, write_buf.data());
		break;
	
	default:
		return PLCPROTOCOL_ERROR_INVALID_ADDR;
	}
	if(rc < 0)return rc;
	return PLCPROTOCOL_ERROR_OK;
}

err_code_t ModBusProtocolImpl::batch_write_uint32(const reg_address_t address, const std::vector<uint32> & val){
	int write_len = val.size()*2;
	if(!chechModbusAddress(address, write_len)) return PLCPROTOCOL_ERROR_INVALID_ADDR;
	auto modbusAddr = addressStrAddr2ModBusAddr(address);

	auto write_buf = convertLittleEndianToBigEndianPairs32(val);

	int rc;
	switch (getModbusRegisterTypeByAddress(modbusAddr))
	{
	case MODBUS_ADDR_TYPE_OUTPUT:
		rc = modbus_write_registers(modbusContext_, modbusAddr, write_len, write_buf.data());
		break;
	
	default:
		return PLCPROTOCOL_ERROR_INVALID_ADDR;
	}
	if(rc < 0)return rc;
	return PLCPROTOCOL_ERROR_OK;
}

err_code_t ModBusProtocolImpl::batch_write_int64(const reg_address_t address, const std::vector<int64> & val){
	int write_len = val.size()*4;
	if(!chechModbusAddress(address, write_len)) return PLCPROTOCOL_ERROR_INVALID_ADDR;
	auto modbusAddr = addressStrAddr2ModBusAddr(address);

	auto write_buf = convertLittleEndianToBigEndianPairs64(val);

	int rc;
	switch (getModbusRegisterTypeByAddress(modbusAddr))
	{
	case MODBUS_ADDR_TYPE_OUTPUT:
		rc = modbus_write_registers(modbusContext_, modbusAddr, write_len, write_buf.data());
		break;
	
	default:
		return PLCPROTOCOL_ERROR_INVALID_ADDR;
	}
	if(rc < 0)return rc;
	return PLCPROTOCOL_ERROR_OK;
}

err_code_t ModBusProtocolImpl::batch_write_uint64(const reg_address_t address, const std::vector<uint64> & val){
	int write_len = val.size()*4;
	if(!chechModbusAddress(address, write_len)) return PLCPROTOCOL_ERROR_INVALID_ADDR;
	auto modbusAddr = addressStrAddr2ModBusAddr(address);

	auto write_buf = convertLittleEndianToBigEndianPairs64(val);

	int rc;
	switch (getModbusRegisterTypeByAddress(modbusAddr))
	{
	case MODBUS_ADDR_TYPE_OUTPUT:
		rc = modbus_write_registers(modbusContext_, modbusAddr, write_len, write_buf.data());
		break;
	
	default:
		return PLCPROTOCOL_ERROR_INVALID_ADDR;
	}
	if(rc < 0)return rc;
	return PLCPROTOCOL_ERROR_OK;
}

err_code_t ModBusProtocolImpl::batch_write_float(const reg_address_t address, const std::vector<float> & val){
	int write_len = val.size()*2;
	if(!chechModbusAddress(address, write_len)) return PLCPROTOCOL_ERROR_INVALID_ADDR;
	auto modbusAddr = addressStrAddr2ModBusAddr(address);

	auto write_buf = convertLittleEndianToBigEndianPairs32(val);
	for(auto iter : write_buf)
		printf("[%d]", iter);
	printf("\n");

	int rc;
	switch (getModbusRegisterTypeByAddress(modbusAddr))
	{
	case MODBUS_ADDR_TYPE_OUTPUT:
		rc = modbus_write_registers(modbusContext_, modbusAddr, write_len, write_buf.data());
		break;
	
	default:
		return PLCPROTOCOL_ERROR_INVALID_ADDR;
	}
	if(rc < 0)return rc;
	return PLCPROTOCOL_ERROR_OK;
}

err_code_t ModBusProtocolImpl::batch_write_double(const reg_address_t address, const std::vector<double> & val){
	int write_len = val.size()*4;
	if(!chechModbusAddress(address, write_len)) return PLCPROTOCOL_ERROR_INVALID_ADDR;
	auto modbusAddr = addressStrAddr2ModBusAddr(address);

	auto write_buf = convertLittleEndianToBigEndianPairs64(val);

	int rc;
	switch (getModbusRegisterTypeByAddress(modbusAddr))
	{
	case MODBUS_ADDR_TYPE_OUTPUT:
		rc = modbus_write_registers(modbusContext_, modbusAddr, write_len, write_buf.data());
		break;
	
	default:
		return PLCPROTOCOL_ERROR_INVALID_ADDR;
	}
	if(rc < 0)return rc;
	return PLCPROTOCOL_ERROR_OK;
}

/*==============ModBusTCPProtocol===============*/

ModBusTCPProtocol::ModBusTCPProtocol(const std::string & ip_, int port_, int slave)
{
	ipaddr = ip_;
	port = port_;
	slaveNum_ = slave;
}

ModBusTCPProtocol::~ModBusTCPProtocol(){
	disconnect();
}

bool ModBusTCPProtocol::connect(){
	return connect(ipaddr, port, slaveNum_);
}

bool ModBusTCPProtocol::connect(const std::string & ipaddr, int port, int slave){
	modbusContext_ = modbus_new_tcp(ipaddr.c_str(), port);
	if(modbusContext_ == NULL){
		throw std::bad_alloc();
	}

	modbus_set_slave(modbusContext_, slave);

    modbus_set_error_recovery(
        modbusContext_, (modbus_error_recovery_mode)(MODBUS_ERROR_RECOVERY_LINK | MODBUS_ERROR_RECOVERY_PROTOCOL));
	setModbusTimeout(timeoutSec_, timeoutUSec_);

    if (modbus_connect(modbusContext_) < 0) {
		_is_connected = false;
        return false;
    }
	_is_connected = true;
	return true;
}

bool ModBusTCPProtocol::disconnect(){
	if(modbusContext_){
		_is_connected = false;
		modbus_close(modbusContext_);
		modbus_free(modbusContext_);
	}
}

/*=============ModBusRTUProtocol=================*/

ModBusRTUProtocol::ModBusRTUProtocol(const std::string & deviceName, 
					int bautRate, char parity,
					int dataBits, int stopBits,
					int slave)
:deviceName_(deviceName),
bautRate_(bautRate),
parity_(parity),
dataBits_(dataBits),
stopBits_(stopBits)
{
	slaveNum_ = slave;
}

bool ModBusRTUProtocol::connect(){
	return connect(deviceName_,
					bautRate_,
					parity_,
					dataBits_,
					stopBits_,
					slaveNum_);
}

bool ModBusRTUProtocol::connect(const std::string & deviceName, 
						int bautRate, char parity,
						int dataBits, int stopBits,
						int slaveNum_){
	modbusContext_ = modbus_new_rtu(deviceName.c_str(), 
									bautRate,
									parity,
									dataBits,
									stopBits);
	if(modbusContext_ == NULL){
		throw std::bad_alloc();
	}

	modbus_set_slave(modbusContext_, slaveNum_);

    modbus_set_error_recovery(
        modbusContext_, (modbus_error_recovery_mode)(MODBUS_ERROR_RECOVERY_LINK | MODBUS_ERROR_RECOVERY_PROTOCOL));
	setModbusTimeout(timeoutSec_, timeoutUSec_);

    if (modbus_connect(modbusContext_) < 0) {
		_is_connected = false;
        return false;
    }
	_is_connected = true;
	return true;
}

bool ModBusRTUProtocol::disconnect(){
	modbus_close(modbusContext_);
	modbus_free(modbusContext_);
}

#endif