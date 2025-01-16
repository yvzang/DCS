#ifndef __MELSEC_MC_UTILL_H__
#define __MELSEC_MC_UTILL_H__

#include "melsec_mc_typedef.h"

typedef struct _tag_plc_network_address {
	byte	network_number; // 网络号，通常为0;依据PLC的配置而配置
	byte	station_number; // 网络站号，通常为0;依据PLC的配置而配置
}plc_network_address;

typedef struct _tag_byte_array_info {
	byte* data;	// 内容
	int length; // 长度
}byte_array_info;

typedef struct _tag_bool_array_info {
	bool* data;	// 内容
	int length; // 长度
}bool_array_info;

void mshort2bytes(short i, byte* bytes);
short mbytes2short(byte* bytes);

void mushort2bytes(ushort i, byte* bytes);
ushort mbytes2ushort(byte* bytes);

void mint2bytes(int32 i, byte* bytes);
int32 mbytes2int32(byte* bytes);

void muint2bytes(uint32 i, byte* bytes);
uint32 mbytes2uint32(byte* bytes);

void mbigInt2bytes(int64 i, byte* bytes);
int64 mbytes2bigInt(byte* bytes);

void mubigInt2bytes(uint64 i, byte* bytes);
uint64 mbytes2ubigInt(byte* bytes);

void mfloat2bytes(float i, byte* bytes);
float mbytes2float(byte* bytes);

void mdouble2bytes(double i, byte* bytes);
double mbytes2double(byte* bytes);

#ifndef _WIN32
char* mitoa(unsigned long long  value, char str[], int radix);
#endif // !_WIN32

#endif
