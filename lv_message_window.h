#ifndef __LV_MESSAGE_WINDOW_H__
#define __LV_MESSAGE_WINDOW_H__
#include <atomic>
#include "lvgl/lvgl.h"
#include "camera_helper.h"
#include "keyboard.h"
#include "network.h"
#include "task_manager.h"
#include "access.h"


class MessageWind{
private:
    lv_obj_t* msg_wind;
public:
    MessageWind();
    ~MessageWind();
    void create_wind(lv_obj_t* parent, const std::string & title,
                const std::string &msg);
    void create_without_btn_wind(lv_obj_t* parent, 
                                const std::string &msg);
    void create_error_wind(const std::string &msg);
    void create_info_wind(const std::string &msg);
    void delete_wind();
    static void msgbox_event_cb(lv_event_t* e);
};


class CameraSettingWind{
public:

private:
    lv_obj_t* ip_text;
    lv_obj_t* port_text;
    lv_obj_t* account_text;
    lv_obj_t* pwd_text;
    Camera* _pCamera;
    lv_obj_t* message_window;
    VirtualKeyboard* _keyboard;
public:
    CameraSettingWind() = delete;
    CameraSettingWind(Camera* camera);
	void hind();
	void show();
	static void btnmetrix_event_cb(lv_event_t* e);
	~CameraSettingWind();
};

typedef std::function<void(void)> WIFIConnectCallback;

class WIFISettingWind{
public:
    WifiManager::ConnectStateCallback_t wifi_connection_callback;
private:
    VirtualKeyboard* _keyboard;
    lv_obj_t* setting_window;
    lv_obj_t* _wifiList;
    lv_obj_t* wifi_pwd;
    WifiManager* _pWiFiManager;
    lv_obj_t* _status_img;
    Camera* _pLefgCamera;
    Camera* _pRightCamera;
    MessageWind _pMessageWind;

	std::atomic<bool> Stop_;
	std::thread ScanWIFIThread_;

	WIFIConnectCallback SuccessCallback_;
	WIFIConnectCallback FailCallback_;
public:
    WIFISettingWind(WIFIConnectCallback successCallback,
					WIFIConnectCallback failCallback,
					Camera* left_camera, Camera* right_camera);
	void hind();
	void show();
	void showWaitingWind(const std::string & msg);
	void destroyWaitingWind();
	static WifiManager* getWiFiManagerInstance();
	static void setting_btnmatrix_event_cb(lv_event_t* e);
private:
	void _scanWIFILoopFunc();
	void _connSuccessFunc();
	void _connFailFunc();
};

typedef std::function<void(void)> PLCConnectionCallback;

class LinkSettingWind{
private:
    VirtualKeyboard *_keyboard;
    lv_obj_t* _linking_window;
    lv_obj_t* plc_ip;
	PLCConnectionCallback SuccessCallback_;
	PLCConnectionCallback FailCallback_;

	std::string PLCDeviceName_;
	std::string PLCIP;
public:
	LinkSettingWind(PLCConnectionCallback successCallback,
					PLCConnectionCallback failCallback);
	void hind();
	void show();
	bool plc_connect();

private:
	void readPLCConfigFile(const std::string & path,
							std::string & plcIP);
	void writePLCConfigFile(const std::string & path);
	static void setting_btnmatrix_event_cb(lv_event_t* e);
};

class LoggingWind{
private:
    typedef struct{
        bool user_choice_win_del;
        lv_obj_t* user_choice_win_user_choise_win;
        lv_obj_t* user_choice_win_user_choise_win_item0;
        lv_obj_t* content_box;
        lv_obj_t* user_dropdown;
        lv_obj_t* password_textarea;
        lv_obj_t* btn_matrix;
    }lv_ui;
    lv_ui* ui;
    VirtualKeyboard* _pKeyboard;
    MessageWind _messageWind;
    AccessContrl* _pAccessContrl;
public:
    static LoggingWind* getInstance(){
        static LoggingWind wind;
        return &wind;
    }
    void show();
    void hide();
private:
    LoggingWind();
    ~LoggingWind();
	void event_init_screen();
    static void dropdown_event_cb(lv_event_t* e);
    static void close_event_cb(lv_event_t* e);
    static void btnmatrix_event_cb(lv_event_t* e);
};

class ModifyPasswordWind{
private:
    typedef struct {
        bool user_choice_win_del;
        lv_obj_t* user_choice_win_user_choise_win;
        lv_obj_t* user_choice_win_user_choise_win_item0;
        lv_obj_t* content_box;
        lv_obj_t* top_label;
        lv_obj_t* row_box_1;
        lv_obj_t* row_box_2;
        lv_obj_t* row_box_3;
        lv_obj_t* label_1;
        lv_obj_t* opassword_textarea;
        lv_obj_t* label_2;
        lv_obj_t* npassword_textarea;
        lv_obj_t* label_3;
        lv_obj_t* cpassword_textarea;
        lv_obj_t* btn_matrix;
    } lv_ui;
    lv_ui* ui;
    VirtualKeyboard* _pKeyboard;
    AccessContrl* _pAccessContrl;
    MessageWind _messageWind;

public:
    static ModifyPasswordWind* getInstance(){
        static ModifyPasswordWind wind;
        return &wind;
    } 
    void show();
    void hide();
private:
    ModifyPasswordWind();
	~ModifyPasswordWind();
	void event_init_screen();
    static void close_event_cb(lv_event_t* e);
    static void btnmatrix_event_cb(lv_event_t* e);
};

#endif