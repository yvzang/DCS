#ifndef __PLCPROTOCOL_H__
#define __PLCPROTOCOL_H__
#include <string>
#include <vector>
extern "C"{
#include "external/melsec_mc_net/melsec_mc_bin.h"
#include "external/siemens_plc_s7_net/siemens_s7.h"
#ifdef CONFIG_PROTOCOL_MODBUS
#include "modbus/modbus.h"
#include "modbus/modbus-tcp.h"
#endif
}

#define PLC_READ_MAXLEN						1024

typedef enum{
	PLCPROTOCOL_ERROR_OK = 0,					//ok
	PLCPROTOCOL_ERROR_INVALID_ADDR = 0xff,		//invalie address
	PLCPROTOCOL_ERROR_ADDRESS_CROSS,			

} PLCProtocolErrorCode;

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

	//read
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

	//batch read
	virtual err_code_t batch_read_bool(const reg_address_t address, std::vector<bool> & val, size_t size) const = 0;
	virtual err_code_t batch_read_byte(const reg_address_t address, std::vector<byte> & val, size_t size) const = 0;
	virtual err_code_t batch_read_short(const reg_address_t address, std::vector<short> & val, size_t size) const = 0;
	virtual err_code_t batch_read_ushort(const reg_address_t address, std::vector<ushort> & val, size_t size) const = 0;
	virtual err_code_t batch_read_int32(const reg_address_t address, std::vector<int32> & val, size_t size) const = 0;
	virtual err_code_t batch_read_uint32(const reg_address_t address, std::vector<uint32> & val, size_t size) const = 0;
	virtual err_code_t batch_read_int64(const reg_address_t address, std::vector<int64> & val, size_t size) const = 0;
	virtual err_code_t batch_read_uint64(const reg_address_t address, std::vector<uint64> & val, size_t size) const = 0;
	virtual err_code_t batch_read_float(const reg_address_t address, std::vector<float> & val, size_t size) const = 0;
	virtual err_code_t batch_read_double(const reg_address_t address, std::vector<double> & val, size_t size) const = 0;

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

	//batch write
	virtual err_code_t batch_write_bool(const reg_address_t address, const std::vector<bool> & val) = 0;
	virtual err_code_t batch_write_byte(const reg_address_t address, const std::vector<byte> & val) = 0;
	virtual err_code_t batch_write_short(const reg_address_t address, const std::vector<short> & val) = 0;
	virtual err_code_t batch_write_ushort(const reg_address_t address, const std::vector<ushort> & val) = 0;
	virtual err_code_t batch_write_int32(const reg_address_t address, const std::vector<int32> & val) = 0;
	virtual err_code_t batch_write_uint32(const reg_address_t address, const std::vector<uint32> & val) = 0;
	virtual err_code_t batch_write_int64(const reg_address_t address, const std::vector<int64> & val) = 0;
	virtual err_code_t batch_write_uint64(const reg_address_t address, const std::vector<uint64> & val) = 0;
	virtual err_code_t batch_write_float(const reg_address_t address, const std::vector<float> & val) = 0;
	virtual err_code_t batch_write_double(const reg_address_t address, const std::vector<double> & val) = 0;
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

	//batch read
	virtual err_code_t batch_read_bool(const reg_address_t address, std::vector<bool> & val, size_t size) const;
	virtual err_code_t batch_read_byte(const reg_address_t address, std::vector<byte> & val, size_t size) const;
	virtual err_code_t batch_read_short(const reg_address_t address, std::vector<short> & val, size_t size) const;
	virtual err_code_t batch_read_ushort(const reg_address_t address, std::vector<ushort> & val, size_t size) const;
	virtual err_code_t batch_read_int32(const reg_address_t address, std::vector<int32> & val, size_t size) const;
	virtual err_code_t batch_read_uint32(const reg_address_t address, std::vector<uint32> & val, size_t size) const;
	virtual err_code_t batch_read_int64(const reg_address_t address, std::vector<int64> & val, size_t size) const;
	virtual err_code_t batch_read_uint64(const reg_address_t address, std::vector<uint64> & val, size_t size) const;
	virtual err_code_t batch_read_float(const reg_address_t address, std::vector<float> & val, size_t size) const;
	virtual err_code_t batch_read_double(const reg_address_t address, std::vector<double> & val, size_t size) const;

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

	//batch write
	virtual err_code_t batch_write_bool(const reg_address_t address, const std::vector<bool> & val);
	virtual err_code_t batch_write_byte(const reg_address_t address, const std::vector<byte> & val);
	virtual err_code_t batch_write_short(const reg_address_t address, const std::vector<short> & val);
	virtual err_code_t batch_write_ushort(const reg_address_t address, const std::vector<ushort> & val);
	virtual err_code_t batch_write_int32(const reg_address_t address, const std::vector<int32> & val);
	virtual err_code_t batch_write_uint32(const reg_address_t address, const std::vector<uint32> & val);
	virtual err_code_t batch_write_int64(const reg_address_t address, const std::vector<int64> & val);
	virtual err_code_t batch_write_uint64(const reg_address_t address, const std::vector<uint64> & val);
	virtual err_code_t batch_write_float(const reg_address_t address, const std::vector<float> & val);
	virtual err_code_t batch_write_double(const reg_address_t address, const std::vector<double> & val);
};

class S7Protocol: public PLCProtocol{
public:
	siemens_plc_types_e plcType;
	S7Protocol(){};
	S7Protocol(const std::string & ipaddr, int port, siemens_plc_types_e plc_tyle);
	bool connect(const std::string & ipaddr, int port, siemens_plc_types_e plc_type);
	virtual ~S7Protocol();
	virtual	bool connect();
	virtual bool connect(const std::string & ipaddr, int port);
	virtual bool disconnect();
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

	//batch read
	virtual err_code_t batch_read_bool(const reg_address_t address, std::vector<bool> & val, size_t size) const;
	virtual err_code_t batch_read_byte(const reg_address_t address, std::vector<byte> & val, size_t size) const;
	virtual err_code_t batch_read_short(const reg_address_t address, std::vector<short> & val, size_t size) const;
	virtual err_code_t batch_read_ushort(const reg_address_t address, std::vector<ushort> & val, size_t size) const;
	virtual err_code_t batch_read_int32(const reg_address_t address, std::vector<int32> & val, size_t size) const;
	virtual err_code_t batch_read_uint32(const reg_address_t address, std::vector<uint32> & val, size_t size) const;
	virtual err_code_t batch_read_int64(const reg_address_t address, std::vector<int64> & val, size_t size) const;
	virtual err_code_t batch_read_uint64(const reg_address_t address, std::vector<uint64> & val, size_t size) const;
	virtual err_code_t batch_read_float(const reg_address_t address, std::vector<float> & val, size_t size) const;
	virtual err_code_t batch_read_double(const reg_address_t address, std::vector<double> & val, size_t size) const;

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

	//batch write
	virtual err_code_t batch_write_bool(const reg_address_t address, const std::vector<bool> & val);
	virtual err_code_t batch_write_byte(const reg_address_t address, const std::vector<byte> & val);
	virtual err_code_t batch_write_short(const reg_address_t address, const std::vector<short> & val);
	virtual err_code_t batch_write_ushort(const reg_address_t address, const std::vector<ushort> & val);
	virtual err_code_t batch_write_int32(const reg_address_t address, const std::vector<int32> & val);
	virtual err_code_t batch_write_uint32(const reg_address_t address, const std::vector<uint32> & val);
	virtual err_code_t batch_write_int64(const reg_address_t address, const std::vector<int64> & val);
	virtual err_code_t batch_write_uint64(const reg_address_t address, const std::vector<uint64> & val);
	virtual err_code_t batch_write_float(const reg_address_t address, const std::vector<float> & val);
	virtual err_code_t batch_write_double(const reg_address_t address, const std::vector<double> & val);
};
#ifdef CONFIG_PROTOCOL_MODBUS
typedef enum{
	MODBUS_ADDR_TYPE_COIL,
	MODBUS_ADDR_TYPE_POINT,
	MODBUS_ADDR_TYPE_INPUT,
	MODBUS_ADDR_TYPE_OUTPUT,
	MODBUS_ADDR_TYPE_INVALID
}ModBusRegisterType;

class ModBusProtocolImpl : public PLCProtocol{
protected:
	modbus_t* modbusContext_;
	int slaveNum_;
	int timeoutSec_ = 500000;
	int timeoutUSec_ = 500000;
public:
	typedef uint32_t ModBusAddressType;

	ModBusProtocolImpl(){};
	virtual ~ModBusProtocolImpl(){};
	virtual	bool connect() = 0;
	virtual bool disconnect() = 0;

	void setModbusTimeout(int sec, int usec);
	ModBusRegisterType getModbusRegisterTypeByAddress(const ModBusAddressType adress) const;
	bool chechModbusAddress(const std::string & addressStr, size_t length) const;

	//read
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

	//batch read
	virtual err_code_t batch_read_bool(const reg_address_t address, std::vector<bool> & val, size_t size) const;
	virtual err_code_t batch_read_byte(const reg_address_t address, std::vector<byte> & val, size_t size) const;
	virtual err_code_t batch_read_short(const reg_address_t address, std::vector<short> & val, size_t size) const;
	virtual err_code_t batch_read_ushort(const reg_address_t address, std::vector<ushort> & val, size_t size) const;
	virtual err_code_t batch_read_int32(const reg_address_t address, std::vector<int32> & val, size_t size) const;
	virtual err_code_t batch_read_uint32(const reg_address_t address, std::vector<uint32> & val, size_t size) const;
	virtual err_code_t batch_read_int64(const reg_address_t address, std::vector<int64> & val, size_t size) const;
	virtual err_code_t batch_read_uint64(const reg_address_t address, std::vector<uint64> & val, size_t size) const;
	virtual err_code_t batch_read_float(const reg_address_t address, std::vector<float> & val, size_t size) const;
	virtual err_code_t batch_read_double(const reg_address_t address, std::vector<double> & val, size_t size) const;

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

	//batch write
	virtual err_code_t batch_write_bool(const reg_address_t address, const std::vector<bool> & val);
	virtual err_code_t batch_write_byte(const reg_address_t address, const std::vector<byte> & val);
	virtual err_code_t batch_write_short(const reg_address_t address, const std::vector<short> & val);
	virtual err_code_t batch_write_ushort(const reg_address_t address, const std::vector<ushort> & val);
	virtual err_code_t batch_write_int32(const reg_address_t address, const std::vector<int32> & val);
	virtual err_code_t batch_write_uint32(const reg_address_t address, const std::vector<uint32> & val);
	virtual err_code_t batch_write_int64(const reg_address_t address, const std::vector<int64> & val);
	virtual err_code_t batch_write_uint64(const reg_address_t address, const std::vector<uint64> & val);
	virtual err_code_t batch_write_float(const reg_address_t address, const std::vector<float> & val);
	virtual err_code_t batch_write_double(const reg_address_t address, const std::vector<double> & val);
};

class ModBusTCPProtocol : public ModBusProtocolImpl{
public:
	ModBusTCPProtocol(){};
	ModBusTCPProtocol(const std::string & ip, int port, int slaveNum);
	virtual ~ModBusTCPProtocol();

	virtual	bool connect();
	virtual bool connect(const std::string & ipaddr, int port, int slave);
	virtual bool disconnect();
};

class ModBusRTUProtocol : public ModBusProtocolImpl{
private:
	std::string deviceName_;
	int bautRate_;
	char parity_;
	int dataBits_;
	int stopBits_;
public:
	ModBusRTUProtocol(){};
	ModBusRTUProtocol(const std::string & deviceName, 
						int bautRate, char parity,
						int dataBits, int stopBits,
						int slave);

	virtual	bool connect();
	virtual bool connect(const std::string & deviceName, 
						int bautRate, char parity,
						int dataBits, int stopBits,
						int slave);
	virtual bool disconnect();
};
#endif

#endif