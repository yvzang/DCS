#include "melsec_mc_utill.h"
#include <stdlib.h>
#include <string.h>

void mshort2bytes(short i, byte* bytes)
{
	int size = 2;
	memset(bytes, 0, sizeof(byte) * size);
	bytes[0] = (byte)(0xff & i);
	bytes[1] = (byte)((0xff00 & i) >> 8);
}

short mbytes2short(byte* bytes)
{
	short iRetVal = bytes[0] & 0xFF;
	iRetVal |= (((short)bytes[1] << 8) & 0xFF00);
	return iRetVal;
}

void mushort2bytes(ushort i, byte* bytes) {
	int size = 2;
	memset(bytes, 0, sizeof(byte) * size);
	bytes[0] = (byte)(0xff & i);
	bytes[1] = (byte)((0xff00 & i) >> 8);
}

ushort mbytes2ushort(byte* bytes) {
	ushort iRetVal = bytes[0] & 0xFF;
	iRetVal |= (((ushort)bytes[1] << 8) & 0xFF00);
	return iRetVal;
}

void mint2bytes(int32 i, byte* bytes) {
	int size = 4;
	memset(bytes, 0, sizeof(byte) * size);
	bytes[0] = (byte)(0xff & i);
	bytes[1] = (byte)((0xff00 & i) >> 8);
	bytes[2] = (byte)((0xff0000 & i) >> 16);
	bytes[3] = (byte)((0xff000000 & i) >> 24);
}

int32 mbytes2int32(byte* bytes) {
	int32 iRetVal = bytes[0] & 0xFF;
	iRetVal |= (((int32)bytes[1] << 8) & 0xFF00);
	iRetVal |= (((int32)bytes[2] << 16) & 0xFF0000);
	iRetVal |= (((int32)bytes[3] << 24) & 0xFF000000);
	return iRetVal;
}

void muint2bytes(uint32 i, byte* bytes) {
	int size = 4;
	memset(bytes, 0, sizeof(byte) * size);
	bytes[0] = (byte)(0xff & i);
	bytes[1] = (byte)((0xff00 & i) >> 8);
	bytes[2] = (byte)((0xff0000 & i) >> 16);
	bytes[3] = (byte)((0xff000000 & i) >> 24);
}

uint32 mbytes2uint32(byte* bytes) {
	uint32 iRetVal = bytes[0] & 0xFF;
	iRetVal |= (((uint32)bytes[1] << 8) & 0xFF00);
	iRetVal |= (((uint32)bytes[2] << 16) & 0xFF0000);
	iRetVal |= (((uint32)bytes[3] << 24) & 0xFF000000);
	return iRetVal;
}

void mbigInt2bytes(int64 i, byte* bytes)
{
	int size = 8;
	memset(bytes, 0, sizeof(byte) * size);
	bytes[0] = (byte)(0xff & i);
	bytes[1] = (byte)(0xff & (i >> 8));
	bytes[2] = (byte)(0xff & (i >> 16));
	bytes[3] = (byte)(0xff & (i >> 24));
	bytes[4] = (byte)(0xff & (i >> 32));
	bytes[5] = (byte)(0xff & (i >> 40));
	bytes[6] = (byte)(0xff & (i >> 48));
	bytes[7] = (byte)(0xff & (i >> 56));
}

int64 mbytes2bigInt(byte* bytes)
{
	int64 iRetVal = bytes[0] & 0xFF;
	iRetVal |= (((int64)bytes[1] << 8) & 0xFF00);
	iRetVal |= (((int64)bytes[2] << 16) & 0xFF0000);
	iRetVal |= (((int64)bytes[3] << 24) & 0xFF000000);
	iRetVal |= (((int64)bytes[4] << 32) & 0xFF00000000);
	iRetVal |= (((int64)bytes[5] << 40) & 0xFF0000000000);
	iRetVal |= (((int64)bytes[6] << 48) & 0xFF000000000000);
	iRetVal |= (((int64)bytes[7] << 56) & 0xFF00000000000000);
	return iRetVal;
}

void mubigInt2bytes(uint64 i, byte* bytes)
{
	int size = 8;
	memset(bytes, 0, sizeof(byte) * size);
	bytes[0] = (byte)(0xff & i);
	bytes[1] = (byte)(0xff & (i >> 8));
	bytes[2] = (byte)(0xff & (i >> 16));
	bytes[3] = (byte)(0xff & (i >> 24));
	bytes[4] = (byte)(0xff & (i >> 32));
	bytes[5] = (byte)(0xff & (i >> 40));
	bytes[6] = (byte)(0xff & (i >> 48));
	bytes[7] = (byte)(0xff & (i >> 56));
}

uint64 mbytes2ubigInt(byte* bytes)
{
	uint64 iRetVal = bytes[0] & 0xFF;
	iRetVal |= (((uint64)bytes[1] << 8) & 0xFF00);
	iRetVal |= (((uint64)bytes[2] << 16) & 0xFF0000);
	iRetVal |= (((uint64)bytes[3] << 24) & 0xFF000000);
	iRetVal |= (((uint64)bytes[4] << 32) & 0xFF00000000);
	iRetVal |= (((uint64)bytes[5] << 40) & 0xFF0000000000);
	iRetVal |= (((uint64)bytes[6] << 48) & 0xFF000000000000);
	iRetVal |= (((uint64)bytes[7] << 56) & 0xFF00000000000000);
	return iRetVal;
}

void mfloat2bytes(float i, byte* bytes)
{
	int size = 4;
	int temp = *(int*)&i;
	mint2bytes(temp, bytes);
}

float mbytes2float(byte* bytes)
{
	int temp = mbytes2int32(bytes);
	return *(float*)&temp;
}

void mdouble2bytes(double i, byte* bytes)
{
	int64 temp = *(int64*)&i;
	mbigInt2bytes(temp, bytes);
}

double mbytes2double(byte* bytes)
{
	int64 temp = mbytes2bigInt(bytes);
	return *((double*)&temp);
}

#ifndef _WIN32
/*
=============
itoa

Convert integer to string

PARAMS:
- value     A 64-bit number to convert
- str       Destination buffer; should be 66 characters long for radix2, 24 - radix8, 22 - radix10, 18 - radix16.
- radix     Radix must be in range -36 .. 36. Negative values used for signed numbers.
=============
*/

char* mitoa(unsigned long long  value, char str[], int radix)
{
	char        buf[66];
	char* dest = buf + sizeof(buf);
	bool     sign = false;

	if (value == 0) {
		memcpy(str, "0", 2);
		return str;
	}

	if (radix < 0) {
		radix = -radix;
		if ((long long)value < 0) {
			value = -value;
			sign = true;
		}
	}

	*--dest = '\0';

	switch (radix)
	{
	case 16:
		while (value) {
			*--dest = '0' + (value & 0xF);
			if (*dest > '9') *dest += 'A' - '9' - 1;
			value >>= 4;
		}
		break;
	case 10:
		while (value) {
			*--dest = '0' + (value % 10);
			value /= 10;
		}
		break;

	case 8:
		while (value) {
			*--dest = '0' + (value & 7);
			value >>= 3;
		}
		break;

	case 2:
		while (value) {
			*--dest = '0' + (value & 1);
			value >>= 1;
		}
		break;

	default:            // The slow version, but universal
		while (value) {
			*--dest = '0' + (value % radix);
			if (*dest > '9') *dest += 'A' - '9' - 1;
			value /= radix;
		}
		break;
	}

	if (sign) *--dest = '-';

	memcpy(str, dest, buf + sizeof(buf) - dest);
	return str;
}
#endif