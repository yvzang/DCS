#include "helper.h"
#include "utils.h"
#include "limlog.h"

#define CONFIG_PLC_ADDRESS_FILE						"/etc/plc.conf"

WifiManager* WifiManagerGetInstance(){
	static WifiManager manager("wlan0");
	return &manager;
}

void readPLCConfigFile(std::string & plcIP)
{
	std::string readstr;
	cJSON* jsoncfg;
	int ret = file_read(CONFIG_PLC_ADDRESS_FILE, readstr);
	if(!ret){
		return;
	}
	printf("read file: %s\n", readstr.c_str());
	jsoncfg = cJSON_Parse(readstr.c_str());
	if(jsoncfg == NULL){
		return;
	}
	else{
		if(!cJSON_HasObjectItem(jsoncfg, "ipaddr")){
			cJSON_Delete(jsoncfg);
			return;
		}
		plcIP.assign(cJSON_GetObjectItem(jsoncfg, "ipaddr")->valuestring);
	}
	cJSON_Delete(jsoncfg);
}

void writePLCConfigFile(const std::string & ipaddr){
	cJSON* plccfg = cJSON_CreateObject();
	cJSON_AddStringToObject(plccfg, "ipaddr", ipaddr.c_str());
	std::string jsonstr(cJSON_Print(plccfg));
	LOG_DEBUG << "write " << CONFIG_PLC_ADDRESS_FILE << ": " << jsonstr;

	file_write(CONFIG_PLC_ADDRESS_FILE, jsonstr);
	cJSON_Delete(plccfg);
}
