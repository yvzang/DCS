#ifndef __PLC_H__
#define __PLC_H__
#include <string>
#include <list>
#include <functional>
#include <pthread.h>
#include "plc_protocol.h"

#define PLC_CONFIG_PATH				"/etc/plc.conf"

typedef struct{
	std::string	ipaddr;
	int port;
	bool read(const std::string & path);
	bool write(const std::string & path);
} plc_info;

typedef enum{
    REG_TYPE_BIT,
    REG_TYPE_FLOAT,
    REG_TYPE_UINT16,
    REG_TYPE_INT32,
    REG_TYPE_UINT32
}reg_type_t;

typedef union _plc_value_t{
	bool bval;
	float fval;
	uint16_t u16val;
	int32 i32val;
	uint32_t u32val;
}plc_value_t;

typedef struct _PLCTask{
	bool is_read;
	plc_value_t value;
	reg_address_t address;
	uint32_t id;
	reg_type_t reg_type;
} PLCTask;

typedef struct _PLCResult{
	uint32_t id;
	bool is_read;
	plc_value_t value;
}PLCResult;


class PLCManager : public MCProtocol{
public:
	typedef struct{
		void (*connect_success_cb)(void*);
		void (*connect_faild_cb)(void*);
		void* data;
	} ConnectStateCallback_t;
	pthread_cond_t rw_cond;
private:
	ConnectStateCallback_t _callback;
	std::list<PLCTask> _tasks;
	std::list<PLCResult> _results;
	pthread_t task_handler_pid;
public:
	PLCManager();
	PLCManager(const std::string & ipaddr, int port);
	~PLCManager(){};
	bool read_config(const std::string & path = PLC_CONFIG_PATH);
	bool write_config(const std::string & path = PLC_CONFIG_PATH);
	ConnectStateCallback_t callback(){return _callback;};
	void set_callback(ConnectStateCallback_t cb){_callback = cb;};
	bool connect(const std::string & ipaddr, int port) override;
	bool connect() override;
	void add_task(PLCTask & task){_tasks.push_back(task);};
	std::list<PLCResult> & get_result(){return _results;};
private:
	static void* plc_worker_thread(void* args);
};


#endif