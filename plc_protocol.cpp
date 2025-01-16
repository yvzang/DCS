#include <string.h>
#include <errno.h>
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
	connect(ipaddr, port);
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
	_sock_fd = s7_connect((char*)ipaddr.c_str(), port, plc_type, &_sock_fd);
	if(_sock_fd < 0){
		printf("s7 protocol connection error.\n");
		return false;
	}
	return true;
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