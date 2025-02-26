#ifndef	__H_MELSECMCNET_H__
#define __H_MELSECMCNET_H__

#include "melsec_mc_typedef.h"

int mc_connect(char* ip_addr, int port, byte network_addr, byte station_addr);
bool mc_disconnect(int fd);

//read
mc_error_code_e mc_read_bool(int fd, const char* address, bool* val);
mc_error_code_e mc_read_short(int fd, const char* address, short* val);
mc_error_code_e mc_read_ushort(int fd, const char* address, ushort* val);
mc_error_code_e mc_read_int32(int fd, const char* address, int32* val);
mc_error_code_e mc_read_uint32(int fd, const char* address, uint32* val);
mc_error_code_e mc_read_int64(int fd, const char* address, int64* val);
mc_error_code_e mc_read_uint64(int fd, const char* address, uint64* val);
mc_error_code_e mc_read_float(int fd, const char* address, float* val);
mc_error_code_e mc_read_double(int fd, const char* address, double* val);
mc_error_code_e mc_read_string(int fd, const char* address, int length, char** val); //need free val

//batch read
mc_error_code_e mc_batch_read_bool(int fd, const char* address, bool* vals, int size);
mc_error_code_e mc_batch_read_short(int fd, const char* address, short* vals, int size);
mc_error_code_e mc_batch_read_ushort(int fd, const char* address, ushort* vals, int size);
mc_error_code_e mc_batch_read_int32(int fd, const char* address, int32* vals, int size);
mc_error_code_e mc_batch_read_uint32(int fd, const char* address, uint32* vals, int size); 
mc_error_code_e mc_batch_read_int64(int fd, const char* address, int64* vals, int size);
mc_error_code_e mc_batch_read_uint64(int fd, const char* address, uint64* vals, int size);
mc_error_code_e mc_batch_read_float(int fd, const char* address, float* vals, int size);
mc_error_code_e mc_batch_read_double(int fd, const char* address, double* vals, int size);

//write
mc_error_code_e mc_write_bool(int fd, const char* address, bool val);
mc_error_code_e mc_write_short(int fd, const char* address, short val);
mc_error_code_e mc_write_ushort(int fd, const char* address, ushort val);
mc_error_code_e mc_write_int32(int fd, const char* address, int32 val);
mc_error_code_e mc_write_uint32(int fd, const char* address, uint32 val);
mc_error_code_e mc_write_int64(int fd, const char* address, int64 val);
mc_error_code_e mc_write_uint64(int fd, const char* address, uint64 val);
mc_error_code_e mc_write_float(int fd, const char* address, float val);
mc_error_code_e mc_write_double(int fd, const char* address, double val);
mc_error_code_e mc_write_string(int fd, const char* address, int length, const char* val);

//batch write
mc_error_code_e mc_batch_write_bool(int fd, const char* address, const bool* val, int size);
mc_error_code_e mc_batch_write_short(int fd, const char* address, const short* val, int size);
mc_error_code_e mc_batch_write_ushort(int fd, const char* address, const ushort* val, int size);
mc_error_code_e mc_batch_write_int32(int fd, const char* address, const int32* val, int size);
mc_error_code_e mc_batch_write_uint32(int fd, const char* address, const uint32* val, int size);
mc_error_code_e mc_batch_write_int64(int fd, const char* address, const int64* val, int size);
mc_error_code_e mc_batch_write_uint64(int fd, const char* address, const uint64* val, int size);
mc_error_code_e mc_batch_write_float(int fd, const char* address, const float* val, int size);
mc_error_code_e mc_batch_write_double(int fd, const char* address, const double* val, int size);

//
mc_error_code_e mc_remote_run(int fd);
mc_error_code_e mc_remote_stop(int fd);
mc_error_code_e mc_remote_reset(int fd);
mc_error_code_e mc_read_plc_type(int fd, char** type); //need free type 

#endif //__H_MELSECMCNET_H__