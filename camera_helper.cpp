#include <stdio.h>
#include "camera_helper.h"
#include "Format.h"
#include "cJSON.h"
#include "limlog.h"
#include "utils.h"
extern "C" {
	#include "CdxParser.h"
}
#define CAMERA_PARAM_JSON_PATH					"/etc/camera.conf"

#define CAMERA_PARAM_JSON_CAMERA1_KEY			"camera1"
#define CAMERA_PARAM_JSON_CAMERA2_KEY			"camera2"
#define CAMERA_PARAM_JSON_ACNT_KEY				"account"
#define CAMERA_PARAM_JSON_PWD_KEY				"pwd"
#define CAMERA_PARAM_JSON_IP_KEY				"ip"
#define CAMERA_PARAM_JSON_PORT_KEY				"port"

/*std::vector<std::string> RTSPAddrGen::address_pool = {
	"rtsp://{0}:{1}@{2}:{3}/cam/realmonitor?channel=1&subtype=1",	//大华
	"rtsp://{0}:{1}@{2}:{3}/h265/ch1/sub/av_stream",				//海康
	"rtsp://{2}:{3}user={0}&password={1}&channel=1&stream=1.dsp?"	//雄迈
};*/

bool camera_info_json_parse_one(camera_info & info, cJSON* cameracfg){
	if(cameracfg == NULL ||
		!cJSON_HasObjectItem(cameracfg, CAMERA_PARAM_JSON_ACNT_KEY) ||
		!cJSON_HasObjectItem(cameracfg, CAMERA_PARAM_JSON_PWD_KEY) ||
		!cJSON_HasObjectItem(cameracfg, CAMERA_PARAM_JSON_IP_KEY) ||
		!cJSON_HasObjectItem(cameracfg, CAMERA_PARAM_JSON_PORT_KEY))
		return false;
	info.account.assign(cJSON_GetObjectItem(cameracfg, CAMERA_PARAM_JSON_ACNT_KEY)->valuestring);
	info.pwd.assign(cJSON_GetObjectItem(cameracfg, CAMERA_PARAM_JSON_PWD_KEY)->valuestring);
	info.ip_addr.assign(cJSON_GetObjectItem(cameracfg, CAMERA_PARAM_JSON_IP_KEY)->valuestring);
	info.port = cJSON_GetObjectItem(cameracfg, CAMERA_PARAM_JSON_PORT_KEY)->valueint;
	return true;
}

bool camera_info_json_print_one(const camera_info & info, cJSON* jsoncfg){
	cJSON_AddStringToObject(jsoncfg, CAMERA_PARAM_JSON_ACNT_KEY, info.account.c_str());
	cJSON_AddStringToObject(jsoncfg, CAMERA_PARAM_JSON_PWD_KEY, info.pwd.c_str());
	cJSON_AddStringToObject(jsoncfg, CAMERA_PARAM_JSON_IP_KEY, info.ip_addr.c_str());
	cJSON_AddNumberToObject(jsoncfg, CAMERA_PARAM_JSON_PORT_KEY, info.port);
	return true;
}

bool camera_info::camera_info_read(const std::string path){
	std::string readstr;
	cJSON* jsoncfg;
	int ret = file_read(CAMERA_PARAM_JSON_PATH, readstr);
	if(!ret){
		return false;
	}
	printf("read file: %s\n", readstr.c_str());
	jsoncfg = cJSON_Parse(readstr.c_str());
	if(jsoncfg == NULL){
		return false;
	}
	else{
		cJSON* cameracfg = cJSON_GetObjectItem(jsoncfg, name.c_str());
		if(cameracfg == NULL){
			cJSON_Delete(jsoncfg);
			return false;
		}
		else{
			if(!camera_info_json_parse_one(*this, cameracfg)){
				cJSON_Delete(jsoncfg);
				return false;
			}
		}
	}
	cJSON_Delete(jsoncfg);
	return true;
}

bool camera_info::camera_info_write(const std::string & path){
	cJSON* jsoncfg;
	std::string readstr;
	int ret = file_read(CAMERA_PARAM_JSON_PATH, readstr);
	if(!ret){
		jsoncfg = cJSON_CreateObject();
	}
	else{
		jsoncfg = cJSON_Parse(readstr.c_str());
		if(jsoncfg == NULL){
			jsoncfg = cJSON_CreateObject();
		}
	}
	if(cJSON_HasObjectItem(jsoncfg, name.c_str())){
		cJSON_DeleteItemFromObject(jsoncfg, name.c_str());
	}
	cJSON* cameracfg = cJSON_CreateObject();
	camera_info_json_print_one(*this, cameracfg);
	cJSON_AddItemToObject(jsoncfg, name.c_str(), cameracfg);
	std::string jsonstr(cJSON_Print(jsoncfg));
	LOG_DEBUG << "write camera.conf: " << jsonstr;

	file_write(CAMERA_PARAM_JSON_PATH, jsonstr);
	cJSON_Delete(jsoncfg);
	return true;
}

bool camera_param_json_parse_one(camera_info & info, cJSON* cameracfg){
	if(cameracfg == NULL ||
		!cJSON_HasObjectItem(cameracfg, CAMERA_PARAM_JSON_ACNT_KEY) ||
		!cJSON_HasObjectItem(cameracfg, CAMERA_PARAM_JSON_PWD_KEY) ||
		!cJSON_HasObjectItem(cameracfg, CAMERA_PARAM_JSON_IP_KEY) ||
		!cJSON_HasObjectItem(cameracfg, CAMERA_PARAM_JSON_PORT_KEY))
		return false;
	info.account.assign(cJSON_GetObjectItem(cameracfg, CAMERA_PARAM_JSON_ACNT_KEY)->valuestring);
	info.pwd.assign(cJSON_GetObjectItem(cameracfg, CAMERA_PARAM_JSON_PWD_KEY)->valuestring);
	info.ip_addr.assign(cJSON_GetObjectItem(cameracfg, CAMERA_PARAM_JSON_IP_KEY)->valuestring);
	info.port = cJSON_GetObjectItem(cameracfg, CAMERA_PARAM_JSON_PORT_KEY)->valueint;
	return true;
}

bool camera_param_json_parse(camera_param& param, cJSON* jsoncfg){
	cJSON* camera1_cfg = cJSON_GetObjectItem(jsoncfg, CAMERA_PARAM_JSON_CAMERA1_KEY);
	if(camera1_cfg){
		if(!camera_param_json_parse_one(param.camera1_info, camera1_cfg))
			return false;
	}
	cJSON* camera2_cfg = cJSON_GetObjectItem(jsoncfg, CAMERA_PARAM_JSON_CAMERA2_KEY);
	if(camera2_cfg){
		if(!camera_param_json_parse_one(param.camera2_info, camera2_cfg))
			return false;
	}
	return true;
}

bool camera_param_json_print_one(const camera_info & info, cJSON* jsoncfg){
	cJSON_AddStringToObject(jsoncfg, CAMERA_PARAM_JSON_ACNT_KEY, info.account.c_str());
	cJSON_AddStringToObject(jsoncfg, CAMERA_PARAM_JSON_PWD_KEY, info.pwd.c_str());
	cJSON_AddStringToObject(jsoncfg, CAMERA_PARAM_JSON_IP_KEY, info.ip_addr.c_str());
	cJSON_AddNumberToObject(jsoncfg, CAMERA_PARAM_JSON_PORT_KEY, info.port);
	return true;
}


bool camera_param_json_print(const camera_param & param, cJSON* res){
	cJSON* camera1_cfg = cJSON_CreateObject();
	cJSON* camera2_cfg = cJSON_CreateObject();
	cJSON_AddItemToObject(res, CAMERA_PARAM_JSON_CAMERA1_KEY, camera1_cfg);
	cJSON_AddItemToObject(res, CAMERA_PARAM_JSON_CAMERA2_KEY, camera2_cfg);
	if(!camera_param_json_print_one(param.camera1_info, camera1_cfg) ||
		!camera_param_json_print_one(param.camera2_info, camera2_cfg)){
		return false;
	}
	return true;
}

bool camera_param::camera_param_read(const std::string & path){
	int ret;
	FILE* fp = fopen(path.c_str(), "r");
	if(fp == NULL){
		return false;
	}
	else{
		char readbuf[1024]{0};
		ret = fread(readbuf, 1, sizeof(readbuf), fp);
		if(ret > 0){
			cJSON* json = cJSON_Parse(readbuf);
			if(json != NULL){
				ret = camera_param_json_parse(*this, json);
				if(ret){
					cJSON_Delete(json);
					fclose(fp);
					return ret;
				}
			}
			else{
				fclose(fp);
				return false;
			}
		}
		else{
			fclose(fp);
			return false;
		}
	}
	return ret;
}


bool camera_param::camera_param_write(const std::string & path){
	int ret;
	FILE* fp = fopen(path.c_str(), "w");
	cJSON* jsoncfg = cJSON_CreateObject();
	if(!camera_param_json_print(*this, jsoncfg)){
		std::string jsonstr(cJSON_Print(jsoncfg));
		fwrite(jsonstr.c_str(), 1, jsonstr.length(), fp);
		cJSON_Delete(jsoncfg);
		fclose(fp);
		return true;
	}
	cJSON_Delete(jsoncfg);
	fclose(fp);
	return false;
}

RTSPAddrGen::RTSPAddrGen(const camera_info & info)
{
	static std::vector<std::string> format_pool = {
		"rtsp://{0}:{1}@{2}:{3}/h265/ch1/sub/av_stream",				//海康
		"rtsp://{0}:{1}@{2}:{3}/cam/realmonitor?channel=1&subtype=1",	//大华
		"rtsp://{2}:{3}@user={0}&password={1}&channel=1&stream=1.dsp?"	//雄迈
	};
	std::stringstream ss;
	for(auto iter = format_pool.begin(); 
		iter != format_pool.end();
		iter++){
		std::string str = *iter;
		std::stringstream ss;
		str.replace(str.find("{0}"), 3, info.account);
		str.replace(str.find("{1}"), 3, info.pwd);
		str.replace(str.find("{2}"), 3, info.ip_addr);
		ss << info.port;
		str.replace(str.find("{3}"), 3, ss.str());
		address_pool.push_back(str);
	}
}

RTSPAddrGen::rtsp_iterator RTSPAddrGen::begin(){
	return address_pool.begin();
}

RTSPAddrGen::rtsp_iterator RTSPAddrGen::end(){
	return address_pool.end();
}

std::string RTSPAddrGen::operator()(const std::string & account, 
					const std::string & pwd, 
					const std::string & ip_addr,
					int port)
{
	
}

Camera::Camera(lv_obj_t* show_domain, const std::string & name)
:state(0), _showDomain(show_domain), _cameraState(CAMERA_STATE_NO)
{
	pthread_mutex_init(&_lock, NULL);
	info.name.assign(name);
	info.camera_info_read(CAMERA_PARAM_JSON_PATH);
}

bool Camera::probe(){
	RTSPAddrGen gen(info);
	CdxDataSourceT source{0};
	pthread_mutex_t parserMutex;
	CdxParserT *parser = NULL;
	CdxStreamT *stream = NULL;
	int bForceExit = 0;
	int nRet = -1;
	pthread_mutex_init(&parserMutex, NULL);
	for(auto iter = gen.begin(); 
		iter != gen.end(); iter++){
		std::string rtsp_addr = *iter;
		printf("rtsp uri: %s\n", rtsp_addr.c_str());
		source.uri = const_cast<char*>(rtsp_addr.c_str());
		nRet = CdxParserPrepare(&source, 0, &parserMutex,
            &bForceExit, &parser, &stream, NULL, NULL);
		if(nRet < 0 || parser == NULL){
			LOG_INFO << "camera probe failed.";
			continue;
		}
		else{
			this->rtsp_uri.assign(rtsp_addr);
			LOG_INFO << "camera probe success: " << rtsp_addr;
			break;
		}
	}
	if(parser != NULL){
		CdxParserClose(parser);
	}
	return (nRet < 0)?false:true;
}

void Camera::camera_connect_async(CameraConnectionCallbackABS & callback){
	pConnectionCallback_ = &callback;
	_cameraState = CAMERA_STATE_LOADING;
	pConnectionCallback_->on_connecting();
	pthread_create(&_conn_pid, NULL, Camera::camera_connect_thread, this);
	pthread_detach(_conn_pid);
}

void Camera::camera_connect_async(){
	_cameraState = CAMERA_STATE_LOADING;
	if(pConnectionCallback_)
		pConnectionCallback_->on_connecting();
	pthread_create(&_conn_pid, NULL, Camera::camera_connect_thread, this);
	pthread_detach(_conn_pid);
}

void* Camera::camera_connect_thread(void* args){
	int nRet = 0;
	Camera* camera = reinterpret_cast<Camera*>(args);
	if(camera->is_running()){
		camera->camera_stop();
	}
	usleep(10000);
	if(camera->probe()){
		camera->_cameraState = CAMERA_STATE_PLAYING;
		if(camera->pConnectionCallback_){
			camera->pConnectionCallback_->on_success();
			usleep(500);
		}
		camera->camera_start();
	}
	else{
		camera->_cameraState = CAMERA_STATE_NO;
		if(camera->pConnectionCallback_){
			camera->pConnectionCallback_->on_failed();
		}
	}
	pthread_exit(NULL);
}


void Camera::camera_start(){
	state = 0;
	decoder_start(&_pDecoder, rtsp_uri.c_str(), _showDomain, &image_dsc, &state);
	//decoder_start(&_pDecoder, "rtsp://admin:@192.168.1.11:554/h265/ch1/sub/av_stream", _showDomain, &image_dsc, &state);
	running = true;
}

void Camera::camera_stop(){
	if(_pDecoder == NULL) return;
	decoder_stop(_pDecoder, &state);
	//delete _pDecoder;
	running = false;
	_pDecoder = NULL;
}

void Camera::operator()(const std::string & account, 
						const std::string & pwd, 
						const std::string & ip_addr,
						int port)
{
	info.account.assign(account);
	info.pwd.assign(pwd);
	info.ip_addr.assign(ip_addr);
	info.port = port;
	if(!info.camera_info_write(CAMERA_PARAM_JSON_PATH)){
		LOG_ERROR << "configure of camera write error.";
	}
}