#include <map>
#include "plc.h"
#include "utils.h"
#include "cJSON.h"
#include "limlog.h"

#define PLC_PARAM_JSON_IPADDR_KEY				"ipaddr"
#define PLC_PARAM_JSON_PORT_KEY					"port"


bool plc_info_json_parse_one(PLCManager & info, cJSON* plccfg){
	if(plccfg == NULL || 
		!cJSON_HasObjectItem(plccfg, PLC_PARAM_JSON_IPADDR_KEY) ||
		!cJSON_HasObjectItem(plccfg, PLC_PARAM_JSON_PORT_KEY))
		return false;
	info.ipaddr.assign(cJSON_GetObjectItem(plccfg, PLC_PARAM_JSON_IPADDR_KEY)->valuestring);
	info.port = cJSON_GetObjectItem(plccfg, PLC_PARAM_JSON_PORT_KEY)->valueint;
	return true;
}

bool plc_info_json_print_one(PLCManager & info, cJSON* plccfg){
	cJSON_AddStringToObject(plccfg, PLC_PARAM_JSON_IPADDR_KEY, info.ipaddr.c_str());
	cJSON_AddNumberToObject(plccfg, PLC_PARAM_JSON_PORT_KEY, info.port);
	return true;
}

/*====================PLCManager======================*/

#define PLC_RW_ERROR(ret)						{if(ret!=0)break;}

void* PLCManager::plc_worker_thread(void* args){
	PLCManager* manager = reinterpret_cast<PLCManager*>(args);
	while(1){
		if(manager->is_connected() == false){
			sleep(1);
			continue;
		}
		pthread_mutex_lock(&manager->rw_lock);
		clock_t start = clock();
		int ret = 0;
		for(auto ptr = manager->_tasks.begin(); 
			ptr != manager->_tasks.end(); ptr++){
			if(ptr->is_read){
				if(ptr->reg_type == REG_TYPE_BIT){
					bool read_value = 0;
					ret = manager->read_bool(ptr->address, &read_value);
					PLC_RW_ERROR(ret);
					PLCResult read_res;
					read_res.id = ptr->id;
					read_res.value.bval = read_value;
					read_res.is_read = true;
					manager->_results.push_back(read_res);
				}
				else if(ptr->reg_type == REG_TYPE_FLOAT){
					float read_value = 0;
					ret = manager->read_float(ptr->address, &read_value);
					PLC_RW_ERROR(ret);
					PLCResult read_res;
					read_res.id = ptr->id;
					read_res.value.fval = read_value;
					read_res.is_read = true;
					manager->_results.push_back(read_res);
				}
				else if(ptr->reg_type == REG_TYPE_UINT16){
					uint16_t read_value = 0;
					ret = manager->read_ushort(ptr->address, &read_value);
					PLC_RW_ERROR(ret);
					PLCResult read_res;
					read_res.id = ptr->id;
					read_res.value.u16val = read_value;
					read_res.is_read = true;
					manager->_results.push_back(read_res);
				}
				else if(ptr->reg_type == REG_TYPE_INT32){
					int32_t read_value = 0;
					ret = manager->read_int32(ptr->address, &read_value);
					PLC_RW_ERROR(ret);
					PLCResult read_res;
					read_res.id = ptr->id;
					read_res.value.i32val = read_value;
					read_res.is_read = true;
					manager->_results.push_back(read_res);
				}
				else if(ptr->reg_type == REG_TYPE_UINT32){
					uint32_t read_value = 0;
					ret = manager->read_uint32(ptr->address, &read_value);
					PLC_RW_ERROR(ret);
					PLCResult read_res;
					read_res.id = ptr->id;
					read_res.value.u32val = read_value;
					read_res.is_read = true;
					manager->_results.push_back(read_res);
				}
			}
			else if(ptr->is_read == false){
				if(ptr->reg_type == REG_TYPE_BIT){
					ret = manager->write_bool(ptr->address, ptr->value.bval);
					LOG_DEBUG << "write " << ptr->address <<
								" value: " << ptr->value.bval;
					PLC_RW_ERROR(ret);
					PLCResult read_res;
					read_res.id = ptr->id;
					read_res.is_read = false;
					manager->_results.push_back(read_res);
				}
				else if(ptr->reg_type == REG_TYPE_FLOAT){
					ret = manager->write_float(ptr->address, ptr->value.fval);
					LOG_DEBUG << "write " << ptr->address;
					PLC_RW_ERROR(ret);
					PLCResult read_res;
					read_res.id = ptr->id;
					read_res.is_read = false;
					manager->_results.push_back(read_res);
				}
				else if(ptr->reg_type == REG_TYPE_UINT16){
					ret = manager->write_ushort(ptr->address, ptr->value.u16val);
					PLC_RW_ERROR(ret);
					PLCResult read_res;
					read_res.id = ptr->id;
					read_res.is_read = false;
					manager->_results.push_back(read_res);
				}
				else if(ptr->reg_type == REG_TYPE_INT32){
					ret = manager->write_int32(ptr->address, ptr->value.i32val);
					PLC_RW_ERROR(ret);
					PLCResult read_res;
					read_res.id = ptr->id;
					read_res.is_read = false;
					manager->_results.push_back(read_res);
				}
				else if(ptr->reg_type == REG_TYPE_UINT32){
					ret = manager->write_uint32(ptr->address, ptr->value.u32val);
					PLC_RW_ERROR(ret);
					PLCResult read_res;
					read_res.id = ptr->id;
					read_res.is_read = false;
					manager->_results.push_back(read_res);
				}
			}
		}
		if(ret != 0){
			LOG_ERROR << "PLC Task error. Ret = " << ret <<
			", " << strerror(errno);
			manager->_is_connected = false;
			if(manager->_callback.connect_faild_cb){
				manager->_callback.connect_faild_cb(manager->_callback.data);
			}
		}
		manager->_tasks.clear();
		clock_t end = clock();
		//LOG_INFO << "Time of plc transfer: " << (double)(end-start)/CLOCKS_PER_SEC;
		pthread_mutex_unlock(&manager->rw_lock);
		pthread_cond_signal(&manager->rw_cond);
		usleep(1000);
	}
}

PLCManager::PLCManager(){
	read_config();
	pthread_create(&task_handler_pid, NULL, plc_worker_thread, this);
}

PLCManager::PLCManager(const std::string & ipaddr, int port)
:MCProtocol(ipaddr, port)
{
	pthread_create(&task_handler_pid, NULL, plc_worker_thread, this);
}

bool PLCManager::read_config(const std::string & path){
	std::string readstr;
	cJSON* jsoncfg;
	int ret = file_read(PLC_CONFIG_PATH, readstr);
	if(!ret){
		return false;
	}
	printf("read file: %s\n", readstr.c_str());
	jsoncfg = cJSON_Parse(readstr.c_str());
	if(jsoncfg == NULL){
		return false;
	}
	else{
		if(!plc_info_json_parse_one(*this, jsoncfg)){
			cJSON_Delete(jsoncfg);
			return false;
		}
	}
	cJSON_Delete(jsoncfg);
	return true;
}

bool PLCManager::write_config(const std::string & path){
	cJSON* plccfg = cJSON_CreateObject();
	plc_info_json_print_one(*this, plccfg);
	std::string jsonstr(cJSON_Print(plccfg));
	LOG_DEBUG << "write " << PLC_CONFIG_PATH << ": " << jsonstr;

	file_write(PLC_CONFIG_PATH, jsonstr);
	cJSON_Delete(plccfg);
	return true;
}

bool PLCManager::connect(const std::string & ipaddr, int port){
	_sock_fd = mc_connect((char*)ipaddr.c_str(), port, 0, 0);
	if(_sock_fd < 0){
		printf("mc protocol connection error.\n%s\n", strerror(errno));
		if(_callback.connect_faild_cb){
			_callback.connect_faild_cb(_callback.data);
		}
		_is_connected = false;
		return false;
	}
	_is_connected = true;
	if(_callback.connect_success_cb){
		_callback.connect_success_cb(_callback.data);
	}
	return true;
}

bool PLCManager::connect(){
	return this->connect(this->ipaddr, this->port);
}