#ifndef __PLCPROTOCOL_H__
#define __PLCPROTOCOL_H__
#include <string>
extern "C"{
#include "external/melsec_mc_net/melsec_mc_bin.h"
#include "external/siemens_plc_s7_net/siemens_s7.h"
}

typedef uint32_t err_code_t;
typedef std::string reg_address_t;
typedef unsigned char byte;
typedef unsigned short ushort;
typedef signed int	int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;

class PLCProtocol{
protected:
	int _sock_fd = 0;
	int _is_connected = false;
public:
	pthread_mutex_t rw_lock;
	std::string ipaddr;
	int port;
	PLCProtocol(){};
	PLCProtocol(const std::string & ipaddr, int port);
	bool reconnect();
	bool reconnect(const std::string & ipaddr, int port);
	bool is_connected(){return _is_connected;};
	virtual ~PLCProtocol(){};
	virtual	bool connect() = 0;
	virtual bool disconnect() = 0;
	virtual bool connect(const std::string & ipaddr, int port) = 0;
	virtual err_code_t read_bool(const reg_address_t address, bool* val) const = 0;
	virtual err_code_t read_byte(const reg_address_t address, byte* val) const = 0;
	virtual err_code_t read_short(const reg_address_t address, short* val) const = 0;
	virtual err_code_t read_ushort(const reg_address_t address, ushort* val) const = 0;
	virtual err_code_t read_int32(const reg_address_t address, int32* val) const = 0;
	virtual err_code_t read_uint32(const reg_address_t address, uint32* val) const = 0;
	virtual err_code_t read_int64(const reg_address_t address, int64* val) const = 0;
	virtual err_code_t read_uint64(const reg_address_t address, uint64* val) const = 0;
	virtual err_code_t read_float(const reg_address_t address, float* val) const = 0;
	virtual err_code_t read_double(const reg_address_t address, double* val) const = 0;
	virtual err_code_t read_string(const reg_address_t address, int length, std::string & val) const = 0; //need free val

	//write
	virtual err_code_t write_bool(const reg_address_t address, bool val) = 0;
	virtual err_code_t write_byte(const reg_address_t address, byte val) = 0;
	virtual err_code_t write_short(const reg_address_t address, short val) = 0;
	virtual err_code_t write_ushort(const reg_address_t address, ushort val) = 0;
	virtual err_code_t write_int32(const reg_address_t address, int32 val) = 0;
	virtual err_code_t write_uint32(const reg_address_t address, uint32 val) = 0;
	virtual err_code_t write_int64(const reg_address_t address, int64 val) = 0;
	virtual err_code_t write_uint64(const reg_address_t address, uint64 val) = 0;
	virtual err_code_t write_float(const reg_address_t address, float val) = 0;
	virtual err_code_t write_double(const reg_address_t address, double val) = 0;
	virtual err_code_t write_string(const reg_address_t address, const std::string & val) = 0;
};

class MCProtocol: public PLCProtocol{
public:
	MCProtocol(){}
	MCProtocol(const std::string & ipaddr, int port);
	virtual ~MCProtocol();
	virtual	bool connect();
	virtual bool disconnect();
	virtual bool connect(const std::string & ipaddr, int port);
	virtual err_code_t read_bool(const reg_address_t address, bool* val) const;
	virtual err_code_t read_byte(const reg_address_t address, byte* val) const;
	virtual err_code_t read_short(const reg_address_t address, short* val) const;
	virtual err_code_t read_ushort(const reg_address_t address, ushort* val) const;
	virtual err_code_t read_int32(const reg_address_t address, int32* val) const;
	virtual err_code_t read_uint32(const reg_address_t address, uint32* val) const;
	virtual err_code_t read_int64(const reg_address_t address, int64* val) const;
	virtual err_code_t read_uint64(const reg_address_t address, uint64* val) const;
	virtual err_code_t read_float(const reg_address_t address, float* val) const;
	virtual err_code_t read_double(const reg_address_t address, double* val) const;
	virtual err_code_t read_string(const reg_address_t address, int length, std::string & val) const; //need free val

	//write
	virtual err_code_t write_bool(const reg_address_t address, bool val);
	virtual err_code_t write_byte(const reg_address_t address, byte val);
	virtual err_code_t write_short(const reg_address_t address, short val);
	virtual err_code_t write_ushort(const reg_address_t address, ushort val);
	virtual err_code_t write_int32(const reg_address_t address, int32 val);
	virtual err_code_t write_uint32(const reg_address_t address, uint32 val);
	virtual err_code_t write_int64(const reg_address_t address, int64 val);
	virtual err_code_t write_uint64(const reg_address_t address, uint64 val);
	virtual err_code_t write_float(const reg_address_t address, float val);
	virtual err_code_t write_double(const reg_address_t address, double val);
	virtual err_code_t write_string(const reg_address_t address, const std::string & val);
};

class S7Protocol: public PLCProtocol{
public:
	siemens_plc_types_e plcType;
	S7Protocol(){}
	S7Protocol(const std::string & ipaddr, int port, siemens_plc_types_e plc_tyle);
	virtual ~S7Protocol();
	virtual	bool connect();
	virtual bool disconnect();
	virtual bool connect(const std::string & ipaddr, int port, siemens_plc_types_e plc_type);
	virtual err_code_t read_bool(const reg_address_t address, bool* val) const;
	virtual err_code_t read_byte(const reg_address_t address, byte* val) const;
	virtual err_code_t read_short(const reg_address_t address, short* val) const;
	virtual err_code_t read_ushort(const reg_address_t address, ushort* val) const;
	virtual err_code_t read_int32(const reg_address_t address, int32* val) const;
	virtual err_code_t read_uint32(const reg_address_t address, uint32* val) const;
	virtual err_code_t read_int64(const reg_address_t address, int64* val) const;
	virtual err_code_t read_uint64(const reg_address_t address, uint64* val) const;
	virtual err_code_t read_float(const reg_address_t address, float* val) const;
	virtual err_code_t read_double(const reg_address_t address, double* val) const;
	virtual err_code_t read_string(const reg_address_t address, int length, std::string & val) const; //need free val

	//write
	virtual err_code_t write_bool(const reg_address_t address, bool val);
	virtual err_code_t write_byte(const reg_address_t address, byte val);
	virtual err_code_t write_short(const reg_address_t address, short val);
	virtual err_code_t write_ushort(const reg_address_t address, ushort val);
	virtual err_code_t write_int32(const reg_address_t address, int32 val);
	virtual err_code_t write_uint32(const reg_address_t address, uint32 val);
	virtual err_code_t write_int64(const reg_address_t address, int64 val);
	virtual err_code_t write_uint64(const reg_address_t address, uint64 val);
	virtual err_code_t write_float(const reg_address_t address, float val);
	virtual err_code_t write_double(const reg_address_t address, double val);
	virtual err_code_t write_string(const reg_address_t address, const std::string & val);
};

#endif