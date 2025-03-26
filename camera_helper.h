#ifndef __CAMERA_HELPER__
#define __CAMERA_HELPER__
#include <string>
#include <functional>
#include <vector>
#include "lvgl/lvgl.h"
extern "C"{
#include "demoVdecoder.h"
}

typedef enum{
	CAMERA_STATE_NO,
	CAMERA_STATE_LOADING,
	CAMERA_STATE_PLAYING
} CameraState;

typedef struct _camera_info{
	std::string name;
	std::string account;
	std::string pwd;
	std::string ip_addr;
	int port;
	bool camera_info_read(const std::string path);
	bool camera_info_write(const std::string & path);
} camera_info;

typedef struct _camera_param{
	camera_info camera1_info;
	camera_info camera2_info;
	bool camera_param_read(const std::string & path);
	bool camera_param_write(const std::string & path);
}camera_param;

class RTSPAddrGen{
private:
	std::vector<std::string> address_pool;
public:
	typedef std::vector<std::string>::iterator rtsp_iterator;
	RTSPAddrGen() = delete;
	RTSPAddrGen(const camera_info & info);
	~RTSPAddrGen(){};
	rtsp_iterator begin();
	rtsp_iterator end();
	std::string operator()(const std::string & account, 
						const std::string & pwd, 
						const std::string & ip_addr,
						int port);
};

class CameraConnectionCallbackABS{
public:
	virtual void on_connecting() = 0;
	virtual void on_success() = 0;
	virtual void on_failed() = 0;
};

class Camera{
private:
	int state;
	lv_obj_t* _showDomain;
	lv_img_dsc_t image_dsc;
	std::string rtsp_uri;
	pthread_mutex_t _lock;
	pthread_t _conn_pid;
	void* _pDecoder = NULL;
	bool running;
public:
	CameraState _cameraState;
	camera_info info;
	CameraConnectionCallbackABS* pConnectionCallback_;
	Camera() = delete;
	Camera(lv_obj_t* show_domain, const std::string & name);
	bool probe();
	void camera_start();
	void camera_stop();
	void camera_connect_async();
	void camera_connect_async(CameraConnectionCallbackABS & callback);
	void operator()(const std::string & account, 
						const std::string & pwd, 
						const std::string & ip_addr,
						int port);
	bool is_running(){return running;};
private:
	static void* camera_connect_thread(void* args);
};

class CameraManager{
public:
	Camera camera_top;
	Camera camera_bottom;
	CameraManager() = delete;
	CameraManager(lv_obj_t* top_domain, lv_obj_t* bottom_domain);

};

#endif