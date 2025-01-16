#ifndef __LV_MAIN_WIDGET_H__
#define __LV_MAIN_WIDGET_H__

#include "lvgl/lvgl.h"
#include "camera_helper.h"
#include "network.h"
#include "plc.h"
#include "keyboard.h"
#include "setting.h"
#include "access.h"

/**********************
 *      TYPEDEFS
 **********************/
typedef enum {
    DISP_SMALL,
    DISP_MEDIUM,
    DISP_LARGE,
} disp_size_t;


class Keyboard{
protected:
    lv_obj_t* _keyboard;
    lv_obj_t* _target;
    lv_obj_t* _parent_ant;
    lv_area_t wind_pre_area;
    Keyboard();
    ~Keyboard();
public:
    void show();
    void hind();
    static void keyboard_event_cb(lv_event_t* e);
    static Keyboard* getInstance();
};

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
    Keyboard* _keyboard;
public:
    CameraSettingWind() = delete;
    CameraSettingWind(Camera* camera);
	void hind();
	void show();
	static void btnmetrix_event_cb(lv_event_t* e);
	~CameraSettingWind();
};

class WIFISettingWind{
public:
    WifiManager::ConnectStateCallback_t wifi_connection_callback;
private:
    Keyboard* _keyboard;
    lv_obj_t* setting_window;
    lv_obj_t* _wifiList;
    lv_obj_t* wifi_pwd;
    WifiManager* _pWiFiManager;
    lv_obj_t* _status_img;
    Camera* _pLefgCamera;
    Camera* _pRightCamera;
    MessageWind _pMessageWind;
public:
    WIFISettingWind(Camera* left_camera, Camera* right_camera);
	void hind();
	void show();
	static WifiManager* getWiFiManagerInstance();
	static void setting_btnmatrix_event_cb(lv_event_t* e);
};

class LinkSettingWind{
private:
    Keyboard *_keyboard;
    lv_obj_t* _linking_window;
    PLCManager* _pPLCManager;
    lv_obj_t* plc_ip;
public:
	LinkSettingWind();
	void hind();
	void show();
	bool plc_connect();
	bool plc_reconnect();
	static PLCManager* getPLCManagerInstance();
	static void setting_btnmatrix_event_cb(lv_event_t* e);
};

class ParamKeyboard: public Keyboard{
protected:
    ParamKeyboard(){};
    ~ParamKeyboard(){};
public:
    static ParamKeyboard* getInstance();
    static void keyboard_event_cb(lv_event_t* e);
};

typedef struct _BoxValueItem{
    void* target;
    std::string address;
    reg_type_t reg_type;
    reg_type_t tar_type;
    std::function<void(void*, void*)> ifmt;
    std::function<void(void*, void*, void*)> ofmt;
    void* data;
} BoxValueItem;

struct WindTask: BoxValueItem{
    bool is_read;
    WindTask(const BoxValueItem & boxItem):BoxValueItem(boxItem){};
};

struct WindTaskQueue{
	pthread_mutex_t _task_lock;
	std::map<uint32_t, WindTask> queue;
	WindTaskQueue();
	void register_task(uint32_t id, const WindTask & task);
	void unregister_task(uint32_t id);
	void clear();
    std::map<uint32_t, WindTask>::iterator find(uint32_t id);
    std::map<uint32_t, WindTask>::iterator begin();
    std::map<uint32_t, WindTask>::iterator end();
};


class ParamSettingWind{
protected:
    PLCManager* _pPLCManager;
    ParamSettingWind();
    /*void write_float_value(lv_obj_t* text_domain, 
                        const std::string & addr, 
                        std::function<void(void*)> calculate_func);
    void write_uint32_value(lv_obj_t* text_domain, 
                        const std::string & addr,
                        std::function<void(void*)> calculate_func);
    void write_uint16_value(lv_obj_t* text_domain,
                        const std::string & addr,
                        std::function<void(void*)> calculate_func);
    */
public:
    ParamKeyboard* _keybaord;
    std::map<uint32_t, BoxValueItem> _tasks;
    virtual void read_value();
    void write_value(void* ui_domain);
};

class Param1SettingWind: public ParamSettingWind{
private:
    typedef struct
    {
        lv_obj_t* screen_fast_mv_spd;
        lv_obj_t* screen_slow_mv_psd;
        lv_obj_t* screen_fast_mv_spd_scl;
        lv_obj_t* screen_mv_len_scl;
        lv_obj_t* screen_single_mmt;
        lv_obj_t* screen_mv_alt_spd_1;
        lv_obj_t* screen_mv_alt_spd_2;
        lv_obj_t* screen_mv_tl_in_len;
        lv_obj_t* screen_mv_tl_ang;
        lv_obj_t* screen_param1_fast_up_spd;
        lv_obj_t* screen_param1_fast_down_spd;
        lv_obj_t* screen_param1_slow_ud_spd;
        lv_obj_t* screen;
        bool screen_del;
        lv_obj_t* screen_background;
        lv_obj_t* screen_background_tile_1;
        lv_obj_t* screen_param_title_label;
        lv_obj_t* screen_line_1;
        lv_obj_t* screen_tileview_1;
        lv_obj_t* screen_tileview_1_tile_2;
        lv_obj_t* screen_cont_1;
        lv_obj_t* screen_label_1;
        lv_obj_t* screen_label_2;
        lv_obj_t* screen_cont_2;
        lv_obj_t* screen_single_mmt_l;
        lv_obj_t* screen_cont_3;
        lv_obj_t* screen_slow_move_speed_;
        lv_obj_t* screen_label_5;
        lv_obj_t* screen_cont_4;
        lv_obj_t* screen_move_alt_spd_l;
        lv_obj_t* screen_label_7;
        lv_obj_t* screen_cont_5;
        lv_obj_t* screen_fast_move_l;
        lv_obj_t* screen_cont_6;
        lv_obj_t* screen_label_12;
        lv_obj_t* screen_label_11;
        lv_obj_t* screen_cont_7;
        lv_obj_t* screen_label_14;
        lv_obj_t* screen_cont_8;
        lv_obj_t* screen_label_16;
        lv_obj_t* screen_label_15;
        lv_obj_t* screen_label_17;
        lv_obj_t* screen_line_2;
        lv_obj_t* screen_tileview_2;
        lv_obj_t* screen_tileview_2_tile_2;
        lv_obj_t* screen_cont_15;
        lv_obj_t* screen_label_28;
        lv_obj_t* screen_label_31;
        lv_obj_t* screen_cont_14;
        lv_obj_t* screen_label_27;
        lv_obj_t* screen_label_26;
        lv_obj_t* screen_cont_12;
        lv_obj_t* screen_label_23;
        lv_obj_t* screen_label_29;
    }lv_ui;
    lv_ui* ui;
    
public:
    static Param1SettingWind* getInstance(lv_obj_t* parent);
	Param1SettingWind(lv_obj_t* parent);
    void set_disable(bool disable);
	void event_init_screen();
};

class Param2SettingWind: public ParamSettingWind{
private:
    typedef struct {
        lv_obj_t* screen_1;
        bool screen_1_del;
        lv_obj_t* screen_1_background;
        lv_obj_t* screen_1_background_tile_1;
        lv_obj_t* screen_1_label_55;
        lv_obj_t* screen_1_line_4;
        lv_obj_t* screen_1_tileview_4;
        lv_obj_t* screen_1_tileview_4_tile_2;
        lv_obj_t* screen_1_cont_29;
        lv_obj_t* screen_1_label_54;
        lv_obj_t* screen_1_ta_15;
        lv_obj_t* screen_1_label_53;
        lv_obj_t* screen_1_cont_27;
        lv_obj_t* screen_1_label_51;
        lv_obj_t* screen_1_ta_13;
        lv_obj_t* screen_1_label_50;
        lv_obj_t* screen_1_cont_26;
        lv_obj_t* screen_1_label_49;
        lv_obj_t* screen_1_ta_12;
        lv_obj_t* screen_1_label_48;
        lv_obj_t* screen_1_ta_11;
        lv_obj_t* screen_1_cont_30;
        lv_obj_t* screen_1_label_57;
        lv_obj_t* screen_1_ta_16;
        lv_obj_t* screen_1_label_56;
        lv_obj_t* screen_1_cont_31;
        lv_obj_t* screen_1_label_58;
        lv_obj_t* screen_1_ta_17;
        lv_obj_t* screen_1_cont_32;
        lv_obj_t* screen_1_label_60;
        lv_obj_t* screen_1_ta_18;
        lv_obj_t* screen_1_label_59;
        lv_obj_t* screen_1_cont_35;
        lv_obj_t* screen_1_label_66;
        lv_obj_t* screen_1_ta_21;
        lv_obj_t* screen_1_label_65;
        lv_obj_t* screen_1_cont_36;
        lv_obj_t* screen_1_label_68;
        lv_obj_t* screen_1_ta_22;
        lv_obj_t* screen_1_label_67;
        lv_obj_t* screen_1_cont_37;
        lv_obj_t* screen_1_label_70;
        lv_obj_t* screen_1_ta_23;
        lv_obj_t* screen_1_label_69;
        lv_obj_t* screen_1_cont_38;
        lv_obj_t* screen_1_label_72;
        lv_obj_t* screen_1_ta_24;
        lv_obj_t* screen_1_label_71;
        lv_obj_t* screen_1_label_41;
        lv_obj_t* screen_1_line_3;
        lv_obj_t* screen_1_tileview_3;
        lv_obj_t* screen_1_tileview_3_tile_2;
        lv_obj_t* screen_1_cont_20;
        lv_obj_t* screen_1_label_39;
        lv_obj_t* screen_1_ta_5;
        lv_obj_t* screen_1_label_38;
        lv_obj_t* screen_1_cont_19;
        lv_obj_t* screen_1_label_37;
        lv_obj_t* screen_1_ta_4;
        lv_obj_t* screen_1_label_36;
        lv_obj_t* screen_1_cont_17;
        lv_obj_t* screen_1_label_33;
        lv_obj_t* screen_1_ta_1;
        lv_obj_t* screen_1_label_32;
        lv_obj_t* screen_1_cont_39;
        lv_obj_t* screen_1_label_74;
        lv_obj_t* screen_1_ta_25;
        lv_obj_t* screen_1_label_73;
        lv_obj_t* screen_1_cont_40;
        lv_obj_t* screen_1_label_76;
        lv_obj_t* screen_1_ta_26;
        lv_obj_t* screen_1_label_75;
        lv_obj_t* screen_1_line_5;
        lv_obj_t* screen_1_label_77;
    }lv_ui;
    lv_ui* ui;
public:
    static Param2SettingWind* getInstance(lv_obj_t* parent);
	Param2SettingWind(lv_obj_t* parent);
    void set_disable(bool disable);
	void events_init_screen();
};

class ModelSettingWind: public ParamSettingWind{
private:
    typedef struct{
        lv_obj_t* screen_1_background_tile_3;
        lv_obj_t* screen_1_label_78;
        lv_obj_t* screen_1_label_79;
        lv_obj_t* screen_1_ddlist_1;
        lv_obj_t* screen_1_btn_1;
        lv_obj_t* screen_1_btn_1_label;
        lv_obj_t* screen_1_btn_2;
        lv_obj_t* screen_1_btn_2_label;
        lv_obj_t* screen_1_line_6;
        lv_obj_t* screen_1_label_80;
        lv_obj_t* screen_1_ta_27;
        lv_obj_t* screen_1_label_81;
        lv_obj_t* screen_1_ta_28;
        lv_obj_t* screen_1_ta_29;
        lv_obj_t* screen_1_line_7;
        lv_obj_t* screen_1_ta_30;
        lv_obj_t* screen_1_label_82;
        lv_obj_t* screen_1_label_83;
        lv_obj_t* screen_1_ta_31;
        lv_obj_t* screen_1_ta_32;
    }lv_ui;
    lv_ui* ui;
    static void btn_model_std_event_cb(lv_event_t* e);
    static void btn_model_menu_event_cb(lv_event_t* e);
public:
    typedef enum{
        MODEL_STD = 1,
        MODEL_MANU
    } model_t;
    model_t _cur_model;
    static ModelSettingWind* getInstance(lv_obj_t* parent);
	ModelSettingWind(lv_obj_t* parent);
	void event_init_screen();
    //virtual void read_value() override;
    void switch_model(model_t model);
	static void dropdown_event_cb(lv_event_t* e);
};


class WorkRecordWind{
public:
    struct WorkRecordItem{
        std::string timestamp;
        std::string arg1;
        std::string arg2;
        std::string arg3;
        std::string arg4;
        std::string arg5;
    };
private:
    typedef struct{
        int table_row;
        lv_obj_t* screen_1_background_tile_4;
        lv_obj_t* screen_1_label_84;
        lv_obj_t* screen_1_work_record;
        lv_obj_t* btnmx;
        lv_obj_t* button_widget;
    }lv_ui;
    lv_ui* ui;
    std::list<WorkRecordItem> _workRecordList;
    Database _pDatabase;
    static void btn_event_cb(lv_event_t* e);
public:
    static WorkRecordWind* getInstance(lv_obj_t* parent);
	WorkRecordWind(lv_obj_t* parent);
    void events_init_screen();
    bool insert_record(const std::string & timestamp, 
                        const std::string & arg1, 
                        const std::string & arg2, 
                        const std::string & arg3, 
                        const std::string & arg4, 
                        const std::string & arg5);
    bool load_first_table_record();
    bool load_next_table_record();
    bool load_previous_table_record();
    void update_table();
};

class FaultRecordWind{
public:
    typedef struct{
        std::string timestamp;
        std::string evenet;
    }FaultRecordItem;
private:
    typedef struct{
        lv_obj_t* screen_1_background_tile_5;
        lv_obj_t* screen_1_label_85;
        lv_obj_t* screen_1_fault_record;
        lv_obj_t* btnmx;
        lv_obj_t* button_widget;
        int table_row;
    } lv_ui;
    lv_ui* ui;
    std::list<FaultRecordItem> _faultRecordList;
    Database _pDatabase;
    static void btn_event_cb(lv_event_t* e);
public:
    static FaultRecordWind* getInstance(lv_obj_t* parent);
	FaultRecordWind(lv_obj_t* parent);
    void events_init_screen();
    bool insert_record(const std::string & timestamp, 
                        const std::string & event);
    bool load_first_table_record();
    bool load_next_table_record();
    bool load_previous_table_record();
    void update_table();
};

class ModifyPasswordWind;
class LoggingWind;
void ofmt_set_running_info(void* pValue, void* pTextarea, void* preg);
void ofmt_work_record(void* pValue, void* pWind, void* pReg);

class MainWind{
private:
    int8_t _tile_index = 0;
    pthread_t sys_clock_pid;
    pthread_t plc_trans_pid;
    lv_obj_t* _tile_wind;
    lv_obj_t* _pMainWind;
    WIFISettingWind* _wifi_setting_wind;
    LinkSettingWind* _link_setting_wind;
    lv_obj_t* _left_img_box;
    lv_obj_t* _right_img_box;
    lv_obj_t* left_img_domain;
    lv_obj_t* right_img_domain;
    lv_obj_t* _wifi_box;
    lv_obj_t* _link_box;
    lv_obj_t* top_label;
    lv_obj_t* top_datetime;
    lv_obj_t* battery_label;
    lv_obj_t* material_label;
    lv_obj_t* battery_img;
    lv_obj_t* current_angle;
    lv_obj_t* battery;
    lv_obj_t* running_info;
    Camera* camera1;
    Camera* camera2;
    CameraSettingWind *left_cameraSettingWind;
    CameraSettingWind *right_cameraSettingWind;
    PhyKeyboard* _pPhyKeyboard;
    Param1SettingWind* _pParam1Wind;
    Param2SettingWind* _pParam2Wind;
    ModelSettingWind* _pModelWind;
    WorkRecordWind* _pWorkRecordWind;
    FaultRecordWind* _pFultRecordWind;
    ModifyPasswordWind* _pModifyPasswordWind;
    LoggingWind* _pLoggingWind;
    PLCManager* _pPLCManager;
    WifiManager* _pWiFiManager;
    Identity* _pIdentity;
    lv_obj_t* status_image_ptr_list[3];
    lv_obj_t* row3_status_image_ptr_list[3];
    lv_obj_t* row0_ptr_list[3];
    lv_obj_t* row1_ptr_list[3];
    std::map<uint32_t, BoxValueItem> _read_tasks;

public:
    static MainWind* getInstance(){
        static MainWind mainwind;
        return &mainwind;
    }

private:
    static void mainwindKeyPress_cb(void* pWind);
    static void mainwindKeyRelease_cb(void* pWind);
    static void prewindKeyPress_cb(void* pWind);
    static void prewindKeyRelease_cb(void* pWind);
    static void nextwindKeyPress_cb(void* pWind);
    static void nextwindKeyRelease_cb(void* pWind);
    static void modifyPwdPress_cb(void* pWind);
    static void modifyPwdRelease_cb(void* pWind);
    static void loggingKeyPress_cb(void* pWind);
    static void loggingKeyRelease_cb(void* pWind);
    static void logginoutKeyPress_cb(void* pWind);
    static void logginoutKeyRelease_cb(void* pWind);
    static KEYXPRESS_CB(0);
    static KEYXRELEASE_CB(0);
    static KEYXPRESS_CB(1);
    static KEYXRELEASE_CB(1);
    static KEYXPRESS_CB(2);
    static KEYXRELEASE_CB(2);
    static KEYXPRESS_CB(3);
    static KEYXRELEASE_CB(3);
    static KEYXPRESS_CB(4);
    static KEYXRELEASE_CB(4);
    static KEYXPRESS_CB(5);
    static KEYXRELEASE_CB(5);
    static KEYXPRESS_CB(6);
    static KEYXRELEASE_CB(6);
    static KEYXPRESS_CB(7);
    static KEYXRELEASE_CB(7);
    static KEYXPRESS_CB(9);
    static KEYXRELEASE_CB(9);
    static KEYXPRESS_CB(10);
    static KEYXRELEASE_CB(10);
    static KEYXPRESS_CB(11);
    static KEYXRELEASE_CB(11);
    static KEYXPRESS_CB(12);
    static KEYXRELEASE_CB(12);
    static KEYXPRESS_CB(13);
    static KEYXRELEASE_CB(13);
    static KEYXPRESS_CB(14);
    static KEYXRELEASE_CB(14);
    static KEYXPRESS_CB(15);
    static KEYXRELEASE_CB(15);
    static KEYXPRESS_CB(18);
    static KEYXRELEASE_CB(18);
    static KEYXPRESS_CB(19);
    static KEYXRELEASE_CB(19);
    static KEYXPRESS_CB(20);
    static KEYXRELEASE_CB(20);
    static KEYXPRESS_CB(21);
    static KEYXRELEASE_CB(21);

    MainWind();
	void event_init_screen();
	void camera_set_image(lv_obj_t* img_domain, CameraState state);
	void set_running_info(const std::string & info_str);
	void read_fault(const std::string & reg);
	void read_indict_led(int led_index);
    void read_plc_value();
    void set_identity();
	static void* sys_clock_handler(void* args);
	static void* plc_read_handler(void* args);
	static void plc_connection_success_callback(void* args);
	static void plc_connection_fail_callback(void* args);
	static void wifi_connection_success_callback(void* args);
	static void wifi_connection_fail_callback(void* args);
	static void setting_event_cb(lv_event_t* e);
	static void link_event_cb(lv_event_t* e);
	static void image_source_event_cb(lv_event_t* e);
	static void lcamera_connection_connecting_cb(void* args);
	static void lcamera_connection_success_cb(void* args);
	static void lcamera_connection_fail_cb(void* args);
	static void rcamera_connection_connection_cb(void* args);
	static void rcamera_connection_success_cb(void* args);
	static void rcamera_connection_fail_cb(void* args);
    static void* mainWind_value_update_thread(void* args);

    friend LoggingWind;
    friend void ofmt_set_running_info(void* pValue, void* pTextarea, void* preg);
    friend void ofmt_work_record(void* pValue, void* pWind, void* pReg);
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
    Keyboard* _pKeyboard;
    MainWind* _pParentWind;
    Identity* _pIdentity;
    MessageWind _messageWind;

public:
    static ModifyPasswordWind* getInstance(MainWind* pWind){
        static ModifyPasswordWind wind(pWind);
        return &wind;
    } 
    void show();
    void hide();
private:
    ModifyPasswordWind() = delete;
    ModifyPasswordWind(MainWind* pParentWind);
	~ModifyPasswordWind();
	void event_init_screen();
    static void close_event_cb(lv_event_t* e);
    static void btnmatrix_event_cb(lv_event_t* e);
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
    MainWind* _pParentWind;
    Keyboard* _pKeyboard;
    Identity* _pIdentity;
    MessageWind _messageWind;
public:
    static LoggingWind* getInstance(MainWind* pWind){
        static LoggingWind wind(pWind);
        return &wind;
    }
    void show();
    void hide();
private:
    LoggingWind() = delete;
    LoggingWind(MainWind* pParent);
    ~LoggingWind();
	void event_init_screen();
    static void dropdown_event_cb(lv_event_t* e);
    static void close_event_cb(lv_event_t* e);
    static void btnmatrix_event_cb(lv_event_t* e);
};

MainWind* lv_main_widgets();

#endif