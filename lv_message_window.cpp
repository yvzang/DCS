#include "lv_message_window.h"
#include "limlog.h"
#include "utils.h"

#define CONFIG_PLC_ADDRESS_FILE						"/etc/plc.conf"

LV_FONT_DECLARE(lv_font_cn_songti_medium_21);
LV_FONT_DECLARE(lv_font_cn_songti_medium_21_msg);
LV_FONT_DECLARE(lv_font_cn_songti_medium_21_media_cfg);
LV_IMG_DECLARE(down_20x20);
LV_IMG_DECLARE(off_20x20);

extern TaskManager gTaskManager;
extern void ui_init_style(lv_style_t* style);

MessageWind::MessageWind():msg_wind(NULL){

}

MessageWind::~MessageWind(){

}

void MessageWind::create_wind(lv_obj_t* parent, const std::string & title,
                const std::string &msg){
    if(msg_wind != NULL)return;
    static const char* btns[] = {" ", " ", "确定", ""};
    msg_wind = lv_msgbox_create(parent, title.c_str(), msg.c_str(),
                            btns, false);
    lv_obj_set_size(msg_wind, 300, 170);
    lv_obj_align(msg_wind, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_border_width(msg_wind, 0, 0); /* 去除边框 */
    lv_obj_set_style_shadow_width(msg_wind, 20, 0); /* 设置阴影宽度 */
    lv_obj_set_style_shadow_color(msg_wind, lv_color_hex(0xa9a9a9), LV_STATE_DEFAULT); /* 设置阴影颜色 */
    lv_obj_set_style_pad_top(msg_wind,18,LV_STATE_DEFAULT); /* 设置顶部填充 */
    lv_obj_set_style_pad_left(msg_wind,20,LV_STATE_DEFAULT); /* 设置左侧填充 */
    lv_obj_add_event_cb(msg_wind, msgbox_event_cb, LV_EVENT_VALUE_CHANGED, this);

    lv_obj_t *title_label = lv_msgbox_get_title(msg_wind); /* 获取标题部分 */
    lv_obj_set_style_text_font(msg_wind, &lv_font_cn_songti_medium_21_msg, LV_STATE_DEFAULT); /* 设置字体 */
    lv_obj_set_style_text_color(title_label, lv_color_hex(0xff0000), LV_STATE_DEFAULT); /* 设置文本颜色：红色 */

    lv_obj_t *content = lv_msgbox_get_content(msg_wind); /* 获取主体部分 */
    lv_obj_set_style_text_font(content, &lv_font_cn_songti_medium_21_msg, LV_STATE_DEFAULT); /* 设置字体 */
    lv_obj_set_style_text_color(content, lv_color_hex(0x6c6c6c), LV_STATE_DEFAULT); /* 设置文本颜色：灰色 */
    lv_obj_set_style_pad_top(content,15,LV_STATE_DEFAULT); /* 设置顶部填充 */

    lv_obj_t *btn = lv_msgbox_get_btns(msg_wind); /* 获取按钮矩阵部分 */
    lv_obj_set_style_bg_opa(btn, 0, LV_PART_ITEMS); /* 设置按钮背景透明度 */
    lv_obj_set_style_shadow_width(btn, 0, LV_PART_ITEMS); /* 去除按钮阴影 */
    lv_obj_set_style_text_font(btn, &lv_font_cn_songti_medium_21_msg, LV_PART_ITEMS);
    /* 设置文本颜色（未按下）：蓝色 */
    lv_obj_set_style_text_color(btn, lv_color_hex(0x2271df), LV_PART_ITEMS);
    /* 设置文本颜色（已按下）：红色 */
    lv_obj_set_style_text_color(btn, lv_color_hex(0xff0000), LV_PART_ITEMS|LV_STATE_PRESSED);

}

void MessageWind::create_without_btn_wind(lv_obj_t* parent, 
                                const std::string &msg){
    if(msg_wind != NULL)return;
    static const char* btns[] = {""};
    msg_wind = lv_msgbox_create(parent, " ", msg.c_str(),
                            btns, false);
    lv_obj_set_size(msg_wind, 300, 170);
    lv_obj_align(msg_wind, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_border_width(msg_wind, 0, 0); /* 去除边框 */
    lv_obj_set_style_shadow_width(msg_wind, 20, 0); /* 设置阴影宽度 */
    lv_obj_set_style_shadow_color(msg_wind, lv_color_hex(0xa9a9a9), LV_STATE_DEFAULT); /* 设置阴影颜色 */
    lv_obj_set_style_pad_top(msg_wind,18,LV_STATE_DEFAULT); /* 设置顶部填充 */
    lv_obj_set_style_pad_left(msg_wind,20,LV_STATE_DEFAULT); /* 设置左侧填充 */

    lv_obj_t *title_label = lv_msgbox_get_title(msg_wind); /* 获取标题部分 */
    lv_obj_set_style_text_font(msg_wind, &lv_font_cn_songti_medium_21_msg, LV_STATE_DEFAULT); /* 设置字体 */
    lv_obj_set_style_text_color(title_label, lv_color_hex(0xff0000), LV_STATE_DEFAULT); /* 设置文本颜色：红色 */

    lv_obj_t *content = lv_msgbox_get_content(msg_wind); /* 获取主体部分 */
    lv_obj_set_style_text_font(content, &lv_font_cn_songti_medium_21_msg, LV_STATE_DEFAULT); /* 设置字体 */
    lv_obj_set_style_text_color(content, lv_color_hex(0x6c6c6c), LV_STATE_DEFAULT); /* 设置文本颜色：灰色 */
    lv_obj_set_style_pad_top(content,15,LV_STATE_DEFAULT); /* 设置顶部填充 */
    lv_obj_set_style_text_align(content, LV_TEXT_ALIGN_CENTER, 0);
}

void MessageWind::create_error_wind(const std::string &msg){
    std::string title("错误！");
    create_wind(lv_layer_top(), title, msg);
}

void MessageWind::create_info_wind(const std::string &msg){
    std::string title("通知！");
    create_wind(lv_layer_top(), title, msg);
}

void MessageWind::delete_wind(){
    if(msg_wind){
        lv_obj_del_async(msg_wind);
        msg_wind = NULL;
    }
}

void MessageWind::msgbox_event_cb(lv_event_t* e){
    lv_obj_t *target = lv_event_get_current_target(e); /* 获取当前触发源 */
    MessageWind* pWind = (MessageWind*)lv_event_get_user_data(e);
    if(lv_msgbox_get_active_btn(target) == 2) /* 获取按钮索引 */
    {
        pWind->delete_wind();
    }

}


CameraSettingWind::CameraSettingWind(Camera* camera)
:_pCamera(camera),
 _keyboard(VirtualKeyboard::getInstance()){
    message_window = lv_obj_create(lv_layer_top());
    lv_obj_set_size(message_window, 450, 300);
    lv_obj_set_align(message_window, LV_ALIGN_CENTER);

    /* center widget */
    lv_obj_t* center_widget = lv_obj_create(message_window);
    lv_obj_set_align(center_widget, LV_ALIGN_CENTER);
    lv_obj_set_size(center_widget, 370, 180);
    lv_obj_set_scrollbar_mode(center_widget, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_width(center_widget, 0, 0);
    lv_obj_set_style_pad_all(center_widget, 0, 0);

    /* button widget */
    lv_obj_t* button_widget = lv_obj_create(message_window);
    lv_obj_set_size(button_widget, 250, 40);
    lv_obj_align_to(button_widget, center_widget, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_scrollbar_mode(button_widget, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_width(button_widget, 0, 0);
    lv_obj_set_style_pad_all(button_widget, 0, 0);


    static lv_coord_t column_dsc[] = { 100, 250, LV_GRID_TEMPLATE_LAST };
    static lv_coord_t row_dsc[] = { 35, 35, 35, 35, LV_GRID_TEMPLATE_LAST };

    lv_obj_set_style_grid_column_dsc_array(center_widget, column_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(center_widget, row_dsc, 0);
    lv_obj_set_layout(center_widget, LV_LAYOUT_GRID);

    /* label style */
    static lv_style_t label_style;
    lv_style_init(&label_style);
    lv_style_set_align(&label_style, LV_ALIGN_LEFT_MID);
    lv_style_set_text_font(&label_style, &lv_font_cn_songti_medium_21_media_cfg);
    /* textarea style */
    static lv_style_t text_style;
    lv_style_init(&text_style);
    lv_style_set_align(&text_style, LV_ALIGN_LEFT_MID);
    lv_style_set_pad_all(&text_style, 0);
    lv_style_set_text_font(&text_style, &lv_font_cn_songti_medium_21_media_cfg);

    /* ip地址栏 */
    lv_obj_t* ip_label = lv_label_create(center_widget);
    lv_label_set_text(ip_label, "ip地址：");
    lv_obj_add_style(ip_label, &label_style, LV_STATE_DEFAULT);
    lv_obj_set_grid_cell(ip_label, LV_GRID_ALIGN_STRETCH, 0, 1,
        LV_GRID_ALIGN_STRETCH, 0, 1);
    ip_text = lv_textarea_create(center_widget);
    lv_textarea_set_placeholder_text(ip_text, "输入摄像头ip");
    lv_textarea_set_one_line(ip_text, true);
    lv_obj_add_style(ip_text, &text_style, 0);
    lv_obj_set_scrollbar_mode(ip_text, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(ip_text, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_grid_cell(ip_text, LV_GRID_ALIGN_STRETCH, 1, 1,
        LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_add_event_cb(ip_text, VirtualKeyboard::keyboard_event_cb, LV_EVENT_ALL, _keyboard);

    /* port栏 */
    lv_obj_t* port_label = lv_label_create(center_widget);
    lv_label_set_text(port_label, "端口：");
    lv_obj_add_style(port_label, &label_style, LV_STATE_DEFAULT);
    lv_obj_set_grid_cell(port_label, LV_GRID_ALIGN_STRETCH, 0, 1,
        LV_GRID_ALIGN_STRETCH, 1, 1);
    port_text = lv_textarea_create(center_widget);
    lv_textarea_set_one_line(port_text, true);
    lv_textarea_set_placeholder_text(port_text, "输入rtsp端口");
    lv_obj_add_style(port_text, &text_style, 0);
    lv_obj_set_scrollbar_mode(port_text, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(port_text, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_grid_cell(port_text, LV_GRID_ALIGN_STRETCH, 1, 1,
        LV_GRID_ALIGN_STRETCH, 1, 1);
    lv_obj_add_event_cb(port_text, VirtualKeyboard::keyboard_event_cb, LV_EVENT_ALL, _keyboard);

    /* 账号栏 */
    lv_obj_t* acount_label = lv_label_create(center_widget);
    lv_label_set_text(acount_label, "账号：");
    lv_obj_add_style(acount_label, &label_style, LV_STATE_DEFAULT);
    lv_obj_set_grid_cell(acount_label, LV_GRID_ALIGN_STRETCH, 0, 1,
        LV_GRID_ALIGN_STRETCH, 2, 1);
    account_text = lv_textarea_create(center_widget);
    lv_textarea_set_one_line(account_text, true);
    lv_textarea_set_placeholder_text(account_text, "输入账号");
    lv_obj_add_style(account_text, &text_style, 0);
    lv_obj_set_scrollbar_mode(account_text, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(account_text, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_grid_cell(account_text, LV_GRID_ALIGN_STRETCH, 1, 1,
        LV_GRID_ALIGN_STRETCH, 2, 1);
    lv_obj_add_event_cb(account_text, VirtualKeyboard::keyboard_event_cb, LV_EVENT_ALL, _keyboard);

    /* 密码栏 */
    lv_obj_t* pwd_label = lv_label_create(center_widget);
    lv_label_set_text(pwd_label, "密码：");
    lv_obj_add_style(pwd_label, &label_style, LV_STATE_DEFAULT);
    lv_obj_set_grid_cell(pwd_label, LV_GRID_ALIGN_STRETCH, 0, 1,
        LV_GRID_ALIGN_STRETCH, 3, 1);
    pwd_text = lv_textarea_create(center_widget);
    lv_textarea_set_one_line(pwd_text, true);
    lv_textarea_set_password_mode(pwd_text, true);
    lv_textarea_set_placeholder_text(pwd_text, "输入密码");
    lv_obj_add_style(pwd_text, &text_style, 0);
    lv_obj_set_scrollbar_mode(pwd_text, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(pwd_text, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_grid_cell(pwd_text, LV_GRID_ALIGN_STRETCH, 1, 1,
        LV_GRID_ALIGN_STRETCH, 3, 1);
    lv_obj_add_event_cb(pwd_text, VirtualKeyboard::keyboard_event_cb, LV_EVENT_ALL, _keyboard);

    /* 按键栏 */
    static const char* map[] = { "保存", "取消", "" };
    lv_obj_t* btnmx = lv_btnmatrix_create(button_widget);
    lv_btnmatrix_set_map(btnmx, map);
    lv_obj_set_style_pad_all(btnmx, 0, 0);
    lv_obj_set_scrollbar_mode(btnmx, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(btnmx, lv_pct(100), lv_pct(100));
    lv_obj_set_style_text_font(btnmx, &lv_font_cn_songti_medium_21_media_cfg, 0);
    lv_obj_add_event_cb(btnmx, btnmetrix_event_cb, LV_EVENT_VALUE_CHANGED, this);
    hind();
}

void CameraSettingWind::hind(){
    lv_obj_clear_flag(lv_layer_sys(), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_TRANSP, 0);
    lv_obj_add_flag(message_window, LV_OBJ_FLAG_HIDDEN);
}

void CameraSettingWind::show(){
    lv_textarea_set_text(ip_text, _pCamera->info.ip_addr.c_str());
    lv_textarea_set_text(account_text, _pCamera->info.account.c_str());
    lv_textarea_set_text(port_text, std::to_string(_pCamera->info.port).c_str());
    lv_textarea_set_text(pwd_text, _pCamera->info.pwd.c_str());
    lv_obj_add_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_color(lv_layer_top(), lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_50, 0);
    lv_obj_clear_flag(message_window, LV_OBJ_FLAG_HIDDEN);
}

void CameraSettingWind::btnmetrix_event_cb(lv_event_t* e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* target = lv_event_get_target(e);
    CameraSettingWind* wind = (CameraSettingWind*)lv_event_get_user_data(e);

    if (code == LV_EVENT_VALUE_CHANGED) {
        uint8_t id = lv_btnmatrix_get_selected_btn(target);
        /* 确定按键 */
        if (id == 0) {
            const char* account_ptr = lv_textarea_get_text(wind->account_text);
            const char* pwd_ptr = lv_textarea_get_text(wind->pwd_text);
            const char* ip_addr_ptr = lv_textarea_get_text(wind->ip_text);
            int port = atoi(lv_textarea_get_text(wind->port_text));
            LOG_DEBUG << "read input data: ";
            LOG_DEBUG << "account :" << account_ptr;
            LOG_DEBUG << "password: " << pwd_ptr;
            LOG_DEBUG << "ip address: " << ip_addr_ptr;
            LOG_DEBUG << "port: " << port;
            (*wind->_pCamera)(account_ptr, pwd_ptr, ip_addr_ptr, port);
            wind->_pCamera->camera_connect_async();
            wind->hind();
        }
        else if (id == 1) {
            wind->hind();
        }
    }
}

CameraSettingWind::~CameraSettingWind(){
    lv_obj_del(message_window);
};


WIFISettingWind::WIFISettingWind(WIFIConnectCallback successCallback,
					WIFIConnectCallback failCallback,
					Camera* left_camera, Camera* right_camera)
:_pWiFiManager(getWiFiManagerInstance()), 
_keyboard(VirtualKeyboard::getInstance()),
_pLefgCamera(left_camera),
_pRightCamera(right_camera),
SuccessCallback_(successCallback),
FailCallback_(failCallback){
    /* label style */
    static lv_style_t label_style;
    lv_style_init(&label_style);
    //lv_style_set_align(&label_style, LV_ALIGN_LEFT_MID);
    lv_style_set_text_font(&label_style, &lv_font_cn_songti_medium_21_media_cfg);
    /* textarea style */
    static lv_style_t dropdown_style;
    lv_style_init(&dropdown_style);
    //lv_style_set_align(&dropdown_style, LV_ALIGN_LEFT_MID);
    lv_style_set_pad_all(&dropdown_style, 0);
    lv_style_set_text_font(&dropdown_style, &lv_font_cn_songti_medium_21_media_cfg);

    lv_obj_add_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
    setting_window = lv_obj_create(lv_layer_top());
    lv_obj_set_style_bg_color(lv_layer_top(), lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_50, 0);
    lv_obj_set_size(setting_window, 380, 220);
    lv_obj_set_align(setting_window, LV_ALIGN_CENTER);
    lv_obj_set_scrollbar_mode(setting_window, LV_SCROLLBAR_MODE_OFF);

    /* center widget */
    lv_obj_t* center_widget = lv_obj_create(setting_window);
    lv_obj_set_align(center_widget, LV_ALIGN_CENTER);
    lv_obj_set_size(center_widget, 320, 100);
    lv_obj_set_scrollbar_mode(center_widget, LV_SCROLLBAR_MODE_OFF);
    //lv_obj_set_style_border_width(center_widget, 0, 0);
    lv_obj_set_style_pad_all(center_widget, 0, 0);

    /* button widget */
    lv_obj_t* button_widget = lv_obj_create(setting_window);
    lv_obj_set_size(button_widget, 250, 40);
    lv_obj_align_to(button_widget, center_widget, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_scrollbar_mode(button_widget, LV_SCROLLBAR_MODE_OFF);
    //lv_obj_set_style_border_width(button_widget, 0, 0);
    lv_obj_set_style_pad_all(button_widget, 0, 0);

    static lv_coord_t column_dsc[] = { 100, 200, LV_GRID_TEMPLATE_LAST };
    static lv_coord_t row_dsc[] = { 35, 35, LV_GRID_TEMPLATE_LAST };

    lv_obj_set_style_grid_column_dsc_array(center_widget, column_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(center_widget, row_dsc, 0);
    lv_obj_set_layout(center_widget, LV_LAYOUT_GRID);

    /* wifi名称栏 */
    lv_obj_t* wifi_name_label = lv_label_create(center_widget);
    lv_label_set_text(wifi_name_label, "WIFI：");
    lv_obj_add_style(wifi_name_label, &label_style, LV_STATE_DEFAULT);
    lv_obj_set_grid_cell(wifi_name_label, LV_GRID_ALIGN_STRETCH, 0, 1,
        LV_GRID_ALIGN_STRETCH, 0, 1);
    _wifiList = lv_dropdown_create(center_widget);
    lv_dropdown_set_symbol(_wifiList, &down_20x20);
    lv_dropdown_set_selected(_wifiList, 0);
    lv_obj_add_style(_wifiList, &dropdown_style, LV_STATE_DEFAULT);
    lv_obj_set_grid_cell(_wifiList, LV_GRID_ALIGN_STRETCH, 1, 1,
        LV_GRID_ALIGN_STRETCH, 0, 1);

    /* wifi 密码 */
    lv_obj_t* wifi_pwd_label = lv_label_create(center_widget);
    lv_label_set_text(wifi_pwd_label, "密码：");
    lv_obj_add_style(wifi_pwd_label, &label_style, LV_STATE_DEFAULT);
    lv_obj_set_grid_cell(wifi_pwd_label, LV_GRID_ALIGN_STRETCH, 0, 1,
        LV_GRID_ALIGN_STRETCH, 1, 1);
    wifi_pwd = lv_textarea_create(center_widget);
    lv_textarea_set_one_line(wifi_pwd, true);
    lv_textarea_set_placeholder_text(wifi_pwd, "输入密码");
    lv_textarea_set_password_mode(wifi_pwd, true);
    lv_obj_add_style(wifi_pwd, &dropdown_style, 0);
    lv_obj_set_scrollbar_mode(wifi_pwd, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(wifi_pwd, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_grid_cell(wifi_pwd, LV_GRID_ALIGN_STRETCH, 1, 1,
        LV_GRID_ALIGN_STRETCH, 1, 1);
    lv_obj_add_event_cb(wifi_pwd, VirtualKeyboard::keyboard_event_cb, LV_EVENT_ALL, _keyboard);

    /* 按键栏 */
    static const char* map[] = { "保存", "取消", "" };
    lv_obj_t* btnmx = lv_btnmatrix_create(button_widget);
    lv_btnmatrix_set_map(btnmx, map);
    lv_obj_set_style_pad_all(btnmx, 0, 0);
    lv_obj_set_scrollbar_mode(btnmx, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(btnmx, lv_pct(100), lv_pct(100));
    lv_obj_set_style_text_font(btnmx, &lv_font_cn_songti_medium_21_media_cfg, 0);
    lv_obj_add_event_cb(btnmx, setting_btnmatrix_event_cb, LV_EVENT_VALUE_CHANGED, this);

    hind();
}

WifiManager* WIFISettingWind::getWiFiManagerInstance(){
    static WifiManager manager(INTER_WLAN);
    return &manager;
}


void WIFISettingWind::hind(){
    lv_obj_clear_flag(lv_layer_sys(), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_TRANSP, 0);
    lv_obj_add_flag(setting_window, LV_OBJ_FLAG_HIDDEN);

	Stop_.store(true);
}

void WIFISettingWind::show(){
    lv_obj_set_style_bg_color(lv_layer_top(), lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_50, 0);
    lv_obj_add_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(setting_window, LV_OBJ_FLAG_HIDDEN);

	if(Stop_.load()){
		Stop_.store(false);
		ScanWIFIThread_ = std::thread(std::bind(&WIFISettingWind::_scanWIFILoopFunc, this));
		ScanWIFIThread_.detach();
	}
}

void WIFISettingWind::showWaitingWind(const std::string & msg){
	_pMessageWind.create_without_btn_wind(lv_layer_top(), msg);
}

void WIFISettingWind::destroyWaitingWind(){
	_pMessageWind.delete_wind();
}

void WIFISettingWind::_connSuccessFunc(){
	destroyWaitingWind();
	_pMessageWind.create_info_wind("连接成功！");
	SuccessCallback_();
}

void WIFISettingWind::_connFailFunc(){
	destroyWaitingWind();
	_pMessageWind.create_info_wind("连接失败！");
	FailCallback_();
}

void WIFISettingWind::setting_btnmatrix_event_cb(lv_event_t* e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* target = lv_event_get_target(e);
    char buf[64]{0};
    WIFISettingWind* setting_window = reinterpret_cast<WIFISettingWind*>(lv_event_get_user_data(e));

    if(code == LV_EVENT_VALUE_CHANGED){
        auto id = lv_btnmatrix_get_selected_btn(target);
        /* 确定按键 */
        if(id == 0){
			//window of connecting
			setting_window->_pMessageWind.create_without_btn_wind(lv_layer_top(), "正在连接");
            //setting_window->_pMessageWind.create_without_btn_wind(lv_layer_top(), "正在连接wifi...");
            setting_window->_pLefgCamera->camera_stop();
            setting_window->_pRightCamera->camera_stop();
            lv_dropdown_get_selected_str(setting_window->_wifiList, buf, sizeof(buf));
            std::string wifiName = buf;
            std::string wifiPwd = lv_textarea_get_text(setting_window->wifi_pwd);
            if(setting_window->_pWiFiManager->connect_async(wifiName, wifiPwd, 
				std::bind(&WIFISettingWind::_connSuccessFunc, setting_window), 
				std::bind(&WIFISettingWind::_connFailFunc, setting_window))){
                setting_window->hind();
            }
        }
        else if(id == 1){   //取消
            setting_window->hind();
        }
    }
}

void WIFISettingWind::_scanWIFILoopFunc(){
	while(1){
		if(Stop_.load()) return;
		_pWiFiManager->wifi_scan();

		/* 显示wifi列表 */
		if(lv_dropdown_get_option_cnt(_wifiList) > 0){
			//lv_dropdown_clear_options(_wifiList);
		}
		int i = 0;
		std::string options_str;
		for(auto item_ptr = _pWiFiManager->getWifiInfo()->begin();
			item_ptr != _pWiFiManager->getWifiInfo()->end(); item_ptr++,i++)
		{
			//lv_dropdown_add_option(_wifiList, item_ptr->wifiName.c_str(), i);
			options_str += (item_ptr->wifiName + "\n");
		}
		options_str.erase(options_str.end()-1, options_str.end());
		printf("options string: %s\n", options_str.c_str());
		lv_dropdown_set_options(_wifiList, options_str.c_str());

		std::this_thread::sleep_for(std::chrono::seconds(10));
	}
}


LinkSettingWind::LinkSettingWind(PLCConnectionCallback successCallback,
								PLCConnectionCallback failCallback):
_keyboard(VirtualKeyboard::getInstance()),
SuccessCallback_(successCallback),
FailCallback_(failCallback),
PLCDeviceName_("mc_plc"){
    /* label style */
    static lv_style_t label_style;
    lv_style_init(&label_style);
    lv_style_set_text_font(&label_style, &lv_font_cn_songti_medium_21_media_cfg);
    lv_style_set_flex_grow(&label_style, 4);
    /* textarea style */
    static lv_style_t dropdown_style;
    lv_style_init(&dropdown_style);
    lv_style_set_pad_all(&dropdown_style, 0);
    lv_style_set_text_font(&dropdown_style, &lv_font_cn_songti_medium_21_media_cfg);
    lv_style_set_flex_grow(&dropdown_style, 7);
    /* content box style */
    static lv_style_t content_box_style;
    lv_style_init(&content_box_style);
    lv_style_set_text_font(&content_box_style, &lv_font_cn_songti_medium_21_media_cfg);
    lv_style_set_flex_flow(&content_box_style, LV_FLEX_FLOW_ROW);
    lv_style_set_flex_cross_place(&content_box_style, LV_FLEX_ALIGN_CENTER);
    lv_style_set_flex_track_place(&content_box_style, LV_FLEX_ALIGN_CENTER);
    lv_style_set_layout(&content_box_style, LV_LAYOUT_FLEX);
    lv_style_set_pad_all(&content_box_style, 0);

    lv_obj_add_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
    _linking_window = lv_obj_create(lv_layer_top());
    lv_obj_set_style_bg_color(lv_layer_top(), lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_50, 0);
    lv_obj_set_size(_linking_window, 400, 130);
    lv_obj_set_align(_linking_window, LV_ALIGN_CENTER);
    lv_obj_set_scrollbar_mode(_linking_window, LV_SCROLLBAR_MODE_OFF);

    /* center widget */
    lv_obj_t* center_widget = lv_obj_create(_linking_window);
    lv_obj_add_style(center_widget, &content_box_style, 0);
    lv_obj_set_size(center_widget, 360, 50);
    lv_obj_set_scrollbar_mode(center_widget, LV_SCROLLBAR_MODE_OFF);
    //lv_obj_set_style_border_width(center_widget, 0, 0);

    /* button widget */
    lv_obj_t* button_widget = lv_obj_create(_linking_window);
    lv_obj_set_size(button_widget, 250, 40);
    lv_obj_align_to(button_widget, center_widget, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_scrollbar_mode(button_widget, LV_SCROLLBAR_MODE_OFF);
    //lv_obj_set_style_border_width(button_widget, 0, 0);
    lv_obj_set_style_pad_all(button_widget, 0, 0);

    /* PLC IP */
    lv_obj_t* plc_label = lv_label_create(center_widget);
    lv_label_set_text(plc_label, "PLC地址：");
    lv_obj_add_style(plc_label, &label_style, LV_STATE_DEFAULT);
    plc_ip = lv_textarea_create(center_widget);
    lv_textarea_set_one_line(plc_ip, true);
    lv_textarea_set_placeholder_text(plc_ip, "输入plc地址");
    lv_obj_add_style(plc_ip, &dropdown_style, 0);
    lv_obj_set_scrollbar_mode(plc_ip, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(plc_ip, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(plc_ip, VirtualKeyboard::keyboard_event_cb, LV_EVENT_ALL, _keyboard);
    /* 按键栏 */
    static const char* map[] = { "保存", "取消", "" };
    lv_obj_t* btnmx = lv_btnmatrix_create(button_widget);
    lv_btnmatrix_set_map(btnmx, map);
    lv_obj_set_style_pad_all(btnmx, 0, 0);
    lv_obj_set_scrollbar_mode(btnmx, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(btnmx, lv_pct(100), lv_pct(100));
    lv_obj_set_style_text_font(btnmx, &lv_font_cn_songti_medium_21_media_cfg, 0);
    lv_obj_add_event_cb(btnmx, setting_btnmatrix_event_cb, LV_EVENT_VALUE_CHANGED, this);

	readPLCConfigFile(CONFIG_PLC_ADDRESS_FILE, PLCIP);
	lv_textarea_set_text(plc_ip, PLCIP.c_str());

    hind();
}

void LinkSettingWind::hind(){
    lv_obj_clear_flag(lv_layer_sys(), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_TRANSP, 0);
    lv_obj_add_flag(_linking_window, LV_OBJ_FLAG_HIDDEN);
}

void LinkSettingWind::show(){
    lv_obj_set_style_bg_color(lv_layer_top(), lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_50, 0);
    lv_obj_add_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(_linking_window, LV_OBJ_FLAG_HIDDEN);
}

bool LinkSettingWind::plc_connect(){
    std::string ipstr = lv_textarea_get_text(plc_ip);
    if(ipstr.length() > 0){
        DeviceDescribe describe;
		describe.deviceName = PLCDeviceName_;
		describe.protoType = PROTOCOL_TYPE_MC;
		describe.ipAddr = ipstr;
		describe.port = 5551;
		if(gTaskManager.registerDevice(describe)){
			PLCIP.assign(ipstr);
			SuccessCallback_();
			writePLCConfigFile(CONFIG_PLC_ADDRESS_FILE);
			gTaskManager.run();
			return true;
		}
		FailCallback_();
		return false;
    }
}

void LinkSettingWind::readPLCConfigFile(const std::string & path,
										std::string & plcIP)
{
	std::string readstr;
	cJSON* jsoncfg;
	int ret = file_read(path, readstr);
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

void LinkSettingWind::writePLCConfigFile(const std::string & path){
	cJSON* plccfg = cJSON_CreateObject();
	cJSON_AddStringToObject(plccfg, "ipaddr", PLCIP.c_str());
	std::string jsonstr(cJSON_Print(plccfg));
	LOG_DEBUG << "write " << CONFIG_PLC_ADDRESS_FILE << ": " << jsonstr;

	file_write(CONFIG_PLC_ADDRESS_FILE, jsonstr);
	cJSON_Delete(plccfg);
}

void LinkSettingWind::setting_btnmatrix_event_cb(lv_event_t* e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* target = lv_event_get_target(e);
    char buf[64]{0};
    LinkSettingWind* linking_window = reinterpret_cast<LinkSettingWind*>(lv_event_get_user_data(e));

    if(code == LV_EVENT_VALUE_CHANGED){
        auto id = lv_btnmatrix_get_selected_btn(target);
        /* 确定按键 */
        if(id == 0){
            std::string ip = lv_textarea_get_text(linking_window->plc_ip);
            LOG_DEBUG << "connecting " << ip << ":5551";
            linking_window->plc_connect();
            linking_window->hind();
        }
        else if(id == 1){   //取消
            linking_window->hind();
        }
    }
}


LoggingWind::LoggingWind()
: _pKeyboard(VirtualKeyboard::getInstance()),
_pAccessContrl(AccessContrl::getInstance()){
    ui = new lv_ui;
    //Write codes user_choice_win_user_choise_win
    ui->user_choice_win_user_choise_win = lv_win_create(lv_layer_top(), 40);
    lv_obj_t* user_choice_win_user_choise_win_title = lv_win_add_title(ui->user_choice_win_user_choise_win, "用户选择");
    ui->user_choice_win_user_choise_win_item0 = lv_win_add_btn(ui->user_choice_win_user_choise_win, &off_20x20, 40);
    lv_obj_t* user_choice_win_user_choise_win_label = lv_label_create(lv_win_get_content(ui->user_choice_win_user_choise_win));
    lv_obj_set_scrollbar_mode(lv_win_get_content(ui->user_choice_win_user_choise_win), LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(user_choice_win_user_choise_win_label, "");
    //lv_obj_set_pos(ui->user_choice_win_user_choise_win, 312, 184);
    lv_obj_set_align(ui->user_choice_win_user_choise_win, LV_ALIGN_CENTER);
    lv_obj_set_size(ui->user_choice_win_user_choise_win, 400, 300);
    lv_obj_set_scrollbar_mode(ui->user_choice_win_user_choise_win, LV_SCROLLBAR_MODE_OFF);

    //Write style for user_choice_win_user_choise_win, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->user_choice_win_user_choise_win, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->user_choice_win_user_choise_win, lv_color_hex(0xeeeef6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->user_choice_win_user_choise_win, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->user_choice_win_user_choise_win, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->user_choice_win_user_choise_win, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_user_choice_win_user_choise_win_extra_content_main_default
    static lv_style_t style_user_choice_win_user_choise_win_extra_content_main_default;
    ui_init_style(&style_user_choice_win_user_choise_win_extra_content_main_default);

    lv_style_set_bg_opa(&style_user_choice_win_user_choise_win_extra_content_main_default, 255);
    lv_style_set_bg_color(&style_user_choice_win_user_choise_win_extra_content_main_default, lv_color_hex(0xeeeef6));
    lv_style_set_bg_grad_dir(&style_user_choice_win_user_choise_win_extra_content_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_text_color(&style_user_choice_win_user_choise_win_extra_content_main_default, lv_color_hex(0x393c41));
    lv_style_set_text_font(&style_user_choice_win_user_choise_win_extra_content_main_default, &lv_font_cn_songti_medium_21);
    lv_style_set_text_opa(&style_user_choice_win_user_choise_win_extra_content_main_default, 255);
    lv_style_set_text_letter_space(&style_user_choice_win_user_choise_win_extra_content_main_default, 0);
    lv_style_set_text_line_space(&style_user_choice_win_user_choise_win_extra_content_main_default, 2);
    lv_obj_add_style(lv_win_get_content(ui->user_choice_win_user_choise_win), &style_user_choice_win_user_choise_win_extra_content_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_user_choice_win_user_choise_win_extra_header_main_default
    static lv_style_t style_user_choice_win_user_choise_win_extra_header_main_default;
    ui_init_style(&style_user_choice_win_user_choise_win_extra_header_main_default);

    lv_style_set_bg_opa(&style_user_choice_win_user_choise_win_extra_header_main_default, 255);
    lv_style_set_bg_color(&style_user_choice_win_user_choise_win_extra_header_main_default, lv_color_hex(0xe6e6e6));
    lv_style_set_bg_grad_dir(&style_user_choice_win_user_choise_win_extra_header_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_text_color(&style_user_choice_win_user_choise_win_extra_header_main_default, lv_color_hex(0x393c41));
    lv_style_set_text_font(&style_user_choice_win_user_choise_win_extra_header_main_default, &lv_font_cn_songti_medium_21);
    lv_style_set_text_opa(&style_user_choice_win_user_choise_win_extra_header_main_default, 255);
    lv_style_set_text_letter_space(&style_user_choice_win_user_choise_win_extra_header_main_default, 0);
    lv_style_set_text_line_space(&style_user_choice_win_user_choise_win_extra_header_main_default, 2);
    lv_obj_add_style(lv_win_get_header(ui->user_choice_win_user_choise_win), &style_user_choice_win_user_choise_win_extra_header_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_user_choice_win_user_choise_win_extra_btns_main_default
    static lv_style_t style_user_choice_win_user_choise_win_extra_btns_main_default;
    ui_init_style(&style_user_choice_win_user_choise_win_extra_btns_main_default);

    lv_style_set_radius(&style_user_choice_win_user_choise_win_extra_btns_main_default, 8);
    lv_style_set_border_width(&style_user_choice_win_user_choise_win_extra_btns_main_default, 0);
    lv_style_set_bg_opa(&style_user_choice_win_user_choise_win_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_user_choice_win_user_choise_win_extra_btns_main_default, lv_color_hex(0x2195f6));
    lv_style_set_bg_grad_dir(&style_user_choice_win_user_choise_win_extra_btns_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_shadow_width(&style_user_choice_win_user_choise_win_extra_btns_main_default, 0);
    lv_obj_add_style(ui->user_choice_win_user_choise_win_item0, &style_user_choice_win_user_choise_win_extra_btns_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    //The custom code of user_choice_win.
    static lv_style_t style_content_box;
    ui_init_style(&style_content_box);
    lv_style_set_flex_flow(&style_content_box, LV_FLEX_FLOW_COLUMN);
    lv_style_set_flex_cross_place(&style_content_box, LV_FLEX_ALIGN_CENTER);
    lv_style_set_flex_track_place(&style_content_box, LV_FLEX_ALIGN_CENTER);
    lv_style_set_layout(&style_content_box, LV_LAYOUT_FLEX);
    lv_style_set_pad_all(&style_content_box, 0);

    ui->content_box = lv_win_get_content(ui->user_choice_win_user_choise_win);
    lv_obj_set_size(ui->content_box, 400, 160);
    lv_obj_add_style(ui->content_box, &style_content_box, 0);

    ui->user_dropdown = lv_dropdown_create(ui->content_box);
    lv_obj_set_width(ui->user_dropdown, lv_pct(70));
    lv_dropdown_set_options(ui->user_dropdown, "操作员\n管理员");

    ui->password_textarea = lv_textarea_create(ui->content_box);
    lv_obj_set_width(ui->password_textarea, lv_pct(70));
    lv_textarea_set_one_line(ui->password_textarea, true);
    lv_textarea_set_password_mode(ui->password_textarea, true);

    static const char* btn_str_label[] = {"确定", "取消", ""};
    ui->btn_matrix = lv_btnmatrix_create(ui->content_box);
    lv_btnmatrix_set_map(ui->btn_matrix, btn_str_label);
    lv_obj_set_height(ui->btn_matrix, 40);
    lv_obj_set_width(ui->btn_matrix, lv_pct(70));
    lv_obj_set_style_pad_all(ui->btn_matrix, 0, 0);
    lv_obj_set_style_bg_opa(ui->btn_matrix, 100, 0);
    lv_obj_set_style_border_width(ui->btn_matrix, 0, 0);

    event_init_screen();

    hide();
}

LoggingWind::~LoggingWind(){
    lv_obj_clear_flag(lv_layer_sys(), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_TRANSP, 0);
    lv_obj_del(this->ui->user_choice_win_user_choise_win);
    delete ui;
}

void LoggingWind::event_init_screen(){
    lv_obj_add_event_cb(ui->password_textarea, VirtualKeyboard::keyboard_event_cb,
                        LV_EVENT_ALL, _pKeyboard);
    lv_obj_add_event_cb(ui->btn_matrix, LoggingWind::btnmatrix_event_cb, 
                        LV_EVENT_VALUE_CHANGED, this);
    lv_obj_add_event_cb(ui->user_choice_win_user_choise_win_item0,
                        LoggingWind::close_event_cb, 
                        LV_EVENT_CLICKED, this);
    lv_obj_add_event_cb(ui->user_dropdown, LoggingWind::dropdown_event_cb,
                        LV_EVENT_ALL, this);
}

void LoggingWind::btnmatrix_event_cb(lv_event_t* e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* target = lv_event_get_target(e);
    LoggingWind* pWindow = reinterpret_cast<LoggingWind*>(lv_event_get_user_data(e));

    if(code == LV_EVENT_VALUE_CHANGED){
        auto id = lv_btnmatrix_get_selected_btn(target);
        /* 确定按键 */
        if(id == 0){
            uint16_t index = lv_dropdown_get_selected(pWindow->ui->user_dropdown);
            if(index == 0){
                pWindow->_pAccessContrl->logout();
            }
            else if(index == 1){
                std::string pwd = lv_textarea_get_text(pWindow->ui->password_textarea);
                if(pWindow->_pAccessContrl->login(pwd)){
                    LOG_DEBUG << "switch to manager";
                    //pWindow->_pParentWind->delete_modify_password_wind();
                    pWindow->hide();
                }
                else{
                    std::string msg("密码错误！");
                    pWindow->_messageWind.create_error_wind(msg);
                }
            }
        }
        else if(id == 1){   //取消
            //pWindow->_pParentWind->delete_modify_password_wind();
            pWindow->hide();
        }
    }
}

void LoggingWind::close_event_cb(lv_event_t* e){
    lv_event_code_t code = lv_event_get_code(e);
    LoggingWind* pWindow = reinterpret_cast<LoggingWind*>(lv_event_get_user_data(e));

    if(code == LV_EVENT_CLICKED){
        pWindow->hide();
    }
}

void LoggingWind::dropdown_event_cb(lv_event_t* e){
    lv_event_code_t code = lv_event_get_code(e);
    LoggingWind* pWind = (LoggingWind*)lv_event_get_user_data(e);
    lv_obj_t* pDropdownList = lv_dropdown_get_list(pWind->ui->user_dropdown);
    if(code == LV_EVENT_CLICKED){
        if(pDropdownList == NULL){
            LOG_DEBUG << "Null poiter to dropdown list.";
            return;
        }
        lv_obj_set_style_text_font(pDropdownList, &lv_font_cn_songti_medium_21, 0);
    }

    if(code == LV_EVENT_VALUE_CHANGED){
        uint16_t index = lv_dropdown_get_selected(pWind->ui->user_dropdown);
        if(index == 0){
            lv_obj_clear_flag(pWind->ui->password_textarea, LV_OBJ_FLAG_CLICKABLE);
        }
        else if(index == 1){
            lv_obj_add_flag(pWind->ui->password_textarea, LV_OBJ_FLAG_CLICKABLE);
        }
    }
}

void LoggingWind::show(){
    lv_obj_set_style_bg_color(lv_layer_top(), lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_50, 0);
    lv_obj_add_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(ui->user_choice_win_user_choise_win, LV_OBJ_FLAG_HIDDEN);

    uint16_t index = lv_dropdown_get_selected(ui->user_dropdown);
    if(index == 0){
        lv_obj_clear_flag(ui->password_textarea, LV_OBJ_FLAG_CLICKABLE);
    }
    else if(index == 1){
        lv_obj_add_flag(ui->password_textarea, LV_OBJ_FLAG_CLICKABLE);
    }
}

void LoggingWind::hide(){
    lv_obj_clear_flag(lv_layer_sys(), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_TRANSP, 0);
    lv_obj_add_flag(ui->user_choice_win_user_choise_win, LV_OBJ_FLAG_HIDDEN);
}


ModifyPasswordWind::ModifyPasswordWind()
:_pKeyboard(VirtualKeyboard::getInstance()),
_pAccessContrl(AccessContrl::getInstance()),
ui(nullptr){
    ui = new lv_ui;

    lv_obj_set_style_bg_color(lv_layer_top(), lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_50, 0);
    lv_obj_add_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
    //Write codes user_choice_win_user_choise_win
    ui->user_choice_win_user_choise_win = lv_win_create(lv_layer_top(), 40);
    lv_obj_t* user_choice_win_user_choise_win_title = lv_win_add_title(ui->user_choice_win_user_choise_win, "修改密码");
    ui->user_choice_win_user_choise_win_item0 = lv_win_add_btn(ui->user_choice_win_user_choise_win, &off_20x20, 40);
    lv_obj_t* user_choice_win_user_choise_win_label = lv_label_create(lv_win_get_content(ui->user_choice_win_user_choise_win));
    lv_obj_set_scrollbar_mode(lv_win_get_content(ui->user_choice_win_user_choise_win), LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(user_choice_win_user_choise_win_label, "");
    //lv_obj_set_pos(ui->user_choice_win_user_choise_win, 312, 184);
    lv_obj_set_align(ui->user_choice_win_user_choise_win, LV_ALIGN_CENTER);
    lv_obj_set_size(ui->user_choice_win_user_choise_win, 430, 360);
    lv_obj_set_scrollbar_mode(ui->user_choice_win_user_choise_win, LV_SCROLLBAR_MODE_OFF);

    //Write style for user_choice_win_user_choise_win, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->user_choice_win_user_choise_win, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->user_choice_win_user_choise_win, lv_color_hex(0xeeeef6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->user_choice_win_user_choise_win, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->user_choice_win_user_choise_win, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->user_choice_win_user_choise_win, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_user_choice_win_user_choise_win_extra_content_main_default
    static lv_style_t style_user_choice_win_user_choise_win_extra_content_main_default;
    ui_init_style(&style_user_choice_win_user_choise_win_extra_content_main_default);

    lv_style_set_bg_opa(&style_user_choice_win_user_choise_win_extra_content_main_default, 255);
    lv_style_set_bg_color(&style_user_choice_win_user_choise_win_extra_content_main_default, lv_color_hex(0xeeeef6));
    lv_style_set_bg_grad_dir(&style_user_choice_win_user_choise_win_extra_content_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_text_color(&style_user_choice_win_user_choise_win_extra_content_main_default, lv_color_hex(0x393c41));
    lv_style_set_text_font(&style_user_choice_win_user_choise_win_extra_content_main_default, &lv_font_cn_songti_medium_21);
    lv_style_set_text_opa(&style_user_choice_win_user_choise_win_extra_content_main_default, 255);
    lv_style_set_text_letter_space(&style_user_choice_win_user_choise_win_extra_content_main_default, 0);
    lv_style_set_text_line_space(&style_user_choice_win_user_choise_win_extra_content_main_default, 2);
    lv_obj_add_style(lv_win_get_content(ui->user_choice_win_user_choise_win), &style_user_choice_win_user_choise_win_extra_content_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_user_choice_win_user_choise_win_extra_header_main_default
    static lv_style_t style_user_choice_win_user_choise_win_extra_header_main_default;
    ui_init_style(&style_user_choice_win_user_choise_win_extra_header_main_default);

    lv_style_set_bg_opa(&style_user_choice_win_user_choise_win_extra_header_main_default, 255);
    lv_style_set_bg_color(&style_user_choice_win_user_choise_win_extra_header_main_default, lv_color_hex(0xe6e6e6));
    lv_style_set_bg_grad_dir(&style_user_choice_win_user_choise_win_extra_header_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_text_color(&style_user_choice_win_user_choise_win_extra_header_main_default, lv_color_hex(0x393c41));
    lv_style_set_text_font(&style_user_choice_win_user_choise_win_extra_header_main_default, &lv_font_cn_songti_medium_21);
    lv_style_set_text_opa(&style_user_choice_win_user_choise_win_extra_header_main_default, 255);
    lv_style_set_text_letter_space(&style_user_choice_win_user_choise_win_extra_header_main_default, 0);
    lv_style_set_text_line_space(&style_user_choice_win_user_choise_win_extra_header_main_default, 2);
    lv_obj_add_style(lv_win_get_header(ui->user_choice_win_user_choise_win), &style_user_choice_win_user_choise_win_extra_header_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_user_choice_win_user_choise_win_extra_btns_main_default
    static lv_style_t style_user_choice_win_user_choise_win_extra_btns_main_default;
    ui_init_style(&style_user_choice_win_user_choise_win_extra_btns_main_default);

    lv_style_set_radius(&style_user_choice_win_user_choise_win_extra_btns_main_default, 8);
    lv_style_set_border_width(&style_user_choice_win_user_choise_win_extra_btns_main_default, 0);
    lv_style_set_bg_opa(&style_user_choice_win_user_choise_win_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_user_choice_win_user_choise_win_extra_btns_main_default, lv_color_hex(0x2195f6));
    lv_style_set_bg_grad_dir(&style_user_choice_win_user_choise_win_extra_btns_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_shadow_width(&style_user_choice_win_user_choise_win_extra_btns_main_default, 0);
    lv_obj_add_style(ui->user_choice_win_user_choise_win_item0, &style_user_choice_win_user_choise_win_extra_btns_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    //The custom code of user_choice_win.
    static lv_style_t style_content_box;
    ui_init_style(&style_content_box);
    lv_style_set_flex_flow(&style_content_box, LV_FLEX_FLOW_COLUMN);
    lv_style_set_flex_cross_place(&style_content_box, LV_FLEX_ALIGN_CENTER);
    lv_style_set_flex_track_place(&style_content_box, LV_FLEX_ALIGN_CENTER);
    lv_style_set_layout(&style_content_box, LV_LAYOUT_FLEX);
    lv_style_set_pad_all(&style_content_box, 0);

    ui->content_box = lv_win_get_content(ui->user_choice_win_user_choise_win);
    lv_obj_set_size(ui->content_box, 400, 160);
    lv_obj_add_style(ui->content_box, &style_content_box, 0);

    ui->top_label = lv_label_create(ui->content_box);
    lv_label_set_text(ui->top_label, "管理员");

    //style定义
    static lv_style_t style_row_box;
    ui_init_style(&style_row_box);
    lv_style_set_flex_flow(&style_row_box, LV_FLEX_FLOW_ROW);
    lv_style_set_flex_cross_place(&style_row_box, LV_FLEX_ALIGN_CENTER);
    lv_style_set_flex_track_place(&style_row_box, LV_FLEX_ALIGN_CENTER);
    lv_style_set_layout(&style_row_box, LV_LAYOUT_FLEX);
    lv_style_set_pad_all(&style_row_box, 0);
    lv_style_set_width(&style_row_box, lv_pct(85));
    lv_style_set_height(&style_row_box, 40);
    lv_style_set_bg_color(&style_row_box, lv_color_hex(0xeeeef6));
    lv_style_set_border_width(&style_row_box, 0);

    static lv_style_t style_row_label;
    ui_init_style(&style_row_label);
    lv_style_set_flex_grow(&style_row_label, 4);

    static lv_style_t style_row_textarea;
    ui_init_style(&style_row_textarea);
    lv_style_set_flex_grow(&style_row_textarea, 7);

    //原始密码
    ui->row_box_1 = lv_obj_create(ui->content_box);
    lv_obj_clear_flag(ui->row_box_1, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_style(ui->row_box_1, &style_row_box, 0);
    ui->label_1 = lv_label_create(ui->row_box_1);
    lv_label_set_text(ui->label_1, "原密码：");
    lv_obj_add_style(ui->label_1, &style_row_label, 0);

    ui->opassword_textarea = lv_textarea_create(ui->row_box_1);
    lv_obj_add_style(ui->opassword_textarea, &style_row_textarea, 0);
    lv_textarea_set_one_line(ui->opassword_textarea, true);
    lv_textarea_set_password_mode(ui->opassword_textarea, true);

    //新密码
    ui->row_box_2 = lv_obj_create(ui->content_box);
    lv_obj_clear_flag(ui->row_box_2, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_style(ui->row_box_2, &style_row_box, 0);
    ui->label_2 = lv_label_create(ui->row_box_2);
    lv_label_set_text(ui->label_2, "新密码：");
    lv_obj_add_style(ui->label_2, &style_row_label, 0);

    ui->npassword_textarea = lv_textarea_create(ui->row_box_2);
    lv_obj_add_style(ui->npassword_textarea, &style_row_textarea, 0);
    lv_textarea_set_one_line(ui->npassword_textarea, true);
    lv_textarea_set_password_mode(ui->npassword_textarea, true);

    //确认密码
    ui->row_box_3 = lv_obj_create(ui->content_box);
    lv_obj_clear_flag(ui->row_box_3, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_style(ui->row_box_3, &style_row_box, 0);
    ui->label_3 = lv_label_create(ui->row_box_3);
    lv_label_set_text(ui->label_3, "确认密码：");
    lv_obj_add_style(ui->label_3, &style_row_label, 0);

    ui->cpassword_textarea = lv_textarea_create(ui->row_box_3);
    lv_obj_add_style(ui->cpassword_textarea, &style_row_textarea, 0);
    lv_textarea_set_one_line(ui->cpassword_textarea, true);
    lv_textarea_set_password_mode(ui->cpassword_textarea, true);

    static const char* btn_str_label[] = { "确定", "取消", "" };
    ui->btn_matrix = lv_btnmatrix_create(ui->content_box);
    lv_btnmatrix_set_map(ui->btn_matrix, btn_str_label);
    lv_obj_set_height(ui->btn_matrix, 40);
    lv_obj_set_width(ui->btn_matrix, lv_pct(70));
    lv_obj_set_style_pad_all(ui->btn_matrix, 0, 0);
    lv_obj_set_style_bg_opa(ui->btn_matrix, 100, 0);
    lv_obj_set_style_border_width(ui->btn_matrix, 0, 0);

    event_init_screen();

    hide();
}

ModifyPasswordWind::~ModifyPasswordWind(){
    lv_obj_clear_flag(lv_layer_sys(), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_TRANSP, 0);
    lv_obj_del(this->ui->user_choice_win_user_choise_win);
    delete ui;
}

void ModifyPasswordWind::event_init_screen(){
    lv_obj_add_event_cb(ui->opassword_textarea, VirtualKeyboard::keyboard_event_cb, 
                        LV_EVENT_ALL, _pKeyboard);
    lv_obj_add_event_cb(ui->npassword_textarea, VirtualKeyboard::keyboard_event_cb,
                        LV_EVENT_ALL, _pKeyboard);
    lv_obj_add_event_cb(ui->cpassword_textarea, VirtualKeyboard::keyboard_event_cb,
                        LV_EVENT_ALL, _pKeyboard);
    lv_obj_add_event_cb(ui->btn_matrix, ModifyPasswordWind::btnmatrix_event_cb,
                        LV_EVENT_VALUE_CHANGED, this);
    lv_obj_add_event_cb(ui->user_choice_win_user_choise_win_item0, 
                        ModifyPasswordWind::close_event_cb, 
                        LV_EVENT_CLICKED, this);
}

void ModifyPasswordWind::hide(){
    lv_obj_clear_flag(lv_layer_sys(), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_TRANSP, 0);
    lv_obj_add_flag(ui->user_choice_win_user_choise_win, LV_OBJ_FLAG_HIDDEN);
}

void ModifyPasswordWind::show(){
    lv_obj_set_style_bg_color(lv_layer_top(), lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_50, 0);
    lv_obj_add_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(ui->user_choice_win_user_choise_win, LV_OBJ_FLAG_HIDDEN);
}

void ModifyPasswordWind::close_event_cb(lv_event_t* e){
    lv_event_code_t code = lv_event_get_code(e);
    ModifyPasswordWind* pWindow = reinterpret_cast<ModifyPasswordWind*>(lv_event_get_user_data(e));

    if(code == LV_EVENT_CLICKED){
        pWindow->hide();
    }
}

void ModifyPasswordWind::btnmatrix_event_cb(lv_event_t* e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* target = lv_event_get_target(e);
    ModifyPasswordWind* pWindow = reinterpret_cast<ModifyPasswordWind*>(lv_event_get_user_data(e));

    if(code == LV_EVENT_VALUE_CHANGED){
        auto id = lv_btnmatrix_get_selected_btn(target);
        /* 确定按键 */
        if(id == 0){
            std::string opassword;
            pWindow->_pAccessContrl->get_password(0, opassword);
            std::string pwd = lv_textarea_get_text(pWindow->ui->opassword_textarea);
            if(opassword != pwd){
                std::string message = "原密码错误！";
                pWindow->_messageWind.create_error_wind(message);
                return;
            }
            else{
                std::string npwd = lv_textarea_get_text(pWindow->ui->npassword_textarea);
                std::string cpwd = lv_textarea_get_text(pWindow->ui->cpassword_textarea);
                if(cpwd != npwd){
                    std::string message = "密码不一致！";
                    pWindow->_messageWind.create_error_wind(message);
                    return;
                }
                else{
                    std::string message;
                    if(pWindow->_pAccessContrl->modify_password(0, npwd)){
                        message.assign("修改密码成功！");
                    }
                    else{
                        message.assign("密码修改失败！");
                    }
                    pWindow->_messageWind.create_info_wind(message);
                    //pWindow->_pParentWind->delete_modify_password_wind();
                    pWindow->hide();
                }
            }
        }
        else if(id == 1){   //取消
            //pWindow->_pParentWind->delete_modify_password_wind();
            pWindow->hide();
        }
    }
}