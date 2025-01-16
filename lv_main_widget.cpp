/**
 * @file lv_demo_widgets.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>
#include "lv_main_widget.h"
//#include "lv_demos/src/lv_demo.h"
#include "external/melsec_mc_net/melsec_mc_bin.h"
#include "limlog.h"
#include "Format.h"
#include "utils.h"

#if LV_USE_STDLIB_MALLOC == LV_STDLIB_BUILTIN && LV_MEM_SIZE < (38ul * 1024ul)
    #error Insufficient memory for lv_demo_widgets. Please set LV_MEM_SIZE to at least 38KB (38ul * 1024ul).  48KB is recommended.
#endif

/*********************
 *      DEFINES
 *********************/
#define BATTRERY_ADC_FS                      "/sys/class/gpadc/data"

LV_FONT_DECLARE(lv_font_cn_songti_medium_21_media_cfg);
LV_FONT_DECLARE(lv_font_cn_songti_bold);
LV_FONT_DECLARE(lv_font_cn_songti_bold_25);
LV_FONT_DECLARE(lv_font_cn_songti_medium_21);
LV_FONT_DECLARE(lv_font_cn_songti_medium_21_msg);
LV_IMG_DECLARE(green_point);
LV_IMG_DECLARE(red_point);
LV_IMG_DECLARE(column_30x30);
LV_IMG_DECLARE(battery_4);
LV_IMG_DECLARE(loading_200x200);
LV_IMG_DECLARE(no_camera_200x200);
LV_IMG_DECLARE(down_20x20);
LV_IMG_DECLARE(off_20x20);

extern pthread_mutex_t lv_lock;

void ui_init_style(lv_style_t* style);

/**********************
 *      TYPEDEFS
 **********************/

WindTaskQueue persistWindTask;
WindTaskQueue tempWindTask;

WindTaskQueue::WindTaskQueue(){
	pthread_mutex_init(&_task_lock, NULL);
}

void WindTaskQueue::register_task(uint32_t id, const WindTask & task){
	pthread_mutex_lock(&_task_lock);
	queue.insert(std::make_pair(id, task));
	pthread_mutex_unlock(&_task_lock);
}

void WindTaskQueue::unregister_task(uint32_t id){
	queue.erase(id);
}

void WindTaskQueue::clear(){
	pthread_mutex_lock(&_task_lock);
	queue.clear();
	pthread_mutex_unlock(&_task_lock);
}

std::map<uint32_t, WindTask>::iterator WindTaskQueue::begin(){
    return queue.begin();
}


std::map<uint32_t, WindTask>::iterator WindTaskQueue::end(){
    return queue.end();
}

std::map<uint32_t, WindTask>::iterator WindTaskQueue::find(uint32_t id){
    return queue.find(id);
}

/*==================Keyboard==================*/

Keyboard::Keyboard(){
    /* 创建键盘 */
    _keyboard = lv_keyboard_create(lv_layer_sys());
    lv_obj_add_flag(_keyboard, LV_OBJ_FLAG_HIDDEN);
}

Keyboard::~Keyboard(){
    lv_obj_del(_keyboard);
}

Keyboard* Keyboard::getInstance(){
    static Keyboard _keyboard;
    return &_keyboard;
}

void Keyboard::show(){
    lv_keyboard_set_textarea(_keyboard, _target);
    lv_obj_set_style_max_height(_keyboard, LV_HOR_RES * 2 / 1, 0);
    lv_obj_clear_flag(_keyboard, LV_OBJ_FLAG_HIDDEN);
    lv_obj_scroll_to_view_recursive(_target, LV_ANIM_OFF);
    /* 移动构件 */
    lv_area_t text_area, kb_area;
    lv_obj_get_coords(_target, &text_area);
    lv_obj_get_coords(_keyboard, &kb_area);
    int offset = text_area.y1 + 50 - kb_area.y1;
    /* 保存原始坐标 */
    lv_area_t wind_area;
    lv_obj_get_coords(_parent_ant, &wind_area);
    wind_pre_area = wind_area;
    if (offset > 0) {
        uint32_t x = wind_area.x1;
        uint32_t y = wind_area.y1 - offset;
        lv_obj_move_to(_parent_ant, x, y);
    }
}

void Keyboard::hind(){
    /* 恢复原始坐标 */
    lv_obj_move_to(_parent_ant, wind_pre_area.x1, wind_pre_area.y1);
    lv_keyboard_set_textarea(_keyboard, NULL);
    lv_obj_add_flag(_keyboard, LV_OBJ_FLAG_HIDDEN);
}

void Keyboard::keyboard_event_cb(lv_event_t* e){
    static lv_area_t wind_pre_area;
    Keyboard* keyboard = (Keyboard*)lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);
    keyboard->_target = lv_event_get_target(e);
    /* 获得顶层父节点 */
    lv_obj_t* parent = keyboard->_target;
    keyboard->_parent_ant = keyboard->_target;
    for (;parent != lv_layer_top() && parent != lv_scr_act();) {
        parent = lv_obj_get_parent(keyboard->_parent_ant);
        if (parent != lv_layer_top() && parent != lv_scr_act()) {
            keyboard->_parent_ant = parent;
        }
    }
    
    if (code == LV_EVENT_FOCUSED) {
        keyboard->show();
    }
    else if (code == LV_EVENT_DEFOCUSED) {
        keyboard->hind();
    }
    else if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
        keyboard->hind();
        lv_obj_clear_state(keyboard->_target, LV_STATE_FOCUSED);
    }
}

void keyboard_event_cb(lv_event_t* e){
    static lv_area_t wind_pre_area;
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* ta = lv_event_get_target(e);
    lv_obj_t* keyboard = (lv_obj_t*)lv_event_get_user_data(e);
    /* 获得顶层父节点 */
    lv_obj_t* parent = ta, * current = ta;
    for (;parent != lv_layer_top();) {
        parent = lv_obj_get_parent(current);
        if (parent != lv_layer_top()) {
            current = parent;
        }
    }
    
    if (code == LV_EVENT_FOCUSED) {
        lv_keyboard_set_textarea(keyboard, ta);
        lv_obj_set_style_max_height(keyboard, LV_HOR_RES * 2 / 1, 0);
        lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
        lv_obj_scroll_to_view_recursive(ta, LV_ANIM_OFF);
        /* 移动构件 */
        lv_area_t text_area, kb_area;
        lv_obj_get_coords(ta, &text_area);
        lv_obj_get_coords(keyboard, &kb_area);
        int offset = text_area.y1 + 50 - kb_area.y1;
        /* 保存原始坐标 */
        lv_area_t wind_area;
        lv_obj_get_coords(current, &wind_area);
        wind_pre_area = wind_area;
        if (offset > 0) {
            uint32_t x = wind_area.x1;
            uint32_t y = wind_area.y1 - offset;
            lv_obj_move_to(current, x, y);
        }
    }
    else if (code == LV_EVENT_DEFOCUSED) {
        /* 恢复原始坐标 */
        lv_obj_move_to(current, wind_pre_area.x1, wind_pre_area.y1);
        lv_keyboard_set_textarea(keyboard, NULL);
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    }
    else if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
        lv_obj_move_to(current, wind_pre_area.x1, wind_pre_area.y1);
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
        lv_keyboard_set_textarea(keyboard, NULL);
        lv_obj_clear_state(ta, LV_STATE_FOCUSED);
    }
}
/*=================Keyboard==================*/

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

/*=================CameraSettingWind==================*/

CameraSettingWind::CameraSettingWind(Camera* camera):_pCamera(camera), _keyboard(Keyboard::getInstance()){
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
    lv_obj_add_event_cb(ip_text, Keyboard::keyboard_event_cb, LV_EVENT_ALL, _keyboard);

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
    lv_obj_add_event_cb(port_text, Keyboard::keyboard_event_cb, LV_EVENT_ALL, _keyboard);

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
    lv_obj_add_event_cb(account_text, Keyboard::keyboard_event_cb, LV_EVENT_ALL, _keyboard);

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
    lv_obj_add_event_cb(pwd_text, Keyboard::keyboard_event_cb, LV_EVENT_ALL, _keyboard);

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

/*==================CameraSettingWind=====================*/

/*==================WIFISettingWind=======================*/

WIFISettingWind::WIFISettingWind(Camera* left_camera, Camera* right_camera)
:_pWiFiManager(getWiFiManagerInstance()), 
_keyboard(Keyboard::getInstance()),
_pLefgCamera(left_camera),
_pRightCamera(right_camera){
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
    lv_obj_add_event_cb(wifi_pwd, Keyboard::keyboard_event_cb, LV_EVENT_ALL, _keyboard);

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


void WIFISettingWind::hind(){
    lv_obj_clear_flag(lv_layer_sys(), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_TRANSP, 0);
    lv_obj_add_flag(setting_window, LV_OBJ_FLAG_HIDDEN);
}

void WIFISettingWind::show(){
    lv_obj_set_style_bg_color(lv_layer_top(), lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_50, 0);
    lv_obj_add_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(setting_window, LV_OBJ_FLAG_HIDDEN);
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
            //setting_window->_pMessageWind.create_without_btn_wind(lv_layer_top(), "正在连接wifi...");
            setting_window->_pLefgCamera->camera_stop();
            setting_window->_pRightCamera->camera_stop();
            lv_dropdown_get_selected_str(setting_window->_wifiList, buf, sizeof(buf));
            std::string wifiName = buf;
            std::string wifiPwd = lv_textarea_get_text(setting_window->wifi_pwd);
            if(setting_window->_pWiFiManager->connect(wifiName, wifiPwd)){
                setting_window->hind();
            }
        }
        else if(id == 1){   //取消
            setting_window->hind();
        }
    }
}

WifiManager* WIFISettingWind::getWiFiManagerInstance(){
    static WifiManager manager(INTER_WLAN);
    return &manager;
}
/*==================WIFISettingWind=====================*/

/*===================LinkSettingWind====================*/


LinkSettingWind::LinkSettingWind():
_keyboard(Keyboard::getInstance()),
_pPLCManager(LinkSettingWind::getPLCManagerInstance()){
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
    lv_obj_add_event_cb(plc_ip, Keyboard::keyboard_event_cb, LV_EVENT_ALL, _keyboard);
    /* 按键栏 */
    static const char* map[] = { "保存", "取消", "" };
    lv_obj_t* btnmx = lv_btnmatrix_create(button_widget);
    lv_btnmatrix_set_map(btnmx, map);
    lv_obj_set_style_pad_all(btnmx, 0, 0);
    lv_obj_set_scrollbar_mode(btnmx, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(btnmx, lv_pct(100), lv_pct(100));
    lv_obj_set_style_text_font(btnmx, &lv_font_cn_songti_medium_21_media_cfg, 0);
    lv_obj_add_event_cb(btnmx, setting_btnmatrix_event_cb, LV_EVENT_VALUE_CHANGED, this);

    if(_pPLCManager->read_config()){
        lv_textarea_set_text(plc_ip, _pPLCManager->ipaddr.c_str());
    }

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
        _pPLCManager->ipaddr = ipstr;
        _pPLCManager->port = 5551;
        LOG_DEBUG << "connecting " << ipstr << ":5551";
        _pPLCManager->connect();
    }
}

bool LinkSettingWind::plc_reconnect(){
    std::string ipstr = lv_textarea_get_text(plc_ip);
    if(ipstr.length() > 0){
        _pPLCManager->ipaddr = ipstr;
        _pPLCManager->port = 5551;
        LOG_DEBUG << "connecting " << ipstr << ":5551";
        _pPLCManager->write_config();
        _pPLCManager->reconnect();
    }
}

PLCManager* LinkSettingWind::getPLCManagerInstance(){
    static PLCManager manager;
    return &manager;
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
            linking_window->_pPLCManager->ipaddr = ip;
            linking_window->_pPLCManager->port = 5551;
            linking_window->_pPLCManager->write_config();
            linking_window->_pPLCManager->reconnect();
            linking_window->hind();
        }
        else if(id == 1){   //取消
            linking_window->hind();
        }
    }
}
/*=================LinkSettingWind=====================*/

ParamKeyboard* ParamKeyboard::getInstance(){
    static ParamKeyboard keyboard;
    return &keyboard;
}

void ParamKeyboard::keyboard_event_cb(lv_event_t* e){
    static lv_area_t wind_pre_area;         
    ParamSettingWind* pWind = (ParamSettingWind*)lv_event_get_user_data(e);
    ParamKeyboard* keyboard = pWind->_keybaord;
    lv_event_code_t code = lv_event_get_code(e);
    keyboard->_target = lv_event_get_target(e);
    /* 获得顶层父节点 */
    lv_obj_t* parent = keyboard->_target;
    keyboard->_parent_ant = keyboard->_target;
    for (;parent != lv_layer_top() && parent != lv_scr_act();) {
        parent = lv_obj_get_parent(keyboard->_parent_ant);
        if (parent != lv_layer_top() && parent != lv_scr_act()) {
            keyboard->_parent_ant = parent;
        }
    }
    
    if (code == LV_EVENT_FOCUSED) {
        keyboard->show();
    }
    else if(code == LV_EVENT_CANCEL){
        keyboard->hind();
        lv_obj_clear_state(keyboard->_target, LV_STATE_FOCUSED);
    }
    else if (code == LV_EVENT_READY) {
        pWind->write_value(keyboard->_target);
        keyboard->hind();
        lv_obj_clear_state(keyboard->_target, LV_STATE_FOCUSED);
    }
}

/*=================Param1SettingWind====================*/

void calculate_nothing(void* value, void* target){
    return;
}

void ifmt_write_1(void* value, void* target){
    bool* pValue = reinterpret_cast<bool*>(value);
    *pValue = true;
    return;
}


template <class T>
void ifmt_from_textarea(void* pValue, void* pTaget){
    lv_obj_t* target = reinterpret_cast<lv_obj_t*>(pTaget);
    T* value = reinterpret_cast<T*>(pValue);
    std::stringstream ss(lv_textarea_get_text(target));
    ss >> *value;
}

template<class T>
void ifmt_from_textarea_multip_1000(void* pValue, void* pTarget){
    ifmt_from_textarea<T>(pValue, pTarget);
    T* value = reinterpret_cast<T*>(pValue);
    *value *= 1000;
}

template<class T>
void ifmt_from_textarea_multip_10(void* pValue, void* pTarget){
    ifmt_from_textarea<T>(pValue, pTarget);
    T* value = reinterpret_cast<T*>(pValue);
    *value *= 10;
}

void ifmt_matrial_box(void* pValue, void* pTarget){
    lv_obj_t* dropdown = reinterpret_cast<lv_obj_t*>(pTarget);
    uint16_t* value = reinterpret_cast<uint16_t*>(pValue);
    *value = lv_dropdown_get_selected(dropdown)+1;
}

void ifmt_btns(void* pValue, void* pTarget){
    ModelSettingWind* pWind = reinterpret_cast<ModelSettingWind*>(pTarget);
    uint16_t* value = reinterpret_cast<uint16_t*>(pValue);
    *value = (uint16_t)pWind->_cur_model;
}

void ofmt_nothing(void* pValue, void* pTextarea, void* pPrefix){
    return;
}

template<class T>
void ofmt_set_textarea(void* pValue, void* pTextarea, void* pPrefix){
    std::stringstream ss;
    T value = *reinterpret_cast<T*>(pValue);
    std::string prefix = reinterpret_cast<char*>(pPrefix);
    ss << prefix << value;
    pthread_mutex_lock(&lv_lock);
    lv_textarea_set_text(reinterpret_cast<lv_obj_t*>(pTextarea), ss.str().c_str());
    pthread_mutex_unlock(&lv_lock);
}

template<class T>
void ofmt_set_textarea_devide_1000(void* pValue, void* pTextarea, void* pPrefix){
    T* value = reinterpret_cast<T*>(pValue);
    *value /= 1000;
    ofmt_set_textarea<T>(pValue, pTextarea, pPrefix);
}

template<class T>
void ofmt_set_textarea_divide_10(void* pValue, void* pTextarea, void* pPrefix){
    T* value = reinterpret_cast<T*>(pValue);
    *value /= 10;
    ofmt_set_textarea<T>(pValue, pTextarea, pPrefix);
}

void ofmt_set_bool(void* pValue, void* pTextarea, void* pPrefix){
    bool value = *reinterpret_cast<bool*>(pValue);
    lv_obj_t* img = reinterpret_cast<lv_obj_t*>(pTextarea);
    if(value){
        pthread_mutex_lock(&lv_lock);
        lv_img_set_src(img, &green_point);
        pthread_mutex_unlock(&lv_lock);
    }
    else{
        pthread_mutex_lock(&lv_lock);
        lv_img_set_src(img, &red_point);
        pthread_mutex_unlock(&lv_lock);
    }
}

void ofmt_set_running_info(void* pValue, void* pTextarea, void* preg){
    bool value = *reinterpret_cast<bool*>(pValue);
    MainWind* pMainWind = reinterpret_cast<MainWind*>(pTextarea);
    if(value){
        std::string reg = reinterpret_cast<char*>(preg);
        auto item_ptr = FAULT_MESSAGE.find(reg);
        if(item_ptr == FAULT_MESSAGE.end())
            return;
        pthread_mutex_lock(&lv_lock);
        lv_textarea_set_text(pMainWind->running_info, item_ptr->second.c_str());
        pthread_mutex_unlock(&lv_lock);
        //记录故障
        for(auto iter = FAULT_RECORD_REG.begin(); 
            iter != FAULT_RECORD_REG.end(); iter++){
            if(reg == *iter){
                pMainWind->_pFultRecordWind->insert_record(toString(static_cast<uint32_t>(time(NULL))), item_ptr->second);
                break;
            }
        }
    }
}

void ofmt_work_record(void* pValue, void* pWind, void* pReg){
    bool value = *reinterpret_cast<bool*>(pValue);
    MainWind* pMainWind = reinterpret_cast<MainWind*>(pWind);
    if(value){
        auto sstamp = toString(static_cast<uint32_t>(time(NULL)));
        std::string sarg1 = lv_textarea_get_text(pMainWind->row1_ptr_list[1]);
        std::string sarg2 = lv_textarea_get_text(pMainWind->row1_ptr_list[0]);
        std::string sarg3 = lv_textarea_get_text(pMainWind->row0_ptr_list[0]);
        std::string sarg4 = lv_textarea_get_text(pMainWind->row0_ptr_list[1]);
        std::string sarg5 = lv_textarea_get_text(pMainWind->row0_ptr_list[2]);
        pMainWind->_pWorkRecordWind->insert_record(sstamp, 
                                    sarg1, sarg2, sarg3,
                                    sarg4, sarg5);
    }
}

void ofmt_set_keyboard(void* pValue, void* target, void* pint){
    bool value = *reinterpret_cast<bool*>(pValue);
    PhyKeyboard* pKeyboard = reinterpret_cast<PhyKeyboard*>(target);
    int index = reinterpret_cast<int>(pint);
    auto item_ptr = KEY_LED_PLC_REG_MAP.find(index);
    if(item_ptr == KEY_LED_PLC_REG_MAP.end())
        return;
    pKeyboard->keyboard_led_turn(index, value);
}

void ofmt_matrial_box(void* pValue, void* pTarget, void* pPrefix){
    uint16_t fValue = *reinterpret_cast<uint16_t*>(pValue);
    lv_obj_t* dropdown = reinterpret_cast<lv_obj_t*>(pTarget);
    lv_dropdown_set_selected(dropdown, fValue-1);
}

void ofmt_btns(void* pValue, void* pTarget, void* pPrefix){
    uint16_t fValue = *reinterpret_cast<uint16_t*>(pValue);
    ModelSettingWind* pWind = reinterpret_cast<ModelSettingWind*>(pTarget);
    pWind->switch_model((ModelSettingWind::model_t)fValue);
}

ParamSettingWind::ParamSettingWind()
:_keybaord(ParamKeyboard::getInstance()),
_pPLCManager(LinkSettingWind::getPLCManagerInstance())
{

}

void ParamSettingWind::write_value(void* ui_domain){
    if(ui_domain == NULL)return;
    if(_pPLCManager->is_connected()){
        auto ptr = _tasks.find((uint32_t)ui_domain);
        if(ptr != _tasks.end()){
            WindTask task(ptr->second);
            task.is_read = false;
            tempWindTask.register_task(ptr->first, task);
        }
        read_value();
    }
}

void ParamSettingWind::read_value(){
    if(_pPLCManager->is_connected()){
        for(auto ptr = _tasks.begin(); ptr != _tasks.end();ptr++){
            WindTask task(ptr->second);
            task.is_read = true;
            tempWindTask.register_task(ptr->first, task);
        }
    }
}

#if 0
void ParamSettingWind::write_float_value(lv_obj_t* text_domain, 
                                    const std::string & addr,
                                    std::function<void(void*)> calculate_func){
    if(text_domain == NULL) return;
    std::string text = lv_textarea_get_text(text_domain);
    std::stringstream ss(text);
    float fData;
    ss >> fData;
    calculate_func(&fData);
    if(_pPLCManager->is_connected()){
        pthread_mutex_lock(&_pPLCManager->rw_lock);
        _pPLCManager->write_float(addr, fData);
        pthread_mutex_unlock(&_pPLCManager->rw_lock);
    }
    LOG_DEBUG << "write float " << addr << ": " << fData;
}

void ParamSettingWind::write_uint32_value(lv_obj_t* text_domain, 
                    const std::string & addr,
                    std::function<void(void*)> calculate_func){
    if(text_domain == NULL) return;
    std::string text = lv_textarea_get_text(text_domain);
    std::stringstream ss(text);
    uint32 iData;
    ss >> iData;
    calculate_func(&iData);
    if(_pPLCManager->is_connected()){
        pthread_mutex_lock(&_pPLCManager->rw_lock);
        _pPLCManager->write_uint32(addr, iData);
        pthread_mutex_unlock(&_pPLCManager->rw_lock);
    }
    LOG_DEBUG << "write uint32 " << addr << ": " << iData;
}

void ParamSettingWind::write_uint16_value(lv_obj_t* text_domain,
                    const std::string & addr,
                    std::function<void(void*)> calculate_func){
    if(text_domain == NULL) return;
    std::string text = lv_textarea_get_text(text_domain);
    std::stringstream ss(text);
    uint16_t iData;
    ss >> iData;
    calculate_func(&iData);
    if(_pPLCManager->is_connected()){
        pthread_mutex_lock(&_pPLCManager->rw_lock);
        _pPLCManager->write_ushort(addr, iData);
        pthread_mutex_unlock(&_pPLCManager->rw_lock);
    }
    LOG_DEBUG << "write ushort " << addr << ": " << iData;
}

void ParamSettingWind::write_value(lv_obj_t* text_domain){
    if(text_domain == NULL)return;
    for(auto iter = _write_item.begin(); 
        iter != _write_item.end(); iter++){
        if(iter->text_domain == text_domain){
            if(iter->reg_type == REG_TYPE_FLOAT){
                write_float_value(iter->text_domain, iter->address, 
                                    iter->wcalculate_func);
            }
            else if(iter->reg_type == REG_TYPE_UINT32){
                write_uint32_value(iter->text_domain, iter->address,
                                    iter->wcalculate_func);
            }
            else if(iter->reg_type == REG_TYPE_UINT16){
                write_uint16_value(iter->text_domain, iter->address,
                                    iter->wcalculate_func);
            }
            break;
        }
    }
    read_value();
}

void ParamSettingWind::read_value(){
    if(_pPLCManager->is_connected()){
        pthread_mutex_lock(&lv_lock);
        pthread_mutex_lock(&_pPLCManager->rw_lock);
        for(auto iter = _write_item.begin(); 
            iter != _write_item.end(); iter++)
        {
            std::string text;
            if(iter->reg_type == REG_TYPE_FLOAT){
                float read_val = 0;
                if(_pPLCManager->read_float(iter->address, &read_val) == 0){
                    LOG_DEBUG << "read float" << iter->address << " value: " << read_val;
                    text = iter->ofmt(&read_val, iter->prefix);
                }
            }
            else if(iter->reg_type == REG_TYPE_UINT32){
                uint32 read_val = 0;
                if(_pPLCManager->read_uint32(iter->address, &read_val) == 0){
                    LOG_DEBUG << "read uint" << iter->address << " value: " << read_val;
                    text = iter->ofmt(&read_val, iter->prefix);
                }
            }
            else if(iter->reg_type == REG_TYPE_UINT16){
                uint16_t read_val;
                if(_pPLCManager->read_ushort(iter->address, &read_val) == 0){
                    LOG_DEBUG << "read ushort" << iter->address << " value: " << read_val;
                    text = iter->ofmt(&read_val, iter->prefix);
                }
            }
            lv_textarea_set_text(iter->text_domain, text.c_str());
        }
        pthread_mutex_unlock(&lv_lock);
        pthread_mutex_unlock(&_pPLCManager->rw_lock);
    }
}
#endif

Param1SettingWind* Param1SettingWind::getInstance(lv_obj_t* parent){
    static Param1SettingWind pWind(parent);
    return &pWind;
}

Param1SettingWind::Param1SettingWind(lv_obj_t* parent)
{
    ui = new lv_ui;
    //Write codes screen
    ui->screen = parent;
    //Write codes screen_background
    ui->screen_background = parent;
    ui->screen_background_tile_1 = lv_tileview_add_tile(ui->screen_background, 0, 0, LV_DIR_RIGHT);
    lv_obj_set_style_bg_color(ui->screen_background, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    /*lv_obj_set_pos(ui->screen_background, 0, 0);
    lv_obj_set_size(ui->screen_background, 1024, 768-50-5);
    lv_obj_set_scrollbar_mode(ui->screen_background, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_background, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_background, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_background, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_background, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_background, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_background, lv_color_hex(0xeaeff3), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_background, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_background, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    */

    //Write codes screen_param_title_label
    ui->screen_param_title_label = lv_label_create(ui->screen_background_tile_1);
    lv_label_set_text(ui->screen_param_title_label, "行走电机参数设定");
    lv_label_set_long_mode(ui->screen_param_title_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_param_title_label, 4, 50);
    lv_obj_set_size(ui->screen_param_title_label, 263, 26);

    //Write style for screen_param_title_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_param_title_label, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_param_title_label, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_param_title_label, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_param_title_label, &lv_font_cn_songti_bold_25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_param_title_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_param_title_label, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_param_title_label, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_param_title_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_param_title_label, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_param_title_label, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_param_title_label, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_param_title_label, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_param_title_label, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_param_title_label, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_line_1
    ui->screen_line_1 = lv_line_create(ui->screen_background_tile_1);
    static lv_point_t screen_line_1[] = { {0, 0},{950, 0}, };
    lv_line_set_points(ui->screen_line_1, screen_line_1, 2);
    lv_obj_set_pos(ui->screen_line_1, 27, 80);
    lv_obj_set_size(ui->screen_line_1, 945, 4);

    //Write style for screen_line_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->screen_line_1, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->screen_line_1, lv_color_hex(0x11e652), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->screen_line_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->screen_line_1, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_tileview_1
    ui->screen_tileview_1 = lv_tileview_create(ui->screen_background_tile_1);
    ui->screen_tileview_1_tile_2 = lv_tileview_add_tile(ui->screen_tileview_1, 0, 0, LV_DIR_RIGHT);
    lv_obj_set_pos(ui->screen_tileview_1, 29, 91);
    lv_obj_set_size(ui->screen_tileview_1, 949, 270);
    lv_obj_set_scrollbar_mode(ui->screen_tileview_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_tileview_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_tileview_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_tileview_1, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_tileview_1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_tileview_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_tileview_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_tileview_1, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_tileview_1, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_tileview_1, lv_color_hex(0xeaeff3), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_tileview_1, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_tileview_1, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);



    //Write codes screen_cont_1
    ui->screen_cont_1 = lv_obj_create(ui->screen_tileview_1_tile_2);
    lv_obj_set_pos(ui->screen_cont_1, 35, 4);
    lv_obj_set_size(ui->screen_cont_1, 330, 50);
    lv_obj_set_scrollbar_mode(ui->screen_cont_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_cont_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_cont_1, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_cont_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_cont_1, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_cont_1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_label_1
    ui->screen_label_1 = lv_label_create(ui->screen_cont_1);
    lv_label_set_text(ui->screen_label_1, "快速行走速度：");
    lv_label_set_long_mode(ui->screen_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_1, 0, 10);
    lv_obj_set_size(ui->screen_label_1, 173, 23);

    //Write style for screen_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_1, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_fast_mv_spd
    ui->screen_fast_mv_spd = lv_textarea_create(ui->screen_cont_1);
    lv_obj_clear_flag(ui->screen_fast_mv_spd, LV_OBJ_FLAG_SCROLLABLE);
    lv_textarea_set_placeholder_text(ui->screen_fast_mv_spd, "");
    //lv_textarea_set_password_bullet(ui->screen_fast_mv_spd, "*");
    lv_textarea_set_password_mode(ui->screen_fast_mv_spd, false);
    lv_textarea_set_one_line(ui->screen_fast_mv_spd, true);
    lv_textarea_set_accepted_chars(ui->screen_fast_mv_spd, "1234567890.");
    lv_textarea_set_max_length(ui->screen_fast_mv_spd, 45);
    lv_obj_set_pos(ui->screen_fast_mv_spd, 165, 1);
    lv_obj_set_size(ui->screen_fast_mv_spd, 160, 45);
    lv_obj_add_flag(ui->screen_fast_mv_spd, LV_OBJ_FLAG_SCROLLABLE);

    //Write style for screen_fast_mv_spd, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_fast_mv_spd, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_fast_mv_spd, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_fast_mv_spd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_fast_mv_spd, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_fast_mv_spd, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_fast_mv_spd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_fast_mv_spd, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_fast_mv_spd, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_fast_mv_spd, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_fast_mv_spd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_fast_mv_spd, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_fast_mv_spd, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_fast_mv_spd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_fast_mv_spd, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_fast_mv_spd, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_fast_mv_spd, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_fast_mv_spd, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_fast_mv_spd, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_fast_mv_spd, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_fast_mv_spd, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_fast_mv_spd, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_fast_mv_spd, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_label_2
    ui->screen_label_2 = lv_label_create(ui->screen_cont_1);
    lv_label_set_text(ui->screen_label_2, "%");
    lv_label_set_long_mode(ui->screen_label_2, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_2, 290, 9);
    lv_obj_set_size(ui->screen_label_2, 26, 20);

    //Write style for screen_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_2, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_2, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_cont_2
    ui->screen_cont_2 = lv_obj_create(ui->screen_tileview_1_tile_2);
    lv_obj_set_pos(ui->screen_cont_2, 530, 4);
    lv_obj_set_size(ui->screen_cont_2, 330, 50);
    lv_obj_set_scrollbar_mode(ui->screen_cont_2, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_cont_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_cont_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_cont_2, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_cont_2, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_cont_2, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_cont_2, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_single_mmt_l
    ui->screen_single_mmt_l = lv_label_create(ui->screen_cont_2);
    lv_label_set_text(ui->screen_single_mmt_l, "单圈脉冲数：");
    lv_label_set_long_mode(ui->screen_single_mmt_l, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_single_mmt_l, 0, 10);
    lv_obj_set_size(ui->screen_single_mmt_l, 173, 23);

    //Write style for screen_single_mmt_l, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_single_mmt_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_single_mmt_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_single_mmt_l, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_single_mmt_l, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_single_mmt_l, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_single_mmt_l, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_single_mmt_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_single_mmt_l, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_single_mmt_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_single_mmt_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_single_mmt_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_single_mmt_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_single_mmt_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_single_mmt_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_single_mmt
    ui->screen_single_mmt = lv_textarea_create(ui->screen_cont_2);
    lv_obj_clear_flag(ui->screen_single_mmt, LV_OBJ_FLAG_SCROLLABLE);
    lv_textarea_set_placeholder_text(ui->screen_single_mmt, "");
    ////lv_textarea_set_password_bullet(ui->screen_single_mmt, "*");
    lv_textarea_set_password_mode(ui->screen_single_mmt, false);
    lv_textarea_set_one_line(ui->screen_single_mmt, true);
    lv_textarea_set_accepted_chars(ui->screen_single_mmt, "1234567890.");
    lv_textarea_set_max_length(ui->screen_single_mmt, 32);
    lv_obj_set_pos(ui->screen_single_mmt, 166, 2);
    lv_obj_set_size(ui->screen_single_mmt, 160, 45);

    //Write style for screen_single_mmt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_single_mmt, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_single_mmt, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_single_mmt, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_single_mmt, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_single_mmt, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_single_mmt, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_single_mmt, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_single_mmt, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_single_mmt, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_single_mmt, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_single_mmt, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_single_mmt, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_single_mmt, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_single_mmt, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_single_mmt, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_single_mmt, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_single_mmt, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_single_mmt, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_single_mmt, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_single_mmt, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_single_mmt, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_single_mmt, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_cont_3
    ui->screen_cont_3 = lv_obj_create(ui->screen_tileview_1_tile_2);
    lv_obj_set_pos(ui->screen_cont_3, 35, 63);
    lv_obj_set_size(ui->screen_cont_3, 330, 50);
    lv_obj_set_scrollbar_mode(ui->screen_cont_3, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_cont_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_cont_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_cont_3, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_cont_3, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_cont_3, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_cont_3, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_slow_move_speed_
    ui->screen_slow_move_speed_ = lv_label_create(ui->screen_cont_3);
    lv_label_set_text(ui->screen_slow_move_speed_, "慢速行走速度：");
    lv_label_set_long_mode(ui->screen_slow_move_speed_, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_slow_move_speed_, 0, 10);
    lv_obj_set_size(ui->screen_slow_move_speed_, 173, 23);

    //Write style for screen_slow_move_speed_, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_slow_move_speed_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_slow_move_speed_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_slow_move_speed_, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_slow_move_speed_, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_slow_move_speed_, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_slow_move_speed_, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_slow_move_speed_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_slow_move_speed_, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_slow_move_speed_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_slow_move_speed_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_slow_move_speed_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_slow_move_speed_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_slow_move_speed_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_slow_move_speed_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_slow_mv_psd
    ui->screen_slow_mv_psd = lv_textarea_create(ui->screen_cont_3);
    lv_obj_clear_flag(ui->screen_slow_mv_psd, LV_OBJ_FLAG_SCROLLABLE);
    lv_textarea_set_placeholder_text(ui->screen_slow_mv_psd, "");
    //lv_textarea_set_password_bullet(ui->screen_slow_mv_psd, "*");
    lv_textarea_set_password_mode(ui->screen_slow_mv_psd, false);
    lv_textarea_set_one_line(ui->screen_slow_mv_psd, true);
    lv_textarea_set_accepted_chars(ui->screen_slow_mv_psd, "1234567890.");
    lv_textarea_set_max_length(ui->screen_slow_mv_psd, 32);
    lv_obj_set_pos(ui->screen_slow_mv_psd, 165, 1);
    lv_obj_set_size(ui->screen_slow_mv_psd, 160, 45);

    //Write style for screen_slow_mv_psd, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_slow_mv_psd, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_slow_mv_psd, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_slow_mv_psd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_slow_mv_psd, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_slow_mv_psd, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_slow_mv_psd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_slow_mv_psd, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_slow_mv_psd, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_slow_mv_psd, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_slow_mv_psd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_slow_mv_psd, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_slow_mv_psd, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_slow_mv_psd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_slow_mv_psd, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_slow_mv_psd, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_slow_mv_psd, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_slow_mv_psd, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_slow_mv_psd, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_slow_mv_psd, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_slow_mv_psd, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_slow_mv_psd, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_slow_mv_psd, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_label_5
    ui->screen_label_5 = lv_label_create(ui->screen_cont_3);
    lv_label_set_text(ui->screen_label_5, "%");
    lv_label_set_long_mode(ui->screen_label_5, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_5, 290, 9);
    lv_obj_set_size(ui->screen_label_5, 26, 20);

    //Write style for screen_label_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_5, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_5, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_5, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_cont_4
    ui->screen_cont_4 = lv_obj_create(ui->screen_tileview_1_tile_2);
    lv_obj_set_pos(ui->screen_cont_4, 530, 64);
    lv_obj_set_size(ui->screen_cont_4, 360, 50);
    lv_obj_set_scrollbar_mode(ui->screen_cont_4, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_cont_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_cont_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_cont_4, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_cont_4, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_cont_4, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_cont_4, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_move_alt_spd_l
    ui->screen_move_alt_spd_l = lv_label_create(ui->screen_cont_4);
    lv_label_set_text(ui->screen_move_alt_spd_l, "行走变速比：");
    lv_label_set_long_mode(ui->screen_move_alt_spd_l, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_move_alt_spd_l, 0, 10);
    lv_obj_set_size(ui->screen_move_alt_spd_l, 173, 23);

    //Write style for screen_move_alt_spd_l, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_move_alt_spd_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_move_alt_spd_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_move_alt_spd_l, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_move_alt_spd_l, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_move_alt_spd_l, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_move_alt_spd_l, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_move_alt_spd_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_move_alt_spd_l, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_move_alt_spd_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_move_alt_spd_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_move_alt_spd_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_move_alt_spd_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_move_alt_spd_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_move_alt_spd_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_mv_alt_spd_1
    ui->screen_mv_alt_spd_1 = lv_textarea_create(ui->screen_cont_4);
    lv_obj_clear_flag(ui->screen_mv_alt_spd_1, LV_OBJ_FLAG_SCROLLABLE);
    lv_textarea_set_placeholder_text(ui->screen_mv_alt_spd_1, "");
    //lv_textarea_set_password_bullet(ui->screen_mv_alt_spd_1, "*");
    lv_textarea_set_password_mode(ui->screen_mv_alt_spd_1, false);
    lv_textarea_set_one_line(ui->screen_mv_alt_spd_1, true);
    lv_textarea_set_accepted_chars(ui->screen_mv_alt_spd_1, "1234567890");
    lv_textarea_set_max_length(ui->screen_mv_alt_spd_1, 32);
    lv_obj_set_pos(ui->screen_mv_alt_spd_1, 165, 1);
    lv_obj_set_size(ui->screen_mv_alt_spd_1, 70, 45);

    //Write style for screen_mv_alt_spd_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_mv_alt_spd_1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_mv_alt_spd_1, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_mv_alt_spd_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_mv_alt_spd_1, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_mv_alt_spd_1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_mv_alt_spd_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_mv_alt_spd_1, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_mv_alt_spd_1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_mv_alt_spd_1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_mv_alt_spd_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_mv_alt_spd_1, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_mv_alt_spd_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_mv_alt_spd_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_mv_alt_spd_1, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_mv_alt_spd_1, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_mv_alt_spd_1, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_mv_alt_spd_1, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_mv_alt_spd_1, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_mv_alt_spd_1, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_mv_alt_spd_1, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_mv_alt_spd_1, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_mv_alt_spd_1, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_label_7
    ui->screen_label_7 = lv_label_create(ui->screen_cont_4);
    lv_label_set_text(ui->screen_label_7, "/");
    lv_label_set_long_mode(ui->screen_label_7, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_7, 240, 8);
    lv_obj_set_size(ui->screen_label_7, 16, 20);

    //Write style for screen_label_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_7, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_7, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_7, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_mv_alt_spd_2
    ui->screen_mv_alt_spd_2 = lv_textarea_create(ui->screen_cont_4);
    lv_obj_clear_flag(ui->screen_mv_alt_spd_2, LV_OBJ_FLAG_SCROLLABLE);
    lv_textarea_set_placeholder_text(ui->screen_mv_alt_spd_2, "");
    //lv_textarea_set_password_bullet(ui->screen_mv_alt_spd_2, "*");
    lv_textarea_set_password_mode(ui->screen_mv_alt_spd_2, false);
    lv_textarea_set_one_line(ui->screen_mv_alt_spd_2, true);
    lv_textarea_set_accepted_chars(ui->screen_mv_alt_spd_2, "1234567890");
    lv_textarea_set_max_length(ui->screen_mv_alt_spd_2, 32);
    lv_obj_set_pos(ui->screen_mv_alt_spd_2, 265, 2);
    lv_obj_set_size(ui->screen_mv_alt_spd_2, 70, 45);

    //Write style for screen_mv_alt_spd_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_mv_alt_spd_2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_mv_alt_spd_2, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_mv_alt_spd_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_mv_alt_spd_2, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_mv_alt_spd_2, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_mv_alt_spd_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_mv_alt_spd_2, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_mv_alt_spd_2, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_mv_alt_spd_2, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_mv_alt_spd_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_mv_alt_spd_2, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_mv_alt_spd_2, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_mv_alt_spd_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_mv_alt_spd_2, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_mv_alt_spd_2, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_mv_alt_spd_2, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_mv_alt_spd_2, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_mv_alt_spd_2, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_mv_alt_spd_2, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_mv_alt_spd_2, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_mv_alt_spd_2, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_mv_alt_spd_2, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_cont_5
    ui->screen_cont_5 = lv_obj_create(ui->screen_tileview_1_tile_2);
    lv_obj_set_pos(ui->screen_cont_5, 35, 124);
    lv_obj_set_size(ui->screen_cont_5, 330, 50);
    lv_obj_set_scrollbar_mode(ui->screen_cont_5, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_cont_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_cont_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_cont_5, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_cont_5, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_cont_5, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_cont_5, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_fast_move_l
    ui->screen_fast_move_l = lv_label_create(ui->screen_cont_5);
    lv_label_set_text(ui->screen_fast_move_l, "行走速度系数：");
    lv_label_set_long_mode(ui->screen_fast_move_l, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_fast_move_l, 0, 10);
    lv_obj_set_size(ui->screen_fast_move_l, 173, 23);

    //Write style for screen_fast_move_l, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_fast_move_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_fast_move_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_fast_move_l, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_fast_move_l, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_fast_move_l, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_fast_move_l, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_fast_move_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_fast_move_l, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_fast_move_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_fast_move_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_fast_move_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_fast_move_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_fast_move_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_fast_move_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_fast_mv_spd_scl
    ui->screen_fast_mv_spd_scl = lv_textarea_create(ui->screen_cont_5);
    lv_obj_clear_flag(ui->screen_fast_mv_spd_scl, LV_OBJ_FLAG_SCROLLABLE);
    lv_textarea_set_placeholder_text(ui->screen_fast_mv_spd_scl, "");
    //lv_textarea_set_password_bullet(ui->screen_fast_mv_spd_scl, "*");
    lv_textarea_set_password_mode(ui->screen_fast_mv_spd_scl, false);
    lv_textarea_set_one_line(ui->screen_fast_mv_spd_scl, true);
    lv_textarea_set_accepted_chars(ui->screen_fast_mv_spd_scl, "1234567890.");
    lv_textarea_set_max_length(ui->screen_fast_mv_spd_scl, 32);
    lv_obj_set_pos(ui->screen_fast_mv_spd_scl, 165, 1);
    lv_obj_set_size(ui->screen_fast_mv_spd_scl, 160, 45);

    //Write style for screen_fast_mv_spd_scl, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_fast_mv_spd_scl, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_fast_mv_spd_scl, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_fast_mv_spd_scl, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_fast_mv_spd_scl, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_fast_mv_spd_scl, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_fast_mv_spd_scl, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_fast_mv_spd_scl, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_fast_mv_spd_scl, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_fast_mv_spd_scl, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_fast_mv_spd_scl, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_fast_mv_spd_scl, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_fast_mv_spd_scl, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_fast_mv_spd_scl, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_fast_mv_spd_scl, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_fast_mv_spd_scl, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_fast_mv_spd_scl, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_fast_mv_spd_scl, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_fast_mv_spd_scl, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_fast_mv_spd_scl, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_fast_mv_spd_scl, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_fast_mv_spd_scl, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_fast_mv_spd_scl, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_cont_6
    ui->screen_cont_6 = lv_obj_create(ui->screen_tileview_1_tile_2);
    lv_obj_set_pos(ui->screen_cont_6, 530, 124);
    lv_obj_set_size(ui->screen_cont_6, 330, 50);
    lv_obj_set_scrollbar_mode(ui->screen_cont_6, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_cont_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_cont_6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_cont_6, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_cont_6, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_cont_6, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_cont_6, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_label_12
    ui->screen_label_12 = lv_label_create(ui->screen_cont_6);
    lv_label_set_text(ui->screen_label_12, "行走轮内径：");
    lv_label_set_long_mode(ui->screen_label_12, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_12, 0, 10);
    lv_obj_set_size(ui->screen_label_12, 173, 23);

    //Write style for screen_label_12, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_12, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_12, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_12, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_12, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_12, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_mv_tl_in_len
    ui->screen_mv_tl_in_len = lv_textarea_create(ui->screen_cont_6);
    lv_obj_clear_flag(ui->screen_mv_tl_in_len, LV_OBJ_FLAG_SCROLLABLE);
    lv_textarea_set_placeholder_text(ui->screen_mv_tl_in_len, "");
    //lv_textarea_set_password_bullet(ui->screen_mv_tl_in_len, "*");
    lv_textarea_set_password_mode(ui->screen_mv_tl_in_len, false);
    lv_textarea_set_one_line(ui->screen_mv_tl_in_len, true);
    lv_textarea_set_accepted_chars(ui->screen_mv_tl_in_len, "1234567890.");
    lv_textarea_set_max_length(ui->screen_mv_tl_in_len, 5);
    lv_obj_set_pos(ui->screen_mv_tl_in_len, 165, 1);
    lv_obj_set_size(ui->screen_mv_tl_in_len, 160, 45);

    //Write style for screen_mv_tl_in_len, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_mv_tl_in_len, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_mv_tl_in_len, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_mv_tl_in_len, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_mv_tl_in_len, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_mv_tl_in_len, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_mv_tl_in_len, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_mv_tl_in_len, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_mv_tl_in_len, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_mv_tl_in_len, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_mv_tl_in_len, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_mv_tl_in_len, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_mv_tl_in_len, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_mv_tl_in_len, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_mv_tl_in_len, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_mv_tl_in_len, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_mv_tl_in_len, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_mv_tl_in_len, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_mv_tl_in_len, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_mv_tl_in_len, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_mv_tl_in_len, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_mv_tl_in_len, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_mv_tl_in_len, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_label_11
    ui->screen_label_11 = lv_label_create(ui->screen_cont_6);
    lv_label_set_text(ui->screen_label_11, "mm");
    lv_label_set_long_mode(ui->screen_label_11, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_11, 270, 9);
    lv_obj_set_size(ui->screen_label_11, 50, 20);

    //Write style for screen_label_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_11, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_11, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_11, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_11, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_cont_7
    ui->screen_cont_7 = lv_obj_create(ui->screen_tileview_1_tile_2);
    lv_obj_set_pos(ui->screen_cont_7, 35, 184);
    lv_obj_set_size(ui->screen_cont_7, 330, 50);
    lv_obj_set_scrollbar_mode(ui->screen_cont_7, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_cont_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_cont_7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_cont_7, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_cont_7, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_cont_7, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_cont_7, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_label_14
    ui->screen_label_14 = lv_label_create(ui->screen_cont_7);
    lv_label_set_text(ui->screen_label_14, "行走长度系数：");
    lv_label_set_long_mode(ui->screen_label_14, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_14, 0, 10);
    lv_obj_set_size(ui->screen_label_14, 173, 23);

    //Write style for screen_label_14, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_14, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_14, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_14, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_14, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_14, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_mv_len_scl
    ui->screen_mv_len_scl = lv_textarea_create(ui->screen_cont_7);
    lv_obj_clear_flag(ui->screen_mv_len_scl, LV_OBJ_FLAG_SCROLLABLE);
    lv_textarea_set_placeholder_text(ui->screen_mv_len_scl, "");
    //lv_textarea_set_password_bullet(ui->screen_mv_len_scl, "*");
    lv_textarea_set_password_mode(ui->screen_mv_len_scl, false);
    lv_textarea_set_one_line(ui->screen_mv_len_scl, true);
    lv_textarea_set_accepted_chars(ui->screen_mv_len_scl, "1234567890.");
    lv_textarea_set_max_length(ui->screen_mv_len_scl, 32);
    lv_obj_set_pos(ui->screen_mv_len_scl, 165, 1);
    lv_obj_set_size(ui->screen_mv_len_scl, 160, 45);

    //Write style for screen_mv_len_scl, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_mv_len_scl, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_mv_len_scl, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_mv_len_scl, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_mv_len_scl, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_mv_len_scl, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_mv_len_scl, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_mv_len_scl, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_mv_len_scl, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_mv_len_scl, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_mv_len_scl, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_mv_len_scl, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_mv_len_scl, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_mv_len_scl, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_mv_len_scl, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_mv_len_scl, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_mv_len_scl, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_mv_len_scl, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_mv_len_scl, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_mv_len_scl, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_mv_len_scl, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_mv_len_scl, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_mv_len_scl, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_cont_8
    ui->screen_cont_8 = lv_obj_create(ui->screen_tileview_1_tile_2);
    lv_obj_set_pos(ui->screen_cont_8, 530, 184);
    lv_obj_set_size(ui->screen_cont_8, 330, 50);
    lv_obj_set_scrollbar_mode(ui->screen_cont_8, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_cont_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_cont_8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_cont_8, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_cont_8, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_cont_8, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_cont_8, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_label_16
    ui->screen_label_16 = lv_label_create(ui->screen_cont_8);
    lv_label_set_text(ui->screen_label_16, "行走轮夹角：");
    lv_label_set_long_mode(ui->screen_label_16, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_16, 0, 6);
    lv_obj_set_size(ui->screen_label_16, 173, 23);

    //Write style for screen_label_16, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_16, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_16, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_16, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_16, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_16, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_mv_tl_ang
    ui->screen_mv_tl_ang = lv_textarea_create(ui->screen_cont_8);
    lv_obj_clear_flag(ui->screen_mv_tl_ang, LV_OBJ_FLAG_SCROLLABLE);
    lv_textarea_set_placeholder_text(ui->screen_mv_tl_ang, "");
    //lv_textarea_set_password_bullet(ui->screen_mv_tl_ang, "*");
    lv_textarea_set_password_mode(ui->screen_mv_tl_ang, false);
    lv_textarea_set_one_line(ui->screen_mv_tl_ang, true);
    lv_textarea_set_accepted_chars(ui->screen_mv_tl_ang, "1234567890.");
    lv_textarea_set_max_length(ui->screen_mv_tl_ang, 32);
    lv_obj_set_pos(ui->screen_mv_tl_ang, 165, 1);
    lv_obj_set_size(ui->screen_mv_tl_ang, 160, 45);

    //Write style for screen_mv_tl_ang, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_mv_tl_ang, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_mv_tl_ang, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_mv_tl_ang, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_mv_tl_ang, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_mv_tl_ang, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_mv_tl_ang, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_mv_tl_ang, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_mv_tl_ang, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_mv_tl_ang, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_mv_tl_ang, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_mv_tl_ang, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_mv_tl_ang, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_mv_tl_ang, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_mv_tl_ang, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_mv_tl_ang, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_mv_tl_ang, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_mv_tl_ang, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_mv_tl_ang, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_mv_tl_ang, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_mv_tl_ang, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_mv_tl_ang, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_mv_tl_ang, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_label_15
    ui->screen_label_15 = lv_label_create(ui->screen_cont_8);
    lv_label_set_text(ui->screen_label_15, "°");
    lv_label_set_long_mode(ui->screen_label_15, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_15, 290, 9);
    lv_obj_set_size(ui->screen_label_15, 26, 20);

    //Write style for screen_label_15, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_label_15, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_15, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_15, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_15, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_15, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_label_17
    ui->screen_label_17 = lv_label_create(ui->screen_background_tile_1);
    lv_label_set_text(ui->screen_label_17, "升降电机参数设定");
    lv_label_set_long_mode(ui->screen_label_17, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_17, 4, 404);
    lv_obj_set_size(ui->screen_label_17, 263, 26);

    //Write style for screen_label_17, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_17, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_17, &lv_font_cn_songti_bold_25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_17, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_17, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_17, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_line_2
    ui->screen_line_2 = lv_line_create(ui->screen_background_tile_1);
    static lv_point_t screen_line_2[] = { {0, 0},{950, 0}, };
    lv_line_set_points(ui->screen_line_2, screen_line_2, 2);
    lv_obj_set_pos(ui->screen_line_2, 27, 448);
    lv_obj_set_size(ui->screen_line_2, 945, 4);

    //Write style for screen_line_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->screen_line_2, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->screen_line_2, lv_color_hex(0x11e652), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->screen_line_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->screen_line_2, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_tileview_2
    ui->screen_tileview_2 = lv_tileview_create(ui->screen_background_tile_1);
    ui->screen_tileview_2_tile_2 = lv_tileview_add_tile(ui->screen_tileview_2, 0, 0, LV_DIR_RIGHT);
    lv_obj_set_pos(ui->screen_tileview_2, 28, 464);
    lv_obj_set_size(ui->screen_tileview_2, 949, 182);
    lv_obj_set_scrollbar_mode(ui->screen_tileview_2, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_tileview_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_tileview_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_tileview_2, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_tileview_2, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_tileview_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_tileview_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_tileview_2, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_tileview_2, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_tileview_2, lv_color_hex(0xeaeff3), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_tileview_2, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_tileview_2, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);


    //Write codes screen_cont_15
    ui->screen_cont_15 = lv_obj_create(ui->screen_tileview_2_tile_2);
    lv_obj_set_pos(ui->screen_cont_15, 35, 4);
    lv_obj_set_size(ui->screen_cont_15, 330, 50);
    lv_obj_set_scrollbar_mode(ui->screen_cont_15, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_cont_15, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_cont_15, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_cont_15, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_cont_15, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_15, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_cont_15, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_cont_15, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_label_28
    ui->screen_label_28 = lv_label_create(ui->screen_cont_15);
    lv_label_set_text(ui->screen_label_28, "快速上升速度：");
    lv_label_set_long_mode(ui->screen_label_28, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_28, 0, 10);
    lv_obj_set_size(ui->screen_label_28, 173, 23);

    //Write style for screen_label_28, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_28, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_28, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_28, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_28, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_28, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_param1_fast_up_spd
    ui->screen_param1_fast_up_spd = lv_textarea_create(ui->screen_cont_15);
    lv_textarea_set_placeholder_text(ui->screen_param1_fast_up_spd, "");
    //lv_textarea_set_password_bullet(ui->screen_param1_fast_up_spd, "*");
    lv_textarea_set_password_mode(ui->screen_param1_fast_up_spd, false);
    lv_textarea_set_one_line(ui->screen_param1_fast_up_spd, true);
    lv_textarea_set_accepted_chars(ui->screen_param1_fast_up_spd, "1234567890");
    lv_textarea_set_max_length(ui->screen_param1_fast_up_spd, 32);
    lv_obj_set_pos(ui->screen_param1_fast_up_spd, 165, 2);
    lv_obj_set_size(ui->screen_param1_fast_up_spd, 160, 45);

    //Write style for screen_param1_fast_up_spd, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_param1_fast_up_spd, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_param1_fast_up_spd, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_param1_fast_up_spd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_param1_fast_up_spd, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_param1_fast_up_spd, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_param1_fast_up_spd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_param1_fast_up_spd, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_param1_fast_up_spd, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_param1_fast_up_spd, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_param1_fast_up_spd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_param1_fast_up_spd, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_param1_fast_up_spd, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_param1_fast_up_spd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_param1_fast_up_spd, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_param1_fast_up_spd, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_param1_fast_up_spd, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_param1_fast_up_spd, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_param1_fast_up_spd, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_param1_fast_up_spd, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_param1_fast_up_spd, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_param1_fast_up_spd, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_param1_fast_up_spd, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_label_31
    ui->screen_label_31 = lv_label_create(ui->screen_cont_15);
    lv_label_set_text(ui->screen_label_31, "%");
    lv_label_set_long_mode(ui->screen_label_31, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_31, 290, 9);
    lv_obj_set_size(ui->screen_label_31, 26, 20);

    //Write style for screen_label_31, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_31, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_31, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_31, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_31, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_31, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_cont_14
    ui->screen_cont_14 = lv_obj_create(ui->screen_tileview_2_tile_2);
    lv_obj_set_pos(ui->screen_cont_14, 35, 64);
    lv_obj_set_size(ui->screen_cont_14, 330, 50);
    lv_obj_set_scrollbar_mode(ui->screen_cont_14, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_cont_14, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_cont_14, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_cont_14, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_cont_14, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_14, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_cont_14, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_cont_14, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_label_27
    ui->screen_label_27 = lv_label_create(ui->screen_cont_14);
    lv_label_set_text(ui->screen_label_27, "快速下降速度：");
    lv_label_set_long_mode(ui->screen_label_27, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_27, 0, 10);
    lv_obj_set_size(ui->screen_label_27, 173, 23);

    //Write style for screen_label_27, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_27, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_27, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_27, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_27, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_27, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_param1_fast_down_spd
    ui->screen_param1_fast_down_spd = lv_textarea_create(ui->screen_cont_14);
    lv_textarea_set_placeholder_text(ui->screen_param1_fast_down_spd, "");
    //lv_textarea_set_password_bullet(ui->screen_param1_fast_down_spd, "*");
    lv_textarea_set_password_mode(ui->screen_param1_fast_down_spd, false);
    lv_textarea_set_one_line(ui->screen_param1_fast_down_spd, true);
    lv_textarea_set_accepted_chars(ui->screen_param1_fast_down_spd, "1234567890");
    lv_textarea_set_max_length(ui->screen_param1_fast_down_spd, 32);
    lv_obj_set_pos(ui->screen_param1_fast_down_spd, 165, 1);
    lv_obj_set_size(ui->screen_param1_fast_down_spd, 160, 45);

    //Write style for screen_param1_fast_down_spd, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_param1_fast_down_spd, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_param1_fast_down_spd, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_param1_fast_down_spd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_param1_fast_down_spd, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_param1_fast_down_spd, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_param1_fast_down_spd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_param1_fast_down_spd, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_param1_fast_down_spd, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_param1_fast_down_spd, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_param1_fast_down_spd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_param1_fast_down_spd, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_param1_fast_down_spd, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_param1_fast_down_spd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_param1_fast_down_spd, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_param1_fast_down_spd, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_param1_fast_down_spd, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_param1_fast_down_spd, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_param1_fast_down_spd, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_param1_fast_down_spd, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_param1_fast_down_spd, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_param1_fast_down_spd, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_param1_fast_down_spd, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_label_26
    ui->screen_label_26 = lv_label_create(ui->screen_cont_14);
    lv_label_set_text(ui->screen_label_26, "%");
    lv_label_set_long_mode(ui->screen_label_26, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_26, 290, 9);
    lv_obj_set_size(ui->screen_label_26, 26, 20);

    //Write style for screen_label_26, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_26, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_26, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_26, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_26, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_26, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_cont_12
    ui->screen_cont_12 = lv_obj_create(ui->screen_tileview_2_tile_2);
    lv_obj_set_pos(ui->screen_cont_12, 35, 124);
    lv_obj_set_size(ui->screen_cont_12, 330, 50);
    lv_obj_set_scrollbar_mode(ui->screen_cont_12, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_cont_12, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_cont_12, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_cont_12, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_cont_12, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_12, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_cont_12, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_cont_12, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_label_23
    ui->screen_label_23 = lv_label_create(ui->screen_cont_12);
    lv_label_set_text(ui->screen_label_23, "慢速升降速度：");
    lv_label_set_long_mode(ui->screen_label_23, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_23, 0, 10);
    lv_obj_set_size(ui->screen_label_23, 173, 23);

    //Write style for screen_label_23, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_23, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_23, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_23, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_23, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_23, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_param1_slow_ud_spd
    ui->screen_param1_slow_ud_spd = lv_textarea_create(ui->screen_cont_12);
    lv_textarea_set_placeholder_text(ui->screen_param1_slow_ud_spd, "");
    //lv_textarea_set_password_bullet(ui->screen_param1_slow_ud_spd, "*");
    lv_textarea_set_password_mode(ui->screen_param1_slow_ud_spd, false);
    lv_textarea_set_one_line(ui->screen_param1_slow_ud_spd, true);
    lv_textarea_set_accepted_chars(ui->screen_param1_slow_ud_spd, "1234567890");
    lv_textarea_set_max_length(ui->screen_param1_slow_ud_spd, 32);
    lv_obj_set_pos(ui->screen_param1_slow_ud_spd, 165, 1);
    lv_obj_set_size(ui->screen_param1_slow_ud_spd, 160, 45);

    //Write style for screen_param1_slow_ud_spd, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_param1_slow_ud_spd, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_param1_slow_ud_spd, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_param1_slow_ud_spd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_param1_slow_ud_spd, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_param1_slow_ud_spd, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_param1_slow_ud_spd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_param1_slow_ud_spd, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_param1_slow_ud_spd, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_param1_slow_ud_spd, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_param1_slow_ud_spd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_param1_slow_ud_spd, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_param1_slow_ud_spd, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_param1_slow_ud_spd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_param1_slow_ud_spd, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_param1_slow_ud_spd, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_param1_slow_ud_spd, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_param1_slow_ud_spd, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_param1_slow_ud_spd, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_param1_slow_ud_spd, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_param1_slow_ud_spd, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_param1_slow_ud_spd, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_param1_slow_ud_spd, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_label_29
    ui->screen_label_29 = lv_label_create(ui->screen_cont_12);
    lv_label_set_text(ui->screen_label_29, "%");
    lv_label_set_long_mode(ui->screen_label_29, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_29, 290, 9);
    lv_obj_set_size(ui->screen_label_29, 26, 20);

    //Write style for screen_label_29, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_29, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_29, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_29, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_29, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_29, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    event_init_screen();
    //set_disable(true);
    //Update current screen layout.
    lv_obj_update_layout(ui->screen);
#if 0
    _write_item = std::vector<BoxValueItem>{
        BoxValueItem{ui->screen_fast_mv_spd, "D1070", REG_TYPE_FLOAT, calculate_nothing, ofmt_nothing<float>, ""},
        BoxValueItem{ui->screen_slow_mv_psd, "D1072", REG_TYPE_FLOAT, calculate_nothing, ofmt_nothing<float>, ""},
        BoxValueItem{ui->screen_fast_mv_spd_scl, "D1074", REG_TYPE_FLOAT, calculate_nothing, ofmt_nothing<float>, ""},
        BoxValueItem{ui->screen_mv_len_scl, "D1076", REG_TYPE_FLOAT, calculate_nothing, ofmt_nothing<float>, ""},
        BoxValueItem{ui->screen_single_mmt, "D1000",  REG_TYPE_FLOAT, calculate_nothing, ofmt_nothing<float>, ""},
        BoxValueItem{ui->screen_mv_alt_spd_1, "D1002", REG_TYPE_UINT32, calculate_nothing, ofmt_nothing<uint32>, ""},
        BoxValueItem{ui->screen_mv_alt_spd_2, "D1004", REG_TYPE_UINT32, calculate_nothing, ofmt_nothing<uint32>, ""},
        BoxValueItem{ui->screen_mv_tl_in_len, "D1006", REG_TYPE_FLOAT, calculate_nothing, ofmt_nothing<float>, ""},
        BoxValueItem{ui->screen_mv_tl_ang, "D1008", REG_TYPE_FLOAT, calculate_nothing, ofmt_nothing<float>, ""},
        BoxValueItem{ui->screen_param1_slow_ud_spd, "D1060", REG_TYPE_UINT32, multiple_x1000<uint32_t>, divide_1000<uint32_t>, ""},
        BoxValueItem{ui->screen_param1_fast_up_spd, "D1062", REG_TYPE_UINT32, multiple_x1000<uint32_t>, divide_1000<uint32_t>, ""},
        BoxValueItem{ui->screen_param1_fast_down_spd, "D1064", REG_TYPE_UINT32, multiple_x1000<uint32_t>, divide_1000<uint32_t>, ""}
    };
#endif
    _tasks = std::map<uint32_t, BoxValueItem>{
        std::make_pair((uint32_t)ui->screen_fast_mv_spd, 
                    BoxValueItem{ui->screen_fast_mv_spd, "D1070", REG_TYPE_FLOAT, REG_TYPE_FLOAT, ifmt_from_textarea<float>, ofmt_set_textarea<float>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_slow_mv_psd, 
                    BoxValueItem{ui->screen_slow_mv_psd, "D1072", REG_TYPE_FLOAT, REG_TYPE_FLOAT, ifmt_from_textarea<float>, ofmt_set_textarea<float>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_fast_mv_spd_scl, 
                    BoxValueItem{ui->screen_fast_mv_spd_scl, "D1074", REG_TYPE_FLOAT, REG_TYPE_FLOAT, ifmt_from_textarea<float>, ofmt_set_textarea<float>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_mv_len_scl, 
                    BoxValueItem{ui->screen_mv_len_scl, "D1076", REG_TYPE_FLOAT, REG_TYPE_FLOAT, ifmt_from_textarea<float>, ofmt_set_textarea<float>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_single_mmt, 
                    BoxValueItem{ui->screen_single_mmt, "D1000", REG_TYPE_FLOAT, REG_TYPE_FLOAT, ifmt_from_textarea<float>, ofmt_set_textarea<float>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_mv_alt_spd_1, 
                    BoxValueItem{ui->screen_mv_alt_spd_1, "D1002", REG_TYPE_UINT32, REG_TYPE_UINT32, ifmt_from_textarea<uint32_t>, ofmt_set_textarea<uint32_t>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_mv_alt_spd_2, 
                    BoxValueItem{ui->screen_mv_alt_spd_2, "D1004", REG_TYPE_UINT32, REG_TYPE_UINT32, ifmt_from_textarea<uint32_t>, ofmt_set_textarea<uint32_t>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_mv_tl_in_len, 
                    BoxValueItem{ui->screen_mv_tl_in_len, "D1006", REG_TYPE_FLOAT, REG_TYPE_FLOAT, ifmt_from_textarea<float>, ofmt_set_textarea<float>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_mv_tl_ang, 
                    BoxValueItem{ui->screen_mv_tl_ang, "D1008", REG_TYPE_FLOAT, REG_TYPE_FLOAT, ifmt_from_textarea<float>, ofmt_set_textarea<float>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_param1_slow_ud_spd, 
                    BoxValueItem{ui->screen_param1_slow_ud_spd, "D1060", REG_TYPE_UINT32, REG_TYPE_UINT32, ifmt_from_textarea_multip_1000<uint32_t>, ofmt_set_textarea_devide_1000<uint32_t>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_param1_fast_up_spd, 
                    BoxValueItem{ui->screen_param1_fast_up_spd, "D1062", REG_TYPE_UINT32, REG_TYPE_UINT32, ifmt_from_textarea_multip_1000<uint32_t>, ofmt_set_textarea_devide_1000<uint32_t>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_param1_fast_down_spd, 
                    BoxValueItem{ui->screen_param1_fast_down_spd, "D1064", REG_TYPE_UINT32, REG_TYPE_UINT32, ifmt_from_textarea_multip_1000<uint32_t>, ofmt_set_textarea_devide_1000<uint32_t>, (void*)""}),
    };
}

void Param1SettingWind::set_disable(bool disable){
    if(disable){
        lv_obj_clear_flag(ui->screen_fast_mv_spd, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_mv_alt_spd_1, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_mv_alt_spd_2, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_fast_mv_spd_scl, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_mv_len_scl, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_mv_tl_ang, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_mv_tl_in_len, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_param1_fast_down_spd, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_param1_fast_up_spd, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_param1_slow_ud_spd, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_slow_mv_psd, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_single_mmt, LV_OBJ_FLAG_CLICKABLE);
    }
    else{
        lv_obj_add_flag(ui->screen_fast_mv_spd, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_mv_alt_spd_1, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_mv_alt_spd_2, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_fast_mv_spd_scl, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_mv_len_scl, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_mv_tl_ang, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_mv_tl_in_len, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_param1_fast_down_spd, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_param1_fast_up_spd, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_param1_slow_ud_spd, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_slow_mv_psd, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_single_mmt, LV_OBJ_FLAG_CLICKABLE);
    }
}

void Param1SettingWind::event_init_screen(){
    lv_obj_add_event_cb(ui->screen_fast_mv_spd, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_mv_alt_spd_1, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_mv_alt_spd_2, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_fast_mv_spd_scl, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_mv_len_scl, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_mv_tl_ang, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_mv_tl_in_len, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_param1_fast_down_spd, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_param1_fast_up_spd, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_param1_slow_ud_spd, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_slow_mv_psd, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_single_mmt, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
}


Param2SettingWind* Param2SettingWind::getInstance(lv_obj_t* parent){
    static Param2SettingWind pWind(parent);
    return &pWind;
}

Param2SettingWind::Param2SettingWind(lv_obj_t* parent){
    ui = new lv_ui;
    //Write codes screen_1
    ui->screen_1 = parent;
    //lv_obj_set_size(ui->screen_1, 1024, 768);
    //lv_obj_clear_flag(ui->screen_1, LV_OBJ_FLAG_SCROLLABLE);
    //lv_obj_set_scrollbar_mode(ui->screen_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    //lv_obj_set_style_bg_opa(ui->screen_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_background
    ui->screen_1_background = parent;
    ui->screen_1_background_tile_1 = lv_tileview_add_tile(ui->screen_1_background, 0, 0, LV_DIR_RIGHT);
    lv_obj_set_style_bg_color(ui->screen_1_background, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    /*lv_obj_set_pos(ui->screen_1_background, 0, 0);
    lv_obj_set_size(ui->screen_1_background, 1024, 768);
    lv_obj_set_scrollbar_mode(ui->screen_1_background, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1_background, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_background, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_background, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_background, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_background, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_background, lv_color_hex(0xeaeff3), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_background, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_background, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    */

    //Write codes screen_1_label_55
    ui->screen_1_label_55 = lv_label_create(ui->screen_1_background_tile_1);
    lv_label_set_text(ui->screen_1_label_55, "喷漆电机参数设定");
    lv_label_set_long_mode(ui->screen_1_label_55, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_55, 4, 40);
    lv_obj_set_size(ui->screen_1_label_55, 263, 26);

    //Write style for screen_1_label_55, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_55, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_55, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_55, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_55, &lv_font_cn_songti_bold_25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_55, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_55, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_55, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_55, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_55, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_55, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_55, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_55, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_55, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_55, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_line_4
    ui->screen_1_line_4 = lv_line_create(ui->screen_1_background_tile_1);
    static lv_point_t screen_1_line_4[] = { {0, 0},{950, 0}, };
    lv_line_set_points(ui->screen_1_line_4, screen_1_line_4, 2);
    lv_obj_set_pos(ui->screen_1_line_4, 27, 80);
    lv_obj_set_size(ui->screen_1_line_4, 945, 4);

    //Write style for screen_1_line_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->screen_1_line_4, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->screen_1_line_4, lv_color_hex(0x11e652), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->screen_1_line_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->screen_1_line_4, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_tileview_4
    ui->screen_1_tileview_4 = lv_tileview_create(ui->screen_1_background_tile_1);
    ui->screen_1_tileview_4_tile_2 = lv_tileview_add_tile(ui->screen_1_tileview_4, 0, 0, LV_DIR_RIGHT);
    lv_obj_set_pos(ui->screen_1_tileview_4, 30, 91);
    lv_obj_set_size(ui->screen_1_tileview_4, 949, 328);
    lv_obj_set_scrollbar_mode(ui->screen_1_tileview_4, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1_tileview_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_tileview_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_tileview_4, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_tileview_4, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_tileview_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_tileview_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_tileview_4, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_tileview_4, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_tileview_4, lv_color_hex(0xeaeff3), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_tileview_4, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_tileview_4, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);



    //Write codes screen_1_cont_29
    ui->screen_1_cont_29 = lv_obj_create(ui->screen_1_tileview_4_tile_2);
    lv_obj_set_pos(ui->screen_1_cont_29, 35, 3);
    lv_obj_set_size(ui->screen_1_cont_29, 330, 50);
    lv_obj_set_scrollbar_mode(ui->screen_1_cont_29, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1_cont_29, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_cont_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_cont_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_cont_29, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_cont_29, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_cont_29, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_cont_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_cont_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_cont_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_cont_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_cont_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_label_54
    ui->screen_1_label_54 = lv_label_create(ui->screen_1_cont_29);
    lv_label_set_text(ui->screen_1_label_54, "快速顶漆速度：");
    lv_label_set_long_mode(ui->screen_1_label_54, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_54, 1, 11);
    lv_obj_set_size(ui->screen_1_label_54, 173, 23);

    //Write style for screen_1_label_54, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_54, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_54, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_54, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_54, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_54, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_54, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_54, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_54, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_54, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_54, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_54, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_54, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_54, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_54, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_ta_15
    ui->screen_1_ta_15 = lv_textarea_create(ui->screen_1_cont_29);
    lv_textarea_set_text(ui->screen_1_ta_15, "");
    lv_textarea_set_placeholder_text(ui->screen_1_ta_15, "");
    //lv_textarea_set_password_bullet(ui->screen_1_ta_15, "*");
    lv_textarea_set_password_mode(ui->screen_1_ta_15, false);
    lv_textarea_set_one_line(ui->screen_1_ta_15, true);
    lv_textarea_set_accepted_chars(ui->screen_1_ta_15, "1234567890.");
    lv_textarea_set_max_length(ui->screen_1_ta_15, 36);
    lv_obj_set_pos(ui->screen_1_ta_15, 165, 1);
    lv_obj_set_size(ui->screen_1_ta_15, 160, 45);

    //Write style for screen_1_ta_15, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_ta_15, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_ta_15, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_ta_15, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_ta_15, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_ta_15, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_ta_15, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_15, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_15, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_ta_15, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_ta_15, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_ta_15, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_ta_15, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_ta_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_ta_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_ta_15, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_ta_15, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_15, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_ta_15, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_ta_15, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_15, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_15, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_15, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_1_label_53
    ui->screen_1_label_53 = lv_label_create(ui->screen_1_cont_29);
    lv_label_set_text(ui->screen_1_label_53, "%");
    lv_label_set_long_mode(ui->screen_1_label_53, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_53, 290, 9);
    lv_obj_set_size(ui->screen_1_label_53, 26, 20);

    //Write style for screen_1_label_53, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_53, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_53, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_53, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_53, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_53, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_53, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_53, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_53, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_53, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_53, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_53, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_53, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_53, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_53, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_cont_27
    ui->screen_1_cont_27 = lv_obj_create(ui->screen_1_tileview_4_tile_2);
    lv_obj_set_pos(ui->screen_1_cont_27, 35, 64);
    lv_obj_set_size(ui->screen_1_cont_27, 330, 50);
    lv_obj_set_scrollbar_mode(ui->screen_1_cont_27, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1_cont_27, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_cont_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_cont_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_cont_27, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_cont_27, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_cont_27, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_cont_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_cont_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_cont_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_cont_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_cont_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_label_51
    ui->screen_1_label_51 = lv_label_create(ui->screen_1_cont_27);
    lv_label_set_text(ui->screen_1_label_51, "慢速顶漆速度：");
    lv_label_set_long_mode(ui->screen_1_label_51, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_51, 0, 10);
    lv_obj_set_size(ui->screen_1_label_51, 173, 23);

    //Write style for screen_1_label_51, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_51, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_51, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_51, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_51, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_51, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_51, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_51, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_51, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_51, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_51, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_51, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_51, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_51, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_51, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_ta_13
    ui->screen_1_ta_13 = lv_textarea_create(ui->screen_1_cont_27);
    lv_textarea_set_text(ui->screen_1_ta_13, "");
    lv_textarea_set_placeholder_text(ui->screen_1_ta_13, "");
    //lv_textarea_set_password_bullet(ui->screen_1_ta_13, "*");
    lv_textarea_set_password_mode(ui->screen_1_ta_13, false);
    lv_textarea_set_one_line(ui->screen_1_ta_13, true);
    lv_textarea_set_accepted_chars(ui->screen_1_ta_13, "1234567890.");
    lv_textarea_set_max_length(ui->screen_1_ta_13, 32);
    lv_obj_set_pos(ui->screen_1_ta_13, 165, 1);
    lv_obj_set_size(ui->screen_1_ta_13, 160, 45);

    //Write style for screen_1_ta_13, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_ta_13, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_ta_13, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_ta_13, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_ta_13, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_ta_13, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_ta_13, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_13, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_13, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_ta_13, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_ta_13, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_ta_13, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_ta_13, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_ta_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_ta_13, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_ta_13, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_ta_13, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_13, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_ta_13, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_ta_13, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_13, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_13, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_13, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_1_label_50
    ui->screen_1_label_50 = lv_label_create(ui->screen_1_cont_27);
    lv_label_set_text(ui->screen_1_label_50, "%");
    lv_label_set_long_mode(ui->screen_1_label_50, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_50, 290, 9);
    lv_obj_set_size(ui->screen_1_label_50, 26, 20);

    //Write style for screen_1_label_50, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_50, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_50, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_50, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_50, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_50, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_50, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_50, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_50, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_50, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_50, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_50, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_50, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_50, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_50, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_cont_26
    ui->screen_1_cont_26 = lv_obj_create(ui->screen_1_tileview_4_tile_2);
    lv_obj_set_pos(ui->screen_1_cont_26, 560, 124);
    lv_obj_set_size(ui->screen_1_cont_26, 360, 50);
    lv_obj_set_scrollbar_mode(ui->screen_1_cont_26, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1_cont_26, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_cont_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_cont_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_cont_26, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_cont_26, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_cont_26, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_cont_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_cont_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_cont_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_cont_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_cont_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_label_49
    ui->screen_1_label_49 = lv_label_create(ui->screen_1_cont_26);
    lv_label_set_text(ui->screen_1_label_49, "顶漆变速比：");
    lv_label_set_long_mode(ui->screen_1_label_49, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_49, 0, 10);
    lv_obj_set_size(ui->screen_1_label_49, 173, 23);

    //Write style for screen_1_label_49, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_49, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_49, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_49, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_49, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_49, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_49, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_49, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_49, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_49, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_49, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_49, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_49, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_49, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_49, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_ta_12
    ui->screen_1_ta_12 = lv_textarea_create(ui->screen_1_cont_26);
    lv_textarea_set_text(ui->screen_1_ta_12, "");
    lv_textarea_set_placeholder_text(ui->screen_1_ta_12, "");
    //lv_textarea_set_password_bullet(ui->screen_1_ta_12, "*");
    lv_textarea_set_password_mode(ui->screen_1_ta_12, false);
    lv_textarea_set_one_line(ui->screen_1_ta_12, true);
    lv_textarea_set_accepted_chars(ui->screen_1_ta_12, "1234567890");
    lv_textarea_set_max_length(ui->screen_1_ta_12, 32);
    lv_obj_set_pos(ui->screen_1_ta_12, 165, 1);
    lv_obj_set_size(ui->screen_1_ta_12, 70, 45);

    //Write style for screen_1_ta_12, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_ta_12, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_ta_12, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_ta_12, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_ta_12, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_ta_12, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_ta_12, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_12, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_12, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_ta_12, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_ta_12, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_ta_12, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_ta_12, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_ta_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_ta_12, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_ta_12, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_ta_12, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_12, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_ta_12, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_ta_12, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_12, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_12, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_12, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_1_label_48
    ui->screen_1_label_48 = lv_label_create(ui->screen_1_cont_26);
    lv_label_set_text(ui->screen_1_label_48, "/");
    lv_label_set_long_mode(ui->screen_1_label_48, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_48, 240, 8);
    lv_obj_set_size(ui->screen_1_label_48, 16, 20);

    //Write style for screen_1_label_48, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_48, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_48, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_48, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_48, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_48, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_48, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_48, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_48, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_48, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_48, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_48, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_48, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_48, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_48, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_ta_11
    ui->screen_1_ta_11 = lv_textarea_create(ui->screen_1_cont_26);
    lv_textarea_set_text(ui->screen_1_ta_11, "");
    lv_textarea_set_placeholder_text(ui->screen_1_ta_11, "");
    //lv_textarea_set_password_bullet(ui->screen_1_ta_11, "*");
    lv_textarea_set_password_mode(ui->screen_1_ta_11, false);
    lv_textarea_set_one_line(ui->screen_1_ta_11, true);
    lv_textarea_set_accepted_chars(ui->screen_1_ta_11, "1234567890");
    lv_textarea_set_max_length(ui->screen_1_ta_11, 32);
    lv_obj_set_pos(ui->screen_1_ta_11, 265, 2);
    lv_obj_set_size(ui->screen_1_ta_11, 70, 45);

    //Write style for screen_1_ta_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_ta_11, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_ta_11, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_ta_11, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_ta_11, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_ta_11, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_ta_11, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_11, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_11, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_ta_11, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_ta_11, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_ta_11, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_ta_11, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_ta_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_ta_11, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_ta_11, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_ta_11, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_11, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_ta_11, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_ta_11, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_11, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_11, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_11, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_1_cont_30
    ui->screen_1_cont_30 = lv_obj_create(ui->screen_1_tileview_4_tile_2);
    lv_obj_set_pos(ui->screen_1_cont_30, 570, 4);
    lv_obj_set_size(ui->screen_1_cont_30, 330, 50);
    lv_obj_set_scrollbar_mode(ui->screen_1_cont_30, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1_cont_30, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_cont_30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_cont_30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_cont_30, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_cont_30, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_cont_30, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_cont_30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_cont_30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_cont_30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_cont_30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_cont_30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_label_57
    ui->screen_1_label_57 = lv_label_create(ui->screen_1_cont_30);
    lv_label_set_text(ui->screen_1_label_57, "顶漆后退行程：");
    lv_label_set_long_mode(ui->screen_1_label_57, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_57, 0, 10);
    lv_obj_set_size(ui->screen_1_label_57, 173, 23);

    //Write style for screen_1_label_57, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_57, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_57, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_57, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_57, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_57, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_57, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_57, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_57, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_57, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_57, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_57, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_57, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_57, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_57, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_ta_16
    ui->screen_1_ta_16 = lv_textarea_create(ui->screen_1_cont_30);
    lv_textarea_set_text(ui->screen_1_ta_16, "");
    lv_textarea_set_placeholder_text(ui->screen_1_ta_16, "");
    //lv_textarea_set_password_bullet(ui->screen_1_ta_16, "*");
    lv_textarea_set_password_mode(ui->screen_1_ta_16, false);
    lv_textarea_set_one_line(ui->screen_1_ta_16, true);
    lv_textarea_set_accepted_chars(ui->screen_1_ta_16, "1234567890.");
    lv_textarea_set_max_length(ui->screen_1_ta_16, 5);
    lv_obj_set_pos(ui->screen_1_ta_16, 165, 1);
    lv_obj_set_size(ui->screen_1_ta_16, 160, 45);

    //Write style for screen_1_ta_16, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_ta_16, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_ta_16, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_ta_16, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_ta_16, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_ta_16, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_ta_16, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_16, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_16, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_ta_16, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_ta_16, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_ta_16, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_ta_16, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_ta_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_ta_16, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_ta_16, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_ta_16, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_16, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_ta_16, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_ta_16, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_16, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_16, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_16, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_1_label_56
    ui->screen_1_label_56 = lv_label_create(ui->screen_1_cont_30);
    lv_label_set_text(ui->screen_1_label_56, "mm");
    lv_label_set_long_mode(ui->screen_1_label_56, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_56, 270, 9);
    lv_obj_set_size(ui->screen_1_label_56, 60, 20);

    //Write style for screen_1_label_56, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_56, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_56, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_56, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_56, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_56, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_56, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_56, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_56, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_56, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_56, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_56, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_56, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_56, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_56, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_cont_31
    ui->screen_1_cont_31 = lv_obj_create(ui->screen_1_tileview_4_tile_2);
    lv_obj_set_pos(ui->screen_1_cont_31, 570, 64);
    lv_obj_set_size(ui->screen_1_cont_31, 330, 50);
    lv_obj_set_scrollbar_mode(ui->screen_1_cont_31, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1_cont_31, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_cont_31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_cont_31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_cont_31, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_cont_31, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_cont_31, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_cont_31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_cont_31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_cont_31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_cont_31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_cont_31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_label_58
    ui->screen_1_label_58 = lv_label_create(ui->screen_1_cont_31);
    lv_label_set_text(ui->screen_1_label_58, "顶漆脉冲数：");
    lv_label_set_long_mode(ui->screen_1_label_58, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_58, 0, 10);
    lv_obj_set_size(ui->screen_1_label_58, 173, 23);

    //Write style for screen_1_label_58, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_58, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_58, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_58, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_58, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_58, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_58, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_58, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_58, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_58, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_58, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_58, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_58, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_58, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_58, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_ta_17
    ui->screen_1_ta_17 = lv_textarea_create(ui->screen_1_cont_31);
    lv_textarea_set_text(ui->screen_1_ta_17, "");
    lv_textarea_set_placeholder_text(ui->screen_1_ta_17, "");
    //lv_textarea_set_password_bullet(ui->screen_1_ta_17, "*");
    lv_textarea_set_password_mode(ui->screen_1_ta_17, false);
    lv_textarea_set_one_line(ui->screen_1_ta_17, true);
    lv_textarea_set_accepted_chars(ui->screen_1_ta_17, "1234567890.");
    lv_textarea_set_max_length(ui->screen_1_ta_17, 32);
    lv_obj_set_pos(ui->screen_1_ta_17, 165, 1);
    lv_obj_set_size(ui->screen_1_ta_17, 160, 45);

    //Write style for screen_1_ta_17, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_ta_17, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_ta_17, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_ta_17, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_ta_17, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_ta_17, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_ta_17, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_17, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_17, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_ta_17, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_ta_17, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_ta_17, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_ta_17, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_ta_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_ta_17, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_ta_17, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_ta_17, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_17, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_ta_17, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_ta_17, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_17, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_17, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_17, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_1_cont_32
    ui->screen_1_cont_32 = lv_obj_create(ui->screen_1_tileview_4_tile_2);
    lv_obj_set_pos(ui->screen_1_cont_32, 35, 124);
    lv_obj_set_size(ui->screen_1_cont_32, 330, 50);
    lv_obj_set_scrollbar_mode(ui->screen_1_cont_32, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1_cont_32, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_cont_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_cont_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_cont_32, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_cont_32, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_cont_32, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_cont_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_cont_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_cont_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_cont_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_cont_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_label_60
    ui->screen_1_label_60 = lv_label_create(ui->screen_1_cont_32);
    lv_label_set_text(ui->screen_1_label_60, "顶漆工作速度：");
    lv_label_set_long_mode(ui->screen_1_label_60, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_60, 0, 10);
    lv_obj_set_size(ui->screen_1_label_60, 173, 23);

    //Write style for screen_1_label_60, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_60, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_60, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_60, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_60, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_60, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_60, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_60, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_60, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_60, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_60, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_60, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_60, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_60, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_60, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_ta_18
    ui->screen_1_ta_18 = lv_textarea_create(ui->screen_1_cont_32);
    lv_textarea_set_text(ui->screen_1_ta_18, "");
    lv_textarea_set_placeholder_text(ui->screen_1_ta_18, "");
    //lv_textarea_set_password_bullet(ui->screen_1_ta_18, "*");
    lv_textarea_set_password_mode(ui->screen_1_ta_18, false);
    lv_textarea_set_one_line(ui->screen_1_ta_18, true);
    lv_textarea_set_accepted_chars(ui->screen_1_ta_18, "1234567890.");
    lv_textarea_set_max_length(ui->screen_1_ta_18, 32);
    lv_obj_set_pos(ui->screen_1_ta_18, 165, 1);
    lv_obj_set_size(ui->screen_1_ta_18, 160, 45);

    //Write style for screen_1_ta_18, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_ta_18, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_ta_18, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_ta_18, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_ta_18, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_ta_18, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_ta_18, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_18, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_18, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_ta_18, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_ta_18, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_ta_18, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_ta_18, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_ta_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_ta_18, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_ta_18, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_ta_18, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_18, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_ta_18, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_ta_18, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_18, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_18, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_18, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_1_label_59
    ui->screen_1_label_59 = lv_label_create(ui->screen_1_cont_32);
    lv_label_set_text(ui->screen_1_label_59, "%");
    lv_label_set_long_mode(ui->screen_1_label_59, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_59, 290, 9);
    lv_obj_set_size(ui->screen_1_label_59, 26, 20);

    //Write style for screen_1_label_59, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_59, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_59, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_59, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_59, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_59, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_59, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_59, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_59, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_59, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_59, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_59, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_59, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_59, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_59, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_cont_35
    ui->screen_1_cont_35 = lv_obj_create(ui->screen_1_tileview_4_tile_2);
    lv_obj_set_pos(ui->screen_1_cont_35, 35, 184);
    lv_obj_set_size(ui->screen_1_cont_35, 330, 50);
    lv_obj_set_scrollbar_mode(ui->screen_1_cont_35, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1_cont_35, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_cont_35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_cont_35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_cont_35, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_cont_35, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_cont_35, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_cont_35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_cont_35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_cont_35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_cont_35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_cont_35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_label_66
    ui->screen_1_label_66 = lv_label_create(ui->screen_1_cont_35);
    lv_label_set_text(ui->screen_1_label_66, "大料顶漆行程：");
    lv_label_set_long_mode(ui->screen_1_label_66, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_66, 0, 10);
    lv_obj_set_size(ui->screen_1_label_66, 173, 23);

    //Write style for screen_1_label_66, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_66, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_66, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_66, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_66, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_66, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_66, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_66, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_66, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_66, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_66, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_66, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_66, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_66, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_66, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_ta_21
    ui->screen_1_ta_21 = lv_textarea_create(ui->screen_1_cont_35);
    lv_textarea_set_text(ui->screen_1_ta_21, "");
    lv_textarea_set_placeholder_text(ui->screen_1_ta_21, "");
    //lv_textarea_set_password_bullet(ui->screen_1_ta_21, "*");
    lv_textarea_set_password_mode(ui->screen_1_ta_21, false);
    lv_textarea_set_one_line(ui->screen_1_ta_21, true);
    lv_textarea_set_accepted_chars(ui->screen_1_ta_21, "1234567890.");
    lv_textarea_set_max_length(ui->screen_1_ta_21, 5);
    lv_obj_set_pos(ui->screen_1_ta_21, 165, 1);
    lv_obj_set_size(ui->screen_1_ta_21, 160, 45);

    //Write style for screen_1_ta_21, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_ta_21, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_ta_21, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_ta_21, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_ta_21, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_ta_21, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_ta_21, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_21, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_21, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_ta_21, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_ta_21, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_ta_21, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_ta_21, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_ta_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_ta_21, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_ta_21, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_ta_21, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_21, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_ta_21, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_ta_21, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_21, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_21, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_21, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_1_label_65
    ui->screen_1_label_65 = lv_label_create(ui->screen_1_cont_35);
    lv_label_set_text(ui->screen_1_label_65, "mm");
    lv_label_set_long_mode(ui->screen_1_label_65, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_65, 270, 9);
    lv_obj_set_size(ui->screen_1_label_65, 60, 20);

    //Write style for screen_1_label_65, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_65, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_65, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_65, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_65, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_65, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_65, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_65, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_65, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_65, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_65, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_65, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_65, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_65, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_65, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_cont_36
    ui->screen_1_cont_36 = lv_obj_create(ui->screen_1_tileview_4_tile_2);
    lv_obj_set_pos(ui->screen_1_cont_36, 34, 244);
    lv_obj_set_size(ui->screen_1_cont_36, 330, 50);
    lv_obj_set_scrollbar_mode(ui->screen_1_cont_36, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1_cont_36, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_cont_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_cont_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_cont_36, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_cont_36, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_cont_36, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_cont_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_cont_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_cont_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_cont_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_cont_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_label_68
    ui->screen_1_label_68 = lv_label_create(ui->screen_1_cont_36);
    lv_label_set_text(ui->screen_1_label_68, "小料顶漆行程：");
    lv_label_set_long_mode(ui->screen_1_label_68, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_68, 0, 10);
    lv_obj_set_size(ui->screen_1_label_68, 173, 23);

    //Write style for screen_1_label_68, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_68, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_68, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_68, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_68, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_68, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_68, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_68, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_68, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_68, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_68, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_68, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_68, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_68, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_68, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_ta_22
    ui->screen_1_ta_22 = lv_textarea_create(ui->screen_1_cont_36);
    lv_textarea_set_text(ui->screen_1_ta_22, "");
    lv_textarea_set_placeholder_text(ui->screen_1_ta_22, "");
    //lv_textarea_set_password_bullet(ui->screen_1_ta_22, "*");
    lv_textarea_set_password_mode(ui->screen_1_ta_22, false);
    lv_textarea_set_one_line(ui->screen_1_ta_22, true);
    lv_textarea_set_accepted_chars(ui->screen_1_ta_22, "1234567890.");
    lv_textarea_set_max_length(ui->screen_1_ta_22, 5);
    lv_obj_set_pos(ui->screen_1_ta_22, 165, 1);
    lv_obj_set_size(ui->screen_1_ta_22, 160, 45);

    //Write style for screen_1_ta_22, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_ta_22, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_ta_22, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_ta_22, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_ta_22, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_ta_22, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_ta_22, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_22, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_22, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_ta_22, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_ta_22, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_ta_22, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_ta_22, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_ta_22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_ta_22, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_ta_22, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_ta_22, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_22, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_ta_22, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_ta_22, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_22, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_22, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_22, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_1_label_67
    ui->screen_1_label_67 = lv_label_create(ui->screen_1_cont_36);
    lv_label_set_text(ui->screen_1_label_67, "mm");
    lv_label_set_long_mode(ui->screen_1_label_67, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_67, 270, 9);
    lv_obj_set_size(ui->screen_1_label_67, 60, 20);

    //Write style for screen_1_label_67, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_67, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_67, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_67, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_67, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_67, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_67, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_67, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_67, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_67, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_67, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_67, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_67, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_67, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_67, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_cont_37
    ui->screen_1_cont_37 = lv_obj_create(ui->screen_1_tileview_4_tile_2);
    lv_obj_set_pos(ui->screen_1_cont_37, 570, 184);
    lv_obj_set_size(ui->screen_1_cont_37, 330, 50);
    lv_obj_set_scrollbar_mode(ui->screen_1_cont_37, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1_cont_37, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_cont_37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_cont_37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_cont_37, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_cont_37, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_cont_37, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_cont_37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_cont_37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_cont_37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_cont_37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_cont_37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_label_70
    ui->screen_1_label_70 = lv_label_create(ui->screen_1_cont_37);
    lv_label_set_text(ui->screen_1_label_70, "顶漆丝杆导程：");
    lv_label_set_long_mode(ui->screen_1_label_70, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_70, 0, 9);
    lv_obj_set_size(ui->screen_1_label_70, 173, 23);

    //Write style for screen_1_label_70, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_70, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_70, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_70, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_70, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_70, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_70, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_70, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_70, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_70, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_70, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_70, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_70, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_70, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_70, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_ta_23
    ui->screen_1_ta_23 = lv_textarea_create(ui->screen_1_cont_37);
    lv_textarea_set_text(ui->screen_1_ta_23, "");
    lv_textarea_set_placeholder_text(ui->screen_1_ta_23, "");
    //lv_textarea_set_password_bullet(ui->screen_1_ta_23, "*");
    lv_textarea_set_password_mode(ui->screen_1_ta_23, false);
    lv_textarea_set_one_line(ui->screen_1_ta_23, true);
    lv_textarea_set_accepted_chars(ui->screen_1_ta_23, "1234567890.");
    lv_textarea_set_max_length(ui->screen_1_ta_23, 5);
    lv_obj_set_pos(ui->screen_1_ta_23, 165, 1);
    lv_obj_set_size(ui->screen_1_ta_23, 160, 45);

    //Write style for screen_1_ta_23, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_ta_23, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_ta_23, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_ta_23, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_ta_23, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_ta_23, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_ta_23, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_23, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_23, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_ta_23, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_ta_23, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_ta_23, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_ta_23, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_ta_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_ta_23, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_ta_23, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_ta_23, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_23, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_ta_23, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_ta_23, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_23, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_23, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_23, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_1_label_69
    ui->screen_1_label_69 = lv_label_create(ui->screen_1_cont_37);
    lv_label_set_text(ui->screen_1_label_69, "mm");
    lv_label_set_long_mode(ui->screen_1_label_69, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_69, 270, 9);
    lv_obj_set_size(ui->screen_1_label_69, 60, 20);

    //Write style for screen_1_label_69, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_69, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_69, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_69, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_69, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_69, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_69, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_69, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_69, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_69, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_69, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_69, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_69, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_69, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_69, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_cont_38
    ui->screen_1_cont_38 = lv_obj_create(ui->screen_1_tileview_4_tile_2);
    lv_obj_set_pos(ui->screen_1_cont_38, 570, 244);
    lv_obj_set_size(ui->screen_1_cont_38, 330, 50);
    lv_obj_set_scrollbar_mode(ui->screen_1_cont_38, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1_cont_38, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_cont_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_cont_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_cont_38, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_cont_38, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_cont_38, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_cont_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_cont_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_cont_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_cont_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_cont_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_label_72
    ui->screen_1_label_72 = lv_label_create(ui->screen_1_cont_38);
    lv_label_set_text(ui->screen_1_label_72, "顶漆漆筒内径：");
    lv_label_set_long_mode(ui->screen_1_label_72, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_72, 0, 10);
    lv_obj_set_size(ui->screen_1_label_72, 173, 23);

    //Write style for screen_1_label_72, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_72, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_72, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_72, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_72, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_72, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_72, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_72, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_72, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_72, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_72, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_72, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_72, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_72, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_72, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_ta_24
    ui->screen_1_ta_24 = lv_textarea_create(ui->screen_1_cont_38);
    lv_textarea_set_text(ui->screen_1_ta_24, "");
    lv_textarea_set_placeholder_text(ui->screen_1_ta_24, "");
    //lv_textarea_set_password_bullet(ui->screen_1_ta_24, "*");
    lv_textarea_set_password_mode(ui->screen_1_ta_24, false);
    lv_textarea_set_one_line(ui->screen_1_ta_24, true);
    lv_textarea_set_accepted_chars(ui->screen_1_ta_24, "1234567890.");
    lv_textarea_set_max_length(ui->screen_1_ta_24, 5);
    lv_obj_set_pos(ui->screen_1_ta_24, 165, 1);
    lv_obj_set_size(ui->screen_1_ta_24, 160, 45);

    //Write style for screen_1_ta_24, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_ta_24, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_ta_24, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_ta_24, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_ta_24, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_ta_24, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_ta_24, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_24, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_24, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_ta_24, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_ta_24, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_ta_24, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_ta_24, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_ta_24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_ta_24, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_ta_24, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_ta_24, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_24, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_ta_24, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_ta_24, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_24, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_24, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_24, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_1_label_71
    ui->screen_1_label_71 = lv_label_create(ui->screen_1_cont_38);
    lv_label_set_text(ui->screen_1_label_71, "mm");
    lv_label_set_long_mode(ui->screen_1_label_71, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_71, 270, 9);
    lv_obj_set_size(ui->screen_1_label_71, 60, 20);

    //Write style for screen_1_label_71, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_71, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_71, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_71, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_71, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_71, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_71, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_71, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_71, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_71, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_71, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_71, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_71, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_71, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_71, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_label_41
    ui->screen_1_label_41 = lv_label_create(ui->screen_1_background_tile_1);
    lv_label_set_text(ui->screen_1_label_41, "喷头电机参数设定");
    lv_label_set_long_mode(ui->screen_1_label_41, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_41, 4, 460);
    lv_obj_set_size(ui->screen_1_label_41, 263, 26);

    //Write style for screen_1_label_41, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_label_41, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_41, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_41, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_label_41, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_41, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_41, &lv_font_cn_songti_bold_25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_41, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_41, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_41, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_41, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_41, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_41, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_41, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_41, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_line_3
    ui->screen_1_line_3 = lv_line_create(ui->screen_1_background_tile_1);
    static lv_point_t screen_1_line_3[] = { {0, 0},{400, 0}, };
    lv_line_set_points(ui->screen_1_line_3, screen_1_line_3, 2);
    lv_obj_set_pos(ui->screen_1_line_3, 27, 501);
    lv_obj_set_size(ui->screen_1_line_3, 404, 3);

    //Write style for screen_1_line_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->screen_1_line_3, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->screen_1_line_3, lv_color_hex(0x11e652), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->screen_1_line_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->screen_1_line_3, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_tileview_3
    ui->screen_1_tileview_3 = lv_tileview_create(ui->screen_1_background_tile_1);
    ui->screen_1_tileview_3_tile_2 = lv_tileview_add_tile(ui->screen_1_tileview_3, 0, 0, LV_DIR_RIGHT);
    lv_obj_set_pos(ui->screen_1_tileview_3, 26, 521);
    lv_obj_set_size(ui->screen_1_tileview_3, 949, 182);
    lv_obj_set_scrollbar_mode(ui->screen_1_tileview_3, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1_tileview_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_tileview_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_tileview_3, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_tileview_3, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_tileview_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_tileview_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_tileview_3, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_tileview_3, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_tileview_3, lv_color_hex(0xeaeff3), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_tileview_3, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_tileview_3, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);



    //Write codes screen_1_cont_20
    ui->screen_1_cont_20 = lv_obj_create(ui->screen_1_tileview_3_tile_2);
    lv_obj_set_pos(ui->screen_1_cont_20, 523, 3);
    lv_obj_set_size(ui->screen_1_cont_20, 380, 50);
    lv_obj_set_scrollbar_mode(ui->screen_1_cont_20, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1_cont_20, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_cont_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_cont_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_cont_20, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_cont_20, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_cont_20, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_cont_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_cont_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_cont_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_cont_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_cont_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_label_39
    ui->screen_1_label_39 = lv_label_create(ui->screen_1_cont_20);
    lv_label_set_text(ui->screen_1_label_39, "行走电机补偿时间：");
    lv_label_set_long_mode(ui->screen_1_label_39, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_39, 5, 13);
    lv_obj_set_size(ui->screen_1_label_39, 210, 23);

    //Write style for screen_1_label_39, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_39, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_39, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_39, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_39, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_39, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_ta_5
    ui->screen_1_ta_5 = lv_textarea_create(ui->screen_1_cont_20);
    lv_textarea_set_text(ui->screen_1_ta_5, "");
    lv_textarea_set_placeholder_text(ui->screen_1_ta_5, "");
    //lv_textarea_set_password_bullet(ui->screen_1_ta_5, "*");
    lv_textarea_set_password_mode(ui->screen_1_ta_5, false);
    lv_textarea_set_one_line(ui->screen_1_ta_5, true);
    lv_textarea_set_accepted_chars(ui->screen_1_ta_5, "1234567890");
    lv_textarea_set_max_length(ui->screen_1_ta_5, 32);
    lv_obj_set_pos(ui->screen_1_ta_5, 213, 1);
    lv_obj_set_size(ui->screen_1_ta_5, 160, 45);

    //Write style for screen_1_ta_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_ta_5, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_ta_5, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_ta_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_ta_5, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_ta_5, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_ta_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_5, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_5, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_ta_5, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_ta_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_ta_5, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_ta_5, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_ta_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_ta_5, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_ta_5, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_ta_5, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_5, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_ta_5, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_ta_5, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_5, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_5, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_5, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_1_label_38
    ui->screen_1_label_38 = lv_label_create(ui->screen_1_cont_20);
    lv_label_set_text(ui->screen_1_label_38, "S");
    lv_label_set_long_mode(ui->screen_1_label_38, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_38, 337, 9);
    lv_obj_set_size(ui->screen_1_label_38, 26, 20);
    
    //Write style for screen_1_label_38, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_38, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_38, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_38, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_38, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_38, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_cont_19
    ui->screen_1_cont_19 = lv_obj_create(ui->screen_1_tileview_3_tile_2);
    lv_obj_set_pos(ui->screen_1_cont_19, 570, 64);
    lv_obj_set_size(ui->screen_1_cont_19, 330, 50);
    lv_obj_set_scrollbar_mode(ui->screen_1_cont_19, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1_cont_19, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_cont_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_cont_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_cont_19, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_cont_19, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_cont_19, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_cont_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_cont_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_cont_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_cont_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_cont_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_label_37
    ui->screen_1_label_37 = lv_label_create(ui->screen_1_cont_19);
    lv_label_set_text(ui->screen_1_label_37, "Link检测时间：");
    lv_label_set_long_mode(ui->screen_1_label_37, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_37, 0, 10);
    lv_obj_set_size(ui->screen_1_label_37, 173, 23);

    //Write style for screen_1_label_37, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_37, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_37, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_37, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_37, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_37, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_ta_4
    ui->screen_1_ta_4 = lv_textarea_create(ui->screen_1_cont_19);
    lv_textarea_set_text(ui->screen_1_ta_4, "");
    lv_textarea_set_placeholder_text(ui->screen_1_ta_4, "");
    //lv_textarea_set_password_bullet(ui->screen_1_ta_4, "*");
    lv_textarea_set_password_mode(ui->screen_1_ta_4, false);
    lv_textarea_set_one_line(ui->screen_1_ta_4, true);
    lv_textarea_set_accepted_chars(ui->screen_1_ta_4, "1234567890");
    lv_textarea_set_max_length(ui->screen_1_ta_4, 32);
    lv_obj_set_pos(ui->screen_1_ta_4, 165, 1);
    lv_obj_set_size(ui->screen_1_ta_4, 160, 45);

    //Write style for screen_1_ta_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_ta_4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_ta_4, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_ta_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_ta_4, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_ta_4, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_ta_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_4, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_4, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_ta_4, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_ta_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_ta_4, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_ta_4, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_ta_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_ta_4, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_ta_4, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_ta_4, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_4, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_ta_4, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_ta_4, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_4, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_4, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_4, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_1_label_36
    ui->screen_1_label_36 = lv_label_create(ui->screen_1_cont_19);
    lv_label_set_text(ui->screen_1_label_36, "S");
    lv_label_set_long_mode(ui->screen_1_label_36, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_36, 290, 9);
    lv_obj_set_size(ui->screen_1_label_36, 26, 20);

    //Write style for screen_1_label_36, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_36, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_36, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_36, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_36, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_36, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_cont_17
    ui->screen_1_cont_17 = lv_obj_create(ui->screen_1_tileview_3_tile_2);
    lv_obj_set_pos(ui->screen_1_cont_17, 35, 124);
    lv_obj_set_size(ui->screen_1_cont_17, 330, 50);
    lv_obj_set_scrollbar_mode(ui->screen_1_cont_17, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1_cont_17, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_cont_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_cont_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_cont_17, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_cont_17, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_cont_17, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_cont_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_cont_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_cont_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_cont_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_cont_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_label_33
    ui->screen_1_label_33 = lv_label_create(ui->screen_1_cont_17);
    lv_label_set_text(ui->screen_1_label_33, "喷头张开力矩：");
    lv_label_set_long_mode(ui->screen_1_label_33, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_33, 0, 10);
    lv_obj_set_size(ui->screen_1_label_33, 173, 23);

    //Write style for screen_1_label_33, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_33, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_33, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_33, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_33, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_33, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_ta_1
    ui->screen_1_ta_1 = lv_textarea_create(ui->screen_1_cont_17);
    lv_textarea_set_text(ui->screen_1_ta_1, "");
    lv_textarea_set_placeholder_text(ui->screen_1_ta_1, "");
    //lv_textarea_set_password_bullet(ui->screen_1_ta_1, "*");
    lv_textarea_set_password_mode(ui->screen_1_ta_1, false);
    lv_textarea_set_one_line(ui->screen_1_ta_1, true);
    lv_textarea_set_accepted_chars(ui->screen_1_ta_1, "1234567890");
    lv_textarea_set_max_length(ui->screen_1_ta_1, 32);
    lv_obj_set_pos(ui->screen_1_ta_1, 165, 1);
    lv_obj_set_size(ui->screen_1_ta_1, 160, 45);

    //Write style for screen_1_ta_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_ta_1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_ta_1, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_ta_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_ta_1, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_ta_1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_ta_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_1, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_ta_1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_ta_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_ta_1, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_ta_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_ta_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_ta_1, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_ta_1, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_ta_1, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_1, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_ta_1, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_ta_1, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_1, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_1, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_1, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_1_label_32
    ui->screen_1_label_32 = lv_label_create(ui->screen_1_cont_17);
    lv_label_set_text(ui->screen_1_label_32, "%");
    lv_label_set_long_mode(ui->screen_1_label_32, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_32, 290, 9);
    lv_obj_set_size(ui->screen_1_label_32, 26, 20);

    //Write style for screen_1_label_32, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_32, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_32, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_32, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_32, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_32, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_cont_39
    ui->screen_1_cont_39 = lv_obj_create(ui->screen_1_tileview_3_tile_2);
    lv_obj_set_pos(ui->screen_1_cont_39, 35, 4);
    lv_obj_set_size(ui->screen_1_cont_39, 330, 50);
    lv_obj_set_scrollbar_mode(ui->screen_1_cont_39, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1_cont_39, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_cont_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_cont_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_cont_39, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_cont_39, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_cont_39, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_cont_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_cont_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_cont_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_cont_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_cont_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_label_74
    ui->screen_1_label_74 = lv_label_create(ui->screen_1_cont_39);
    lv_label_set_text(ui->screen_1_label_74, "喷头工作速度：");
    lv_label_set_long_mode(ui->screen_1_label_74, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_74, 0, 10);
    lv_obj_set_size(ui->screen_1_label_74, 173, 23);

    //Write style for screen_1_label_74, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_74, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_74, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_74, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_74, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_74, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_74, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_74, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_74, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_74, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_74, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_74, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_74, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_74, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_74, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_ta_25
    ui->screen_1_ta_25 = lv_textarea_create(ui->screen_1_cont_39);
    lv_textarea_set_text(ui->screen_1_ta_25, "");
    lv_textarea_set_placeholder_text(ui->screen_1_ta_25, "");
    //lv_textarea_set_password_bullet(ui->screen_1_ta_25, "*");
    lv_textarea_set_password_mode(ui->screen_1_ta_25, false);
    lv_textarea_set_one_line(ui->screen_1_ta_25, true);
    lv_textarea_set_accepted_chars(ui->screen_1_ta_25, "1234567890");
    lv_textarea_set_max_length(ui->screen_1_ta_25, 32);
    lv_obj_set_pos(ui->screen_1_ta_25, 165, 1);
    lv_obj_set_size(ui->screen_1_ta_25, 160, 45);

    //Write style for screen_1_ta_25, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_ta_25, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_ta_25, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_ta_25, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_ta_25, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_ta_25, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_ta_25, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_25, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_25, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_ta_25, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_ta_25, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_ta_25, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_ta_25, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_ta_25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_ta_25, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_ta_25, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_ta_25, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_25, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_ta_25, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_ta_25, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_25, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_25, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_25, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_1_label_73
    ui->screen_1_label_73 = lv_label_create(ui->screen_1_cont_39);
    lv_label_set_text(ui->screen_1_label_73, "%");
    lv_label_set_long_mode(ui->screen_1_label_73, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_73, 290, 9);
    lv_obj_set_size(ui->screen_1_label_73, 26, 20);

    //Write style for screen_1_label_73, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_73, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_73, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_73, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_73, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_73, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_73, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_73, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_73, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_73, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_73, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_73, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_73, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_73, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_73, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_cont_40
    ui->screen_1_cont_40 = lv_obj_create(ui->screen_1_tileview_3_tile_2);
    lv_obj_set_pos(ui->screen_1_cont_40, 35, 64);
    lv_obj_set_size(ui->screen_1_cont_40, 330, 50);
    lv_obj_set_scrollbar_mode(ui->screen_1_cont_40, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1_cont_40, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_cont_40, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_cont_40, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_cont_40, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_cont_40, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_cont_40, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_cont_40, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_cont_40, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_cont_40, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_cont_40, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_cont_40, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_label_76
    ui->screen_1_label_76 = lv_label_create(ui->screen_1_cont_40);
    lv_label_set_text(ui->screen_1_label_76, "喷头闭合力矩：");
    lv_label_set_long_mode(ui->screen_1_label_76, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_76, 0, 10);
    lv_obj_set_size(ui->screen_1_label_76, 173, 23);

    //Write style for screen_1_label_76, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_76, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_76, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_76, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_76, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_76, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_76, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_76, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_76, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_76, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_76, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_76, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_76, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_76, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_76, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_ta_26
    ui->screen_1_ta_26 = lv_textarea_create(ui->screen_1_cont_40);
    lv_textarea_set_text(ui->screen_1_ta_26, "");
    lv_textarea_set_placeholder_text(ui->screen_1_ta_26, "");
    //lv_textarea_set_password_bullet(ui->screen_1_ta_26, "*");
    lv_textarea_set_password_mode(ui->screen_1_ta_26, false);
    lv_textarea_set_one_line(ui->screen_1_ta_26, true);
    lv_textarea_set_accepted_chars(ui->screen_1_ta_26, "1234567890");
    lv_textarea_set_max_length(ui->screen_1_ta_26, 32);
    lv_obj_set_pos(ui->screen_1_ta_26, 165, 1);
    lv_obj_set_size(ui->screen_1_ta_26, 160, 45);

    //Write style for screen_1_ta_26, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_ta_26, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_ta_26, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_ta_26, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_ta_26, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_ta_26, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_ta_26, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_26, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_26, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_ta_26, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_ta_26, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_ta_26, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_ta_26, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_ta_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_ta_26, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_ta_26, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_ta_26, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_26, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_ta_26, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_ta_26, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_26, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_26, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_26, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_1_label_75
    ui->screen_1_label_75 = lv_label_create(ui->screen_1_cont_40);
    lv_label_set_text(ui->screen_1_label_75, "%");
    lv_label_set_long_mode(ui->screen_1_label_75, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_75, 290, 9);
    lv_obj_set_size(ui->screen_1_label_75, 26, 20);

    //Write style for screen_1_label_75, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_75, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_75, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_75, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_75, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_75, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_75, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_75, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_75, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_75, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_75, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_75, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_75, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_75, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_75, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_line_5
    ui->screen_1_line_5 = lv_line_create(ui->screen_1_background_tile_1);
    static lv_point_t screen_1_line_5[] = { {0, 0},{400, 0}, };
    lv_line_set_points(ui->screen_1_line_5, screen_1_line_5, 2);
    lv_obj_set_pos(ui->screen_1_line_5, 569, 501);
    lv_obj_set_size(ui->screen_1_line_5, 404, 3);

    //Write style for screen_1_line_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->screen_1_line_5, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->screen_1_line_5, lv_color_hex(0x11e652), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->screen_1_line_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->screen_1_line_5, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_label_77
    ui->screen_1_label_77 = lv_label_create(ui->screen_1_background_tile_1);
    lv_label_set_text(ui->screen_1_label_77, "其他参数设定");
    lv_label_set_long_mode(ui->screen_1_label_77, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_77, 542, 460);
    lv_obj_set_size(ui->screen_1_label_77, 263, 26);

    //Write style for screen_1_label_77, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_label_77, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_77, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_77, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_label_77, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_77, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_77, &lv_font_cn_songti_bold_25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_77, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_77, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_77, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_77, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_77, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_77, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_77, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_77, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    events_init_screen();
    //set_disable(true);
    //Update current screen layout.
#if 0
    _write_item = std::vector<BoxValueItem>{
        {ui->screen_1_ta_15, "D1040", REG_TYPE_FLOAT, calculate_nothing, ofmt_nothing<float>, ""},
        {ui->screen_1_ta_13, "D1042", REG_TYPE_FLOAT, calculate_nothing, ofmt_nothing<float>, ""},
        {ui->screen_1_ta_18, "D1044", REG_TYPE_FLOAT, calculate_nothing, ofmt_nothing<float>, ""},
        {ui->screen_1_ta_21, "D1046", REG_TYPE_FLOAT, calculate_nothing, ofmt_nothing<float>, ""},
        {ui->screen_1_ta_22, "D1048", REG_TYPE_FLOAT, calculate_nothing, ofmt_nothing<float>, ""},
        {ui->screen_1_ta_16, "D1038", REG_TYPE_FLOAT, calculate_nothing, ofmt_nothing<float>, ""},
        {ui->screen_1_ta_17, "D1012", REG_TYPE_FLOAT, calculate_nothing, ofmt_nothing<float>, ""},
        {ui->screen_1_ta_12, "D1014", REG_TYPE_UINT32, calculate_nothing, ofmt_nothing<uint32_t>, ""},
        {ui->screen_1_ta_11, "D1016", REG_TYPE_UINT32, calculate_nothing, ofmt_nothing<uint32_t>, ""},
        {ui->screen_1_ta_23, "D1018", REG_TYPE_FLOAT, calculate_nothing, ofmt_nothing<float>, ""},
        {ui->screen_1_ta_24, "D1020", REG_TYPE_FLOAT, calculate_nothing, ofmt_nothing<float>, ""},
        {ui->screen_1_ta_25, "D1250", REG_TYPE_UINT16, multiple_x10<uint16_t>, divide_10<uint16_t>, ""},
        {ui->screen_1_ta_26, "D1251", REG_TYPE_UINT16, multiple_x10<uint16_t>, divide_10<uint16_t>, ""},
        {ui->screen_1_ta_1, "D1252", REG_TYPE_UINT16, multiple_x10<uint16_t>, divide_10<uint16_t>, ""},
        {ui->screen_1_ta_5, "D1053", REG_TYPE_UINT16, calculate_nothing, ofmt_nothing<uint16_t>, ""},
        {ui->screen_1_ta_4, "D1054", REG_TYPE_UINT16, calculate_nothing, ofmt_nothing<uint16_t>, ""},
    };
#endif
    _tasks = std::map<uint32_t, BoxValueItem>{
        std::make_pair((uint32_t)ui->screen_1_ta_15, 
                    BoxValueItem{ui->screen_1_ta_15, "D1040", REG_TYPE_FLOAT, REG_TYPE_FLOAT, ifmt_from_textarea<float>, ofmt_set_textarea<float>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_1_ta_13, 
                    BoxValueItem{ui->screen_1_ta_13, "D1042", REG_TYPE_FLOAT, REG_TYPE_FLOAT, ifmt_from_textarea<float>, ofmt_set_textarea<float>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_1_ta_18, 
                    BoxValueItem{ui->screen_1_ta_18, "D1044", REG_TYPE_FLOAT, REG_TYPE_FLOAT, ifmt_from_textarea<float>, ofmt_set_textarea<float>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_1_ta_21, 
                    BoxValueItem{ui->screen_1_ta_21, "D1046", REG_TYPE_FLOAT, REG_TYPE_FLOAT, ifmt_from_textarea<float>, ofmt_set_textarea<float>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_1_ta_22, 
                    BoxValueItem{ui->screen_1_ta_22, "D1048", REG_TYPE_FLOAT, REG_TYPE_FLOAT, ifmt_from_textarea<float>, ofmt_set_textarea<float>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_1_ta_16, 
                    BoxValueItem{ui->screen_1_ta_16, "D1038", REG_TYPE_FLOAT, REG_TYPE_FLOAT, ifmt_from_textarea<float>, ofmt_set_textarea<float>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_1_ta_17, 
                    BoxValueItem{ui->screen_1_ta_17, "D1012", REG_TYPE_FLOAT, REG_TYPE_FLOAT, ifmt_from_textarea<float>, ofmt_set_textarea<float>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_1_ta_12, 
                    BoxValueItem{ui->screen_1_ta_12, "D1014", REG_TYPE_UINT32, REG_TYPE_UINT32, ifmt_from_textarea<uint32_t>, ofmt_set_textarea<uint32_t>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_1_ta_11, 
                    BoxValueItem{ui->screen_1_ta_11, "D1016", REG_TYPE_UINT32, REG_TYPE_UINT32, ifmt_from_textarea<uint32_t>, ofmt_set_textarea<uint32_t>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_1_ta_23, 
                    BoxValueItem{ui->screen_1_ta_23, "D1018", REG_TYPE_FLOAT, REG_TYPE_FLOAT, ifmt_from_textarea<float>, ofmt_set_textarea<float>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_1_ta_24, 
                    BoxValueItem{ui->screen_1_ta_24, "D1020", REG_TYPE_FLOAT, REG_TYPE_FLOAT, ifmt_from_textarea<float>, ofmt_set_textarea<float>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_1_ta_25, 
                    BoxValueItem{ui->screen_1_ta_25, "D1050", REG_TYPE_UINT16, REG_TYPE_UINT16, ifmt_from_textarea_multip_10<uint16_t>, ofmt_set_textarea_divide_10<uint16_t>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_1_ta_26, 
                    BoxValueItem{ui->screen_1_ta_26, "D1051", REG_TYPE_UINT16, REG_TYPE_UINT16, ifmt_from_textarea_multip_10<uint16_t>, ofmt_set_textarea_divide_10<uint16_t>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_1_ta_1, 
                    BoxValueItem{ui->screen_1_ta_1, "D1052", REG_TYPE_UINT16, REG_TYPE_UINT16, ifmt_from_textarea_multip_10<uint16_t>, ofmt_set_textarea_divide_10<uint16_t>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_1_ta_5, 
                    BoxValueItem{ui->screen_1_ta_5, "D1053", REG_TYPE_UINT16, REG_TYPE_UINT16, ifmt_from_textarea_multip_10<uint16_t>, ofmt_set_textarea_divide_10<uint16_t>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_1_ta_4, 
                    BoxValueItem{ui->screen_1_ta_4, "D1054", REG_TYPE_UINT16, REG_TYPE_UINT16, ifmt_from_textarea<uint16_t>, ofmt_set_textarea<uint16_t>, (void*)""}),

    };
}

void Param2SettingWind::set_disable(bool disable){
    if(disable){
        lv_obj_clear_flag(ui->screen_1_ta_1, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_1_ta_4, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_1_ta_5, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_1_ta_11, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_1_ta_12, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_1_ta_13, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_1_ta_15, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_1_ta_16, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_1_ta_17, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_1_ta_18, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_1_ta_21, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_1_ta_22, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_1_ta_23, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_1_ta_24, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_1_ta_25, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_1_ta_26, LV_OBJ_FLAG_CLICKABLE);
    }
    else{
        lv_obj_add_flag(ui->screen_1_ta_1, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_1_ta_4, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_1_ta_5, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_1_ta_11, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_1_ta_12, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_1_ta_13, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_1_ta_15, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_1_ta_16, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_1_ta_17, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_1_ta_18, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_1_ta_21, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_1_ta_22, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_1_ta_23, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_1_ta_24, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_1_ta_25, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_1_ta_26, LV_OBJ_FLAG_CLICKABLE);
    }
}

void Param2SettingWind::events_init_screen(){
    lv_obj_add_event_cb(ui->screen_1_ta_1, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_1_ta_4, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_1_ta_5, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_1_ta_11, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_1_ta_12, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_1_ta_13, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_1_ta_15, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_1_ta_16, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_1_ta_17, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_1_ta_18, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_1_ta_21, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_1_ta_22, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_1_ta_23, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_1_ta_24, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_1_ta_25, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_1_ta_26, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    
}


ModelSettingWind* ModelSettingWind::getInstance(lv_obj_t* parent){
    static ModelSettingWind wind(parent);
    return &wind;
}

ModelSettingWind::ModelSettingWind(lv_obj_t* parent){
    ui = new lv_ui;
    ui->screen_1_background_tile_3 = parent;
    //Write codes screen_1_label_78
    ui->screen_1_label_78 = lv_label_create(ui->screen_1_background_tile_3);
    lv_label_set_text(ui->screen_1_label_78, "材料及工作模式");
    lv_label_set_long_mode(ui->screen_1_label_78, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_78, 380, 64);
    lv_obj_set_size(ui->screen_1_label_78, 263, 26);

    //Write style for screen_1_label_78, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_78, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_78, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_78, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_78, &lv_font_cn_songti_bold_25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_78, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_78, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_78, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_78, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_78, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_78, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_78, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_78, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_78, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_78, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_label_79
    ui->screen_1_label_79 = lv_label_create(ui->screen_1_background_tile_3);
    lv_label_set_text(ui->screen_1_label_79, "材料类型：");
    lv_label_set_long_mode(ui->screen_1_label_79, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_79, 98, 155);
    lv_obj_set_size(ui->screen_1_label_79, 151, 26);

    //Write style for screen_1_label_79, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_79, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_79, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_79, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_79, &lv_font_cn_songti_bold_25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_79, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_79, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_79, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_79, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_79, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_79, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_79, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_79, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_79, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_79, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_ddlist_1
    ui->screen_1_ddlist_1 = lv_dropdown_create(ui->screen_1_background_tile_3);
    lv_dropdown_set_options_static(ui->screen_1_ddlist_1, "大料\n小料");
    lv_dropdown_set_symbol(ui->screen_1_ddlist_1, &down_20x20);
    lv_obj_set_pos(ui->screen_1_ddlist_1, 241, 150);
    lv_obj_set_size(ui->screen_1_ddlist_1, 130, 37);

    //Write style for screen_1_ddlist_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_ddlist_1, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_ddlist_1, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_ddlist_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_ddlist_1, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_ddlist_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_ddlist_1, lv_color_hex(0xe1e6ee), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_ddlist_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_ddlist_1, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_ddlist_1, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_ddlist_1, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ddlist_1, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_ddlist_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ddlist_1, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ddlist_1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_ddlist_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    //Write style state: LV_STATE_CHECKED for &style_screen_1_ddlist_1_extra_list_selected_checked
    static lv_style_t style_screen_1_ddlist_1_extra_list_selected_checked;
    ui_init_style(&style_screen_1_ddlist_1_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_1_ddlist_1_extra_list_selected_checked, 1);
    lv_style_set_border_opa(&style_screen_1_ddlist_1_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_1_ddlist_1_extra_list_selected_checked, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_screen_1_ddlist_1_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_1_ddlist_1_extra_list_selected_checked, 3);
    lv_style_set_bg_opa(&style_screen_1_ddlist_1_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_1_ddlist_1_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_1_ddlist_1_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_t* pDropdownList = lv_dropdown_get_list(ui->screen_1_ddlist_1);
    if(pDropdownList != NULL){
        lv_obj_set_style_text_font(pDropdownList, &lv_font_cn_songti_medium_21, 0);
    }
    //lv_obj_add_style(list_option, &style_screen_1_ddlist_1_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_screen_1_ddlist_1_extra_list_main_default
    static lv_style_t style_screen_1_ddlist_1_extra_list_main_default;
    ui_init_style(&style_screen_1_ddlist_1_extra_list_main_default);

    lv_style_set_max_height(&style_screen_1_ddlist_1_extra_list_main_default, 90);
    lv_style_set_text_color(&style_screen_1_ddlist_1_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_1_ddlist_1_extra_list_main_default, &lv_font_cn_songti_medium_21);
    lv_style_set_text_opa(&style_screen_1_ddlist_1_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_1_ddlist_1_extra_list_main_default, 1);
    lv_style_set_border_opa(&style_screen_1_ddlist_1_extra_list_main_default, 255);
    lv_style_set_border_color(&style_screen_1_ddlist_1_extra_list_main_default, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_screen_1_ddlist_1_extra_list_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_1_ddlist_1_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_1_ddlist_1_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_1_ddlist_1_extra_list_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_1_ddlist_1_extra_list_main_default, LV_GRAD_DIR_NONE);
    //lv_obj_add_style(lv_dropdown_get_list(ui->screen_1_ddlist_1), &style_screen_1_ddlist_1_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_1_ddlist_1_extra_list_scrollbar_default
    static lv_style_t style_screen_1_ddlist_1_extra_list_scrollbar_default;
    ui_init_style(&style_screen_1_ddlist_1_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_1_ddlist_1_extra_list_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_screen_1_ddlist_1_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_1_ddlist_1_extra_list_scrollbar_default, lv_color_hex(0x00ff00));
    lv_style_set_bg_grad_dir(&style_screen_1_ddlist_1_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    //lv_obj_add_style(lv_dropdown_get_list(ui->screen_1_ddlist_1), &style_screen_1_ddlist_1_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_1_btn_1
    ui->screen_1_btn_1 = lv_btn_create(ui->screen_1_background_tile_3);
    ui->screen_1_btn_1_label = lv_label_create(ui->screen_1_btn_1);
    lv_label_set_text(ui->screen_1_btn_1_label, "标准模式");
    lv_label_set_long_mode(ui->screen_1_btn_1_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_1_btn_1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_1_btn_1, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_1_btn_1_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_1_btn_1, 103, 240);
    lv_obj_set_size(ui->screen_1_btn_1, 148, 37);

    //Write style for screen_1_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_btn_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_btn_1, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_btn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_btn_1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_btn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_btn_1, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_btn_1, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_btn_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_btn_2
    ui->screen_1_btn_2 = lv_btn_create(ui->screen_1_background_tile_3);
    ui->screen_1_btn_2_label = lv_label_create(ui->screen_1_btn_2);
    lv_label_set_text(ui->screen_1_btn_2_label, "手动模式");
    lv_label_set_long_mode(ui->screen_1_btn_2_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_1_btn_2_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_1_btn_2, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_1_btn_2_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_1_btn_2, 615, 240);
    lv_obj_set_size(ui->screen_1_btn_2, 148, 37);

    //Write style for screen_1_btn_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_btn_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_btn_2, lv_color_hex(0xbbbbbb), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_btn_2, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_btn_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_btn_2, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_btn_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_btn_2, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_btn_2, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_btn_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_btn_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_line_6
    ui->screen_1_line_6 = lv_line_create(ui->screen_1_background_tile_3);
    static lv_point_t screen_1_line_6[] = { {0, 0},{400, 0}, };
    lv_line_set_points(ui->screen_1_line_6, screen_1_line_6, 2);
    lv_obj_set_pos(ui->screen_1_line_6, 104, 325);
    lv_obj_set_size(ui->screen_1_line_6, 358, 2);

    //Write style for screen_1_line_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->screen_1_line_6, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->screen_1_line_6, lv_color_hex(0x11e652), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->screen_1_line_6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->screen_1_line_6, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_label_80
    ui->screen_1_label_80 = lv_label_create(ui->screen_1_background_tile_3);
    lv_label_set_text(ui->screen_1_label_80, "涂覆线径设定：");
    lv_label_set_long_mode(ui->screen_1_label_80, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_80, 92, 352);
    lv_obj_set_size(ui->screen_1_label_80, 173, 23);

    //Write style for screen_1_label_80, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_80, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_80, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_80, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_80, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_80, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_80, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_80, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_80, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_80, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_80, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_80, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_80, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_80, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_80, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_ta_27
    ui->screen_1_ta_27 = lv_textarea_create(ui->screen_1_background_tile_3);
    lv_textarea_set_text(ui->screen_1_ta_27, "");
    lv_textarea_set_placeholder_text(ui->screen_1_ta_27, "");
    ////lv_textarea_set_password_bullet(ui->screen_1_ta_27, "*");
    lv_textarea_set_password_mode(ui->screen_1_ta_27, false);
    lv_textarea_set_one_line(ui->screen_1_ta_27, true);
    lv_textarea_set_accepted_chars(ui->screen_1_ta_27, "1234567890.");
    lv_textarea_set_max_length(ui->screen_1_ta_27, 32);
    lv_obj_set_pos(ui->screen_1_ta_27, 255, 339);
    lv_obj_set_size(ui->screen_1_ta_27, 170, 45);

    //Write style for screen_1_ta_27, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_ta_27, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_ta_27, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_ta_27, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_ta_27, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_ta_27, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_ta_27, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_27, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_27, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_ta_27, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_ta_27, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_ta_27, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_ta_27, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_ta_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_ta_27, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_ta_27, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_ta_27, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_27, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_ta_27, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_ta_27, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_27, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_27, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_27, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_1_label_81
    ui->screen_1_label_81 = lv_label_create(ui->screen_1_background_tile_3);
    lv_label_set_text(ui->screen_1_label_81, "涂覆厚度设定：");
    lv_label_set_long_mode(ui->screen_1_label_81, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_81, 92, 422);
    lv_obj_set_size(ui->screen_1_label_81, 173, 23);

    //Write style for screen_1_label_81, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_81, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_81, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_81, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_81, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_81, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_81, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_81, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_81, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_81, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_81, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_81, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_81, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_81, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_81, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_ta_28
    ui->screen_1_ta_28 = lv_textarea_create(ui->screen_1_background_tile_3);
    lv_textarea_set_text(ui->screen_1_ta_28, "");
    lv_textarea_set_placeholder_text(ui->screen_1_ta_28, "");
    //lv_textarea_set_password_bullet(ui->screen_1_ta_28, "*");
    lv_textarea_set_password_mode(ui->screen_1_ta_28, false);
    lv_textarea_set_one_line(ui->screen_1_ta_28, true);
    lv_textarea_set_accepted_chars(ui->screen_1_ta_28, "1234567890.");
    lv_textarea_set_max_length(ui->screen_1_ta_28, 32);
    lv_obj_set_pos(ui->screen_1_ta_28, 255, 413);
    lv_obj_set_size(ui->screen_1_ta_28, 170, 45);

    //Write style for screen_1_ta_28, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_ta_28, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_ta_28, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_ta_28, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_ta_28, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_ta_28, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_ta_28, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_28, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_28, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_ta_28, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_ta_28, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_ta_28, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_ta_28, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_ta_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_ta_28, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_ta_28, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_ta_28, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_28, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_ta_28, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_ta_28, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_28, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_28, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_28, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_1_ta_29
    ui->screen_1_ta_29 = lv_textarea_create(ui->screen_1_background_tile_3);
    lv_textarea_set_text(ui->screen_1_ta_29, "");
    lv_textarea_set_placeholder_text(ui->screen_1_ta_29, "");
    //lv_textarea_set_password_bullet(ui->screen_1_ta_29, "*");
    lv_textarea_set_password_mode(ui->screen_1_ta_29, false);
    lv_textarea_set_one_line(ui->screen_1_ta_29, true);
    lv_textarea_set_accepted_chars(ui->screen_1_ta_29, "1234567890.");
    lv_textarea_set_max_length(ui->screen_1_ta_29, 32);
    lv_obj_set_pos(ui->screen_1_ta_29, 761, 339);
    lv_obj_set_size(ui->screen_1_ta_29, 170, 45);

    //Write style for screen_1_ta_29, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_ta_29, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_ta_29, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_ta_29, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_ta_29, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_ta_29, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_ta_29, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_29, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_29, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_ta_29, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_ta_29, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_ta_29, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_ta_29, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_ta_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_ta_29, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_ta_29, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_ta_29, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_29, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_ta_29, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_ta_29, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_29, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_29, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_29, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_1_line_7
    ui->screen_1_line_7 = lv_line_create(ui->screen_1_background_tile_3);
    static lv_point_t screen_1_line_7[] = { {0, 0},{400, 0}, };
    lv_line_set_points(ui->screen_1_line_7, screen_1_line_7, 2);
    lv_obj_set_pos(ui->screen_1_line_7, 604, 325);
    lv_obj_set_size(ui->screen_1_line_7, 358, 2);

    //Write style for screen_1_line_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->screen_1_line_7, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->screen_1_line_7, lv_color_hex(0x11e652), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->screen_1_line_7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->screen_1_line_7, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_ta_30
    ui->screen_1_ta_30 = lv_textarea_create(ui->screen_1_background_tile_3);
    lv_textarea_set_text(ui->screen_1_ta_30, "");
    lv_textarea_set_placeholder_text(ui->screen_1_ta_30, "");
    //lv_textarea_set_password_bullet(ui->screen_1_ta_30, "*");
    lv_textarea_set_password_mode(ui->screen_1_ta_30, false);
    lv_textarea_set_one_line(ui->screen_1_ta_30, true);
    lv_textarea_set_accepted_chars(ui->screen_1_ta_30, "1234567890.");
    lv_textarea_set_max_length(ui->screen_1_ta_30, 32);
    lv_obj_set_pos(ui->screen_1_ta_30, 761, 414);
    lv_obj_set_size(ui->screen_1_ta_30, 170, 45);

    //Write style for screen_1_ta_30, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_ta_30, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_ta_30, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_ta_30, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_ta_30, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_ta_30, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_ta_30, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_30, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_30, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_ta_30, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_ta_30, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_ta_30, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_ta_30, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_ta_30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_ta_30, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_ta_30, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_ta_30, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_30, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_ta_30, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_ta_30, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_30, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_30, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_30, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_1_label_82
    ui->screen_1_label_82 = lv_label_create(ui->screen_1_background_tile_3);
    lv_label_set_text(ui->screen_1_label_82, "顶漆速度设定：");
    lv_label_set_long_mode(ui->screen_1_label_82, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_82, 600, 352);
    lv_obj_set_size(ui->screen_1_label_82, 173, 23);

    //Write style for screen_1_label_82, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_82, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_82, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_82, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_82, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_82, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_82, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_82, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_82, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_82, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_82, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_82, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_82, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_82, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_82, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_label_83
    ui->screen_1_label_83 = lv_label_create(ui->screen_1_background_tile_3);
    lv_label_set_text(ui->screen_1_label_83, "行走速度设定：");
    lv_label_set_long_mode(ui->screen_1_label_83, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_83, 600, 426);
    lv_obj_set_size(ui->screen_1_label_83, 173, 23);

    //Write style for screen_1_label_83, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_83, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_83, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_83, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_83, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_83, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_83, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_83, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_83, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_83, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_83, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_83, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_83, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_83, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_83, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_ta_31
    ui->screen_1_ta_31 = lv_textarea_create(ui->screen_1_background_tile_3);
    lv_textarea_set_text(ui->screen_1_ta_31, "顶漆工作速度：");
    lv_textarea_set_placeholder_text(ui->screen_1_ta_31, "");
    //lv_textarea_set_password_bullet(ui->screen_1_ta_31, "*");
    lv_textarea_set_password_mode(ui->screen_1_ta_31, false);
    lv_textarea_set_one_line(ui->screen_1_ta_31, true);
    //lv_textarea_set_accepted_chars(ui->screen_1_ta_31, "1234567890.");
    lv_textarea_set_max_length(ui->screen_1_ta_31, 32);
    lv_obj_set_pos(ui->screen_1_ta_31, 94, 506);
    lv_obj_set_size(ui->screen_1_ta_31, 330, 45);
    lv_obj_clear_flag(ui->screen_1_ta_31, LV_OBJ_FLAG_SCROLLABLE);

    //Write style for screen_1_ta_31, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_ta_31, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_ta_31, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_ta_31, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_ta_31, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_ta_31, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_ta_31, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_31, lv_color_hex(0xe5e5e5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_31, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_ta_31, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_ta_31, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_ta_31, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_ta_31, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_ta_31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_ta_31, 9, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_ta_31, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_ta_31, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_31, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_ta_31, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_ta_31, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_31, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_31, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_31, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_1_ta_32
    ui->screen_1_ta_32 = lv_textarea_create(ui->screen_1_background_tile_3);
    lv_textarea_set_text(ui->screen_1_ta_32, "行走工作速度：");
    lv_textarea_set_placeholder_text(ui->screen_1_ta_32, "");
    //lv_textarea_set_password_bullet(ui->screen_1_ta_32, "*");
    lv_textarea_set_password_mode(ui->screen_1_ta_32, false);
    lv_textarea_set_one_line(ui->screen_1_ta_32, true);
    //lv_textarea_set_accepted_chars(ui->screen_1_ta_32, "1234567890");
    lv_textarea_set_max_length(ui->screen_1_ta_32, 32);
    lv_obj_set_pos(ui->screen_1_ta_32, 94, 592);
    lv_obj_set_size(ui->screen_1_ta_32, 330, 45);
    lv_obj_clear_flag(ui->screen_1_ta_32, LV_OBJ_FLAG_SCROLLABLE);

    //Write style for screen_1_ta_32, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_ta_32, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_ta_32, &lv_font_cn_songti_medium_21, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_ta_32, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_ta_32, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_ta_32, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_ta_32, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_32, lv_color_hex(0xe5e5e5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_32, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_ta_32, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_ta_32, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_ta_32, lv_color_hex(0xe6e6e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_ta_32, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_ta_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_ta_32, 9, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_ta_32, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_ta_32, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_32, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_ta_32, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_ta_32, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_ta_32, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_ta_32, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_ta_32, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    event_init_screen();

    _tasks = std::map<uint32_t, BoxValueItem>{
        std::make_pair((uint32_t)ui->screen_1_ddlist_1, 
                        BoxValueItem{ui->screen_1_ddlist_1, "D1080", REG_TYPE_UINT16, REG_TYPE_UINT16, ifmt_matrial_box, ofmt_matrial_box, (void*)""}),
        std::make_pair((uint32_t)this, 
                        BoxValueItem{this, "D1081", REG_TYPE_UINT16, REG_TYPE_UINT16, ifmt_btns, ofmt_btns, (void*)""}),
        std::make_pair((uint32_t)ui->screen_1_ta_27, 
                        BoxValueItem{ui->screen_1_ta_27, "D1082", REG_TYPE_FLOAT, REG_TYPE_FLOAT, ifmt_from_textarea<float>, ofmt_set_textarea<float>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_1_ta_28, 
                        BoxValueItem{ui->screen_1_ta_28, "D1084", REG_TYPE_FLOAT, REG_TYPE_FLOAT, ifmt_from_textarea<float>, ofmt_set_textarea<float>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_1_ta_29, 
                        BoxValueItem{ui->screen_1_ta_29, "D1086", REG_TYPE_FLOAT, REG_TYPE_FLOAT, ifmt_from_textarea<float>, ofmt_set_textarea<float>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_1_ta_30, 
                        BoxValueItem{ui->screen_1_ta_30, "D1088", REG_TYPE_FLOAT, REG_TYPE_FLOAT, ifmt_from_textarea<float>, ofmt_set_textarea<float>, (void*)""}),
        std::make_pair((uint32_t)ui->screen_1_ta_31, 
                        BoxValueItem{ui->screen_1_ta_31, "D152", REG_TYPE_FLOAT, REG_TYPE_FLOAT, ifmt_from_textarea<float>, ofmt_set_textarea<float>, (void*)"顶漆工作速度："}),
        std::make_pair((uint32_t)ui->screen_1_ta_32, 
                        BoxValueItem{ui->screen_1_ta_32, "D154", REG_TYPE_FLOAT, REG_TYPE_FLOAT, ifmt_from_textarea<float>, ofmt_set_textarea<float>, (void*)"行走工作速度："})
    };
}
#if 0
void ModelSettingWind::read_value(){
    if(_pPLCManager->is_connected()){
        for(auto iter = _write_item.begin(); 
            iter != _write_item.end(); iter++)
        {
            if(iter->reg_type == REG_TYPE_FLOAT){
                float read_val = 0;
                pthread_mutex_lock(&_pPLCManager->rw_lock);
                if(_pPLCManager->read_float(iter->address, &read_val) == 0){
                    LOG_DEBUG << "read float" << iter->address << " value: " << read_val;
                    std::string text = iter->ofmt(&read_val, iter->prefix);
                    pthread_mutex_lock(&lv_lock);
                    lv_textarea_set_text(iter->text_domain, text.c_str());
                    pthread_mutex_unlock(&lv_lock);
                }
                pthread_mutex_unlock(&_pPLCManager->rw_lock);
            }
        }
        uint16_t read_val = 0;
        pthread_mutex_lock(&_pPLCManager->rw_lock);
        bool ret = _pPLCManager->read_ushort("D1080", &read_val);
        pthread_mutex_unlock(&_pPLCManager->rw_lock);
        if(ret == 0 && read_val > 0 && read_val < 3){
            LOG_DEBUG << "read uint16" << "D1080" << " value: " << read_val;
            pthread_mutex_lock(&lv_lock);
            lv_dropdown_set_selected(ui->screen_1_ddlist_1, read_val-1);
            pthread_mutex_unlock(&lv_lock);
        }
        read_val = 0;
        pthread_mutex_lock(&_pPLCManager->rw_lock);
        ret = _pPLCManager->read_ushort("D1081", &read_val);
        pthread_mutex_unlock(&_pPLCManager->rw_lock);
        if(ret == 0 && read_val > 0 && read_val < 3){
            LOG_DEBUG << "read uint16" << "D1081" << " value: " << read_val;
            pthread_mutex_lock(&lv_lock);
            switch_model((model_t)read_val);
            pthread_mutex_unlock(&lv_lock);
        }
    }
}
#endif
void ModelSettingWind::switch_model(model_t model){
    if(model == MODEL_STD){
        _cur_model = MODEL_STD;
        lv_obj_set_style_bg_color(ui->screen_1_btn_1, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui->screen_1_btn_2, lv_color_hex(0xbbbbbb), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else if(model == MODEL_MANU){
        _cur_model = MODEL_MANU;
        lv_obj_set_style_bg_color(ui->screen_1_btn_2, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui->screen_1_btn_1, lv_color_hex(0xbbbbbb), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

void ModelSettingWind::event_init_screen(){
    lv_obj_add_event_cb(ui->screen_1_ddlist_1, dropdown_event_cb, LV_EVENT_VALUE_CHANGED, this);
    lv_obj_add_event_cb(ui->screen_1_btn_1, btn_model_std_event_cb, LV_EVENT_CLICKED, this);
    lv_obj_add_event_cb(ui->screen_1_btn_2, btn_model_menu_event_cb, LV_EVENT_CLICKED, this);
    lv_obj_add_event_cb(ui->screen_1_ta_27, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_1_ta_28, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_1_ta_29, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
    lv_obj_add_event_cb(ui->screen_1_ta_30, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, this);
}

void ModelSettingWind::dropdown_event_cb(lv_event_t* e){
    lv_event_code_t code = lv_event_get_code(e);
    ModelSettingWind* pWind = (ModelSettingWind*)lv_event_get_user_data(e);
    lv_obj_t* dropdown = lv_event_get_target(e);
    lv_obj_t* pDropdownList = lv_dropdown_get_list(dropdown);
    if(pDropdownList == NULL){
        LOG_DEBUG << "Null poiter to dropdown list.";
        return;
    }
    lv_obj_set_style_text_font(pDropdownList, &lv_font_cn_songti_medium_21, 0);

    if(code == LV_EVENT_VALUE_CHANGED){
        pWind->write_value(dropdown);
    }
}


void ModelSettingWind::btn_model_std_event_cb(lv_event_t* e){
    ModelSettingWind* pWind = (ModelSettingWind*)lv_event_get_user_data(e);
    lv_obj_t* btn = lv_event_get_target(e);
    pWind->switch_model(MODEL_STD);
    if(pWind->_pPLCManager->is_connected()){
        pWind->write_value(pWind);
    }
}

void ModelSettingWind::btn_model_menu_event_cb(lv_event_t* e){
    ModelSettingWind* pWind = (ModelSettingWind*)lv_event_get_user_data(e);
    lv_obj_t* btn = lv_event_get_target(e);
    pWind->switch_model(MODEL_MANU);
    if(pWind->_pPLCManager->is_connected()){
        pWind->write_value(pWind);
    }
}


WorkRecordWind* WorkRecordWind::getInstance(lv_obj_t* parent){
    static WorkRecordWind wind(parent);
    return &wind; 
}

WorkRecordWind::WorkRecordWind(lv_obj_t* parent)
:_pDatabase(DATABASE_FILE){
    ui = new lv_ui;
    ui->screen_1_background_tile_4 = parent;
    if(parent == NULL){
        printf("Receive nullptr as parent.\n");
        return;
    }

    lv_obj_add_flag(ui->screen_1_background_tile_4, LV_OBJ_FLAG_SCROLLABLE);
    //Write codes screen_1_label_84
    ui->screen_1_label_84 = lv_label_create(ui->screen_1_background_tile_4);
    lv_label_set_text(ui->screen_1_label_84, "工作记录");
    lv_label_set_long_mode(ui->screen_1_label_84, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_84, 375, 21);
    lv_obj_set_size(ui->screen_1_label_84, 263, 26);

    //Write style for screen_1_label_84, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_84, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_84, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_84, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_84, &lv_font_cn_songti_bold_25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_84, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_84, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_84, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_84, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_84, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_84, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_84, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_84, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_84, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_84, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_work_record
    ui->screen_1_work_record = lv_table_create(ui->screen_1_background_tile_4);
    lv_table_set_col_cnt(ui->screen_1_work_record, 7);
    lv_table_set_row_cnt(ui->screen_1_work_record, 1);
    lv_table_set_cell_value(ui->screen_1_work_record, 0, 0, "序号");
    lv_table_set_cell_value(ui->screen_1_work_record, 0, 1, "日期时间");
    lv_table_set_cell_value(ui->screen_1_work_record, 0, 2, "涂覆线径");
    lv_table_set_cell_value(ui->screen_1_work_record, 0, 3, "涂覆厚度");
    lv_table_set_cell_value(ui->screen_1_work_record, 0, 4, "行走速度");
    lv_table_set_cell_value(ui->screen_1_work_record, 0, 5, "顶漆速度");
    lv_table_set_cell_value(ui->screen_1_work_record, 0, 6, "涂覆长度");
    lv_obj_set_pos(ui->screen_1_work_record, 0, 64);
    lv_obj_set_scrollbar_mode(ui->screen_1_work_record, LV_SCROLLBAR_MODE_ACTIVE);

    //Write style for screen_1_work_record, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_align_to(ui->screen_1_work_record, ui->screen_1_label_84, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_pad_top(ui->screen_1_work_record, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_work_record, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_work_record, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_work_record, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_work_record, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_work_record, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_work_record, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_work_record, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_work_record, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_work_record, lv_color_hex(0xd5dee6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_work_record, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_work_record, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_work_record, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_work_record, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_work_record, lv_color_hex(0x393c41), LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_work_record, &lv_font_cn_songti_medium_21, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_work_record, 255, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_work_record, LV_TEXT_ALIGN_CENTER, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_work_record, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_work_record, 3, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_work_record, 255, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_work_record, lv_color_hex(0xd5dee6), LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_work_record, LV_BORDER_SIDE_FULL, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_work_record, 10, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_work_record, 10, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_work_record, 10, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_work_record, 10, LV_PART_ITEMS | LV_STATE_DEFAULT);
    ui->table_row = 31;
    lv_table_set_row_cnt(ui->screen_1_work_record, ui->table_row);

    /* button widget */
    ui->button_widget = lv_obj_create(ui->screen_1_background_tile_4);
    lv_obj_set_size(ui->button_widget, 375, 40);
    lv_obj_align_to(ui->button_widget, ui->screen_1_work_record, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_scrollbar_mode(ui->button_widget, LV_SCROLLBAR_MODE_OFF);
    //lv_obj_set_style_border_width(button_widget, 0, 0);
    lv_obj_set_style_pad_all(ui->button_widget, 0, 0);

    /* 按键栏 */
    static const char* map[] = { "主页", "上一页", "下一页", "" };
    ui->btnmx = lv_btnmatrix_create(ui->button_widget);
    lv_btnmatrix_set_map(ui->btnmx, map);
    lv_obj_set_style_pad_all(ui->btnmx, 0, 0);
    lv_obj_set_scrollbar_mode(ui->btnmx, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(ui->btnmx, lv_pct(100), lv_pct(100));
    lv_obj_set_style_text_font(ui->btnmx, &lv_font_cn_songti_medium_21_media_cfg, 0);
    //lv_obj_add_event_cb(btnmx, setting_btnmatrix_event_cb, LV_EVENT_VALUE_CHANGED, this);

    events_init_screen();
    //连接数据库
    _pDatabase.connect();
    //加载表
    load_first_table_record();
    update_table();
}

void WorkRecordWind::events_init_screen(){
    lv_obj_add_event_cb(ui->btnmx, btn_event_cb, LV_EVENT_VALUE_CHANGED, this);
}


bool WorkRecordWind::insert_record(const std::string & timestamp, 
                        const std::string & arg1, 
                        const std::string & arg2, 
                        const std::string & arg3, 
                        const std::string & arg4, 
                        const std::string & arg5){
    SQL_Response resp;
    std::string create_table_sql = "CREATE TABLE IF NOT EXISTS work_record(\n"\
                                "timestamp TIMESTAMP PRIMARY KEY,\n"\
                                "arg1 FLOAT,\n"\
                                "arg2 FLOAT,\n"\
                                "arg3 FLOAT,\n"\
                                "arg4 FLOAT,\n"\
                                "arg5 FLOAT);";
    std::string insert_sql_fmt = "INSERT INTO work_record VALUES({0}, {1}, {2}, {3}, {4}, {5});";
    std::string insert_sql = util::Format(insert_sql_fmt, timestamp, 
                                        arg1, arg2, arg3, arg4, arg5);
    _workRecordList.push_front(WorkRecordItem{timestamp, arg1, arg2, arg3, arg4, arg5});
    if(_workRecordList.size() > 30){
        _workRecordList.pop_back();
    }
    //创建数据库表
    bool ret = _pDatabase.excute(create_table_sql, resp);
    if(!ret) return ret;
    //插入数据
    ret = _pDatabase.excute(insert_sql, resp);
    if(!ret) return ret;
    //更新表
    update_table();
    return true;
}

bool WorkRecordWind::load_first_table_record(){
    SQL_Response resp;
    std::string sql = "SELECT * FROM work_record "\
                    "ORDER BY timestamp DESC "\
                    "LIMIT 30;";
    bool ret = _pDatabase.excute(sql, resp);
    if(!ret) return ret;
    if(resp.size() > 0){
        _workRecordList.clear();
        for(auto iter = resp.begin(); iter != resp.end(); iter++){
            WorkRecordItem item{iter->at("timestamp"), iter->at("arg1"),
                                iter->at("arg2"), iter->at("arg3"),
                                iter->at("arg4"), iter->at("arg5")};
            _workRecordList.push_back(item);
        }
    }
    return true;
}

bool WorkRecordWind::load_next_table_record(){
    SQL_Response resp;
    std::string sql_fmt = "SELECT * FROM work_record "\
                    "WHERE timestamp < {0} "\
                    "ORDER BY timestamp DESC "\
                    "LIMIT 30;";
    auto item = _workRecordList.back();
    std::string sql = util::Format(sql_fmt, item.timestamp);
    bool ret = _pDatabase.excute(sql, resp);
    if(!ret) return ret;
    if(resp.size() > 0){
        _workRecordList.clear();
        for(auto iter = resp.begin(); iter != resp.end(); iter++){
            WorkRecordItem item{iter->at("timestamp"), iter->at("arg1"),
                                iter->at("arg2"), iter->at("arg3"),
                                iter->at("arg4"), iter->at("arg5")};
            _workRecordList.push_back(item);
        }
    }
    return true;
}

bool WorkRecordWind::load_previous_table_record(){
    SQL_Response resp;
    std::string sql_fmt = "SELECT * FROM work_record "\
                    "WHERE timestamp > {0} "\
                    "ORDER BY timestamp DESC "\
                    "LIMIT 30;";
    auto item = _workRecordList.front();
    std::string sql = util::Format(sql_fmt, item.timestamp);
    bool ret = _pDatabase.excute(sql, resp);
    if(!ret) return ret;
    if(_workRecordList.size() > 0){
        _workRecordList.clear();
        for(auto iter = resp.begin(); iter != resp.end(); iter++){
            WorkRecordItem item{iter->at("timestamp"), iter->at("arg1"),
                                iter->at("arg2"), iter->at("arg3"),
                                iter->at("arg4"), iter->at("arg5")};
            _workRecordList.push_back(item);
        }
    }
    return true;
}


void WorkRecordWind::update_table(){
    std::stringstream ss;
    for(int row = 1; row < _workRecordList.size()&&row < ui->table_row; row++){
        //序号
        ss.str("");
        ss << (row);
        lv_table_set_cell_value(ui->screen_1_work_record, row, 0, ss.str().c_str());
        //本地时间
        ss.str("");
        time_t timestamp = (time_t)atoi(std::next(_workRecordList.begin(), row)->timestamp.c_str());
        struct tm* local_time = localtime(&timestamp);
        char formatted_time[100];
        size_t size = strftime(formatted_time, sizeof(formatted_time),
                            "%Y-%m-%d %H:%M:%S", local_time);
        if (size == 0) {
            LOG_ERROR << "Formating time error: " << timestamp;
        }
        ss << formatted_time;
        lv_table_set_cell_value(ui->screen_1_work_record, row, 1, ss.str().c_str());
        ss.str("");
        ss << std::next(_workRecordList.begin(), row)->arg1;
        lv_table_set_cell_value(ui->screen_1_work_record, row, 2, ss.str().c_str());
        ss.str("");
        ss << std::next(_workRecordList.begin(), row)->arg2;
        lv_table_set_cell_value(ui->screen_1_work_record, row, 3, ss.str().c_str());
        ss.str("");
        ss << std::next(_workRecordList.begin(), row)->arg3;
        lv_table_set_cell_value(ui->screen_1_work_record, row, 4, ss.str().c_str());
        ss.str("");
        ss << std::next(_workRecordList.begin(), row)->arg4;
        lv_table_set_cell_value(ui->screen_1_work_record, row, 5, ss.str().c_str());
        ss.str("");
        ss << std::next(_workRecordList.begin(), row)->arg5;
        lv_table_set_cell_value(ui->screen_1_work_record, row, 6, ss.str().c_str());
    }
    lv_obj_align_to(ui->button_widget, ui->screen_1_work_record, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
}

void WorkRecordWind::btn_event_cb(lv_event_t* e){
    WorkRecordWind* pWind = reinterpret_cast<WorkRecordWind*>(lv_event_get_user_data(e));
    lv_obj_t* btns = reinterpret_cast<lv_obj_t*>(lv_event_get_target(e));
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_VALUE_CHANGED){
        if(lv_btnmatrix_get_selected_btn(btns) == 0){
            pWind->load_first_table_record();
        }
        else if(lv_btnmatrix_get_selected_btn(btns) == 1){
            pWind->load_previous_table_record();
        }
        else if(lv_btnmatrix_get_selected_btn(btns) == 2){
            pWind->load_next_table_record();
        }
        pWind->update_table();
    }
}


FaultRecordWind* FaultRecordWind::getInstance(lv_obj_t* parent){
    static FaultRecordWind wind(parent);
    return &wind;
}

FaultRecordWind::FaultRecordWind(lv_obj_t* parent)
:_pDatabase(DATABASE_FILE){
    ui = new lv_ui;
    ui->screen_1_background_tile_5 = parent;
    if(parent == NULL){
        printf("Receive nullptr as parent.\n");
        return;
    }

    //Write codes screen_1_label_85
    ui->screen_1_label_85 = lv_label_create(ui->screen_1_background_tile_5);
    lv_label_set_text(ui->screen_1_label_85, "故障事件记录");
    lv_label_set_long_mode(ui->screen_1_label_85, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_85, 372, 21);
    lv_obj_set_size(ui->screen_1_label_85, 263, 26);

    //Write style for screen_1_label_85, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_85, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_85, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_85, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_85, &lv_font_cn_songti_bold_25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_85, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_85, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_85, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_85, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_85, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_85, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_85, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_85, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_85, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_85, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_fault_record
    ui->screen_1_fault_record = lv_table_create(ui->screen_1_background_tile_5);
    lv_table_set_col_cnt(ui->screen_1_fault_record, 3);
    lv_table_set_row_cnt(ui->screen_1_fault_record, 1);
    lv_table_set_cell_value(ui->screen_1_fault_record, 0, 0, "序号");
    lv_table_set_cell_value(ui->screen_1_fault_record, 0, 1, "日期时间");
    lv_table_set_cell_value(ui->screen_1_fault_record, 0, 2, "故障事件");
    lv_obj_set_pos(ui->screen_1_fault_record, 0, 63);
    lv_obj_set_scrollbar_mode(ui->screen_1_fault_record, LV_SCROLLBAR_MODE_ACTIVE);
    lv_table_set_col_width(ui->screen_1_fault_record, 2, 600);

    //Write style for screen_1_fault_record, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_align_to(ui->screen_1_fault_record, ui->screen_1_label_85, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_pad_top(ui->screen_1_fault_record, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_fault_record, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_fault_record, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_fault_record, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_fault_record, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_fault_record, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_fault_record, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_fault_record, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_fault_record, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_fault_record, lv_color_hex(0xd5dee6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_fault_record, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_fault_record, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_fault_record, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_fault_record, Part: LV_PART_MAIN, State: LV_STATE_FOCUSED.
    lv_obj_set_style_pad_top(ui->screen_1_fault_record, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_pad_bottom(ui->screen_1_fault_record, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_pad_left(ui->screen_1_fault_record, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_pad_right(ui->screen_1_fault_record, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui->screen_1_fault_record, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_color(ui->screen_1_fault_record, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_fault_record, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_width(ui->screen_1_fault_record, 2, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_opa(ui->screen_1_fault_record, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_color(ui->screen_1_fault_record, lv_color_hex(0xd5dee6), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(ui->screen_1_fault_record, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_radius(ui->screen_1_fault_record, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_width(ui->screen_1_fault_record, 0, LV_PART_MAIN | LV_STATE_FOCUSED);

    //Write style for screen_1_fault_record, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_fault_record, lv_color_hex(0x393c41), LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_fault_record, &lv_font_cn_songti_medium_21, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_fault_record, 255, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_fault_record, LV_TEXT_ALIGN_CENTER, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_fault_record, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_fault_record, 3, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_fault_record, 255, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_fault_record, lv_color_hex(0xd5dee6), LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_fault_record, LV_BORDER_SIDE_FULL, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_fault_record, 10, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_fault_record, 10, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_fault_record, 10, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_fault_record, 10, LV_PART_ITEMS | LV_STATE_DEFAULT);

    //Write style for screen_1_fault_record, Part: LV_PART_ITEMS, State: LV_STATE_FOCUSED.
    lv_obj_set_style_text_color(ui->screen_1_fault_record, lv_color_hex(0x393c41), LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_text_font(ui->screen_1_fault_record, &lv_font_cn_songti_medium_21, LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_text_opa(ui->screen_1_fault_record, 255, LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_text_align(ui->screen_1_fault_record, LV_TEXT_ALIGN_CENTER, LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui->screen_1_fault_record, 0, LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_border_width(ui->screen_1_fault_record, 3, LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_border_opa(ui->screen_1_fault_record, 255, LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_border_color(ui->screen_1_fault_record, lv_color_hex(0xd5dee6), LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(ui->screen_1_fault_record, LV_BORDER_SIDE_FULL, LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_pad_top(ui->screen_1_fault_record, 10, LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_pad_bottom(ui->screen_1_fault_record, 10, LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_pad_left(ui->screen_1_fault_record, 10, LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_pad_right(ui->screen_1_fault_record, 10, LV_PART_ITEMS | LV_STATE_FOCUSED);
    ui->table_row = 31;
    lv_table_set_row_cnt(ui->screen_1_fault_record, ui->table_row);

    /* button widget */
    ui->button_widget = lv_obj_create(ui->screen_1_background_tile_5);
    lv_obj_set_size(ui->button_widget, 375, 40);
    lv_obj_align_to(ui->button_widget, ui->screen_1_fault_record, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_scrollbar_mode(ui->button_widget, LV_SCROLLBAR_MODE_OFF);
    //lv_obj_set_style_border_width(button_widget, 0, 0);
    lv_obj_set_style_pad_all(ui->button_widget, 0, 0);

    /* 按键栏 */
    static const char* map[] = { "主页", "上一页", "下一页", "" };
    ui->btnmx = lv_btnmatrix_create(ui->button_widget);
    lv_btnmatrix_set_map(ui->btnmx, map);
    lv_obj_set_style_pad_all(ui->btnmx, 0, 0);
    lv_obj_set_scrollbar_mode(ui->btnmx, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(ui->btnmx, lv_pct(100), lv_pct(100));
    lv_obj_set_style_text_font(ui->btnmx, &lv_font_cn_songti_medium_21_media_cfg, 0);
    //lv_obj_add_event_cb(btnmx, setting_btnmatrix_event_cb, LV_EVENT_VALUE_CHANGED, this);
    
    events_init_screen();
    //连接数据库
    _pDatabase.connect();
    //加载表
    load_first_table_record();
    update_table();
}

void FaultRecordWind::events_init_screen(){
    lv_obj_add_event_cb(ui->btnmx, btn_event_cb, LV_EVENT_VALUE_CHANGED, this);
}

bool FaultRecordWind::insert_record(const std::string & timestamp, 
                        const std::string & event){
    SQL_Response resp;
    std::string create_table_sql = "CREATE TABLE IF NOT EXISTS fault_record(\n"\
                                "timestamp TIMESTAMP PRIMARY KEY,\n"\
                                "event VARCHAR(50));";
    std::string insert_sql_fmt = "INSERT INTO fault_record VALUES({0}, {1});";
    std::string insert_sql = util::Format(insert_sql_fmt, timestamp, event);
    _faultRecordList.push_front(FaultRecordItem{timestamp, event});
    if(_faultRecordList.size() > 30){
        _faultRecordList.pop_back();
    }
    //创建数据库表
    bool ret = _pDatabase.excute(create_table_sql, resp);
    if(!ret) return ret;
    //插入数据
    ret = _pDatabase.excute(insert_sql, resp);
    if(!ret) return ret;
    //更新表
    update_table();
    return true;
}

bool FaultRecordWind::load_first_table_record(){
    SQL_Response resp;
    std::string sql = "SELECT * FROM fault_record "\
                    "ORDER BY timestamp DESC "\
                    "LIMIT 30;";
    bool ret = _pDatabase.excute(sql, resp);
    if(!ret) return ret;
    if(resp.size() > 0){
        _faultRecordList.clear();
        for(auto iter = resp.begin(); iter != resp.end(); iter++){
            FaultRecordItem item{iter->at("timestamp"), iter->at("event")};
            _faultRecordList.push_back(item);
        }
    }
    return true;
}

bool FaultRecordWind::load_next_table_record(){
    SQL_Response resp;
    std::string sql_fmt = "SELECT * FROM fault_record "\
                    "WHERE timestamp < {0} "\
                    "ORDER BY timestamp DESC "\
                    "LIMIT 30;";
    auto item = _faultRecordList.back();
    std::string sql = util::Format(sql_fmt, item.timestamp);
    bool ret = _pDatabase.excute(sql, resp);
    if(!ret) return ret;
    if(resp.size() > 0){
        _faultRecordList.clear();
        for(auto iter = resp.begin(); iter != resp.end(); iter++){
            FaultRecordItem item{iter->at("timestamp"), iter->at("event")};
            _faultRecordList.push_back(item);
        }
    }
    return true;
}

bool FaultRecordWind::load_previous_table_record(){
    SQL_Response resp;
    std::string sql_fmt = "SELECT * FROM fault_record "\
                    "WHERE timestamp > {0} "\
                    "ORDER BY timestamp DESC "\
                    "LIMIT 30;";
    auto item = _faultRecordList.front();
    std::string sql = util::Format(sql_fmt, item.timestamp);
    bool ret = _pDatabase.excute(sql, resp);
    if(!ret) return ret;
    if(resp.size() > 0){
        _faultRecordList.clear();
        for(auto iter = resp.begin(); iter != resp.end(); iter++){
            FaultRecordItem item{iter->at("timestamp"), iter->at("event")};
            _faultRecordList.push_back(item);
        }
    }
    return true;
}

void FaultRecordWind::update_table(){
    std::stringstream ss;
    for(int row = 1; row < _faultRecordList.size()&&row < ui->table_row; row++){
        //序号
        ss.str("");
        ss << (row);
        lv_table_set_cell_value(ui->screen_1_fault_record, row, 0, ss.str().c_str());
        //本地时间
        ss.str("");
        time_t timestamp = (time_t)atoi(std::next(_faultRecordList.begin(), row)->timestamp.c_str());
        struct tm* local_time = localtime(&timestamp);
        char formatted_time[100];
        size_t size = strftime(formatted_time, sizeof(formatted_time),
                            "%Y-%m-%d %H:%M:%S", local_time);
        if (size == 0) {
            LOG_ERROR << "Formating time error: " << timestamp;
        }
        ss << formatted_time;
        lv_table_set_cell_value(ui->screen_1_fault_record, row, 1, ss.str().c_str());
        ss.str("");
        ss << std::next(_faultRecordList.begin(), row)->evenet;
        lv_table_set_cell_value(ui->screen_1_fault_record, row, 2, ss.str().c_str());
    }
    lv_obj_align_to(ui->button_widget, ui->screen_1_fault_record, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
}

void FaultRecordWind::btn_event_cb(lv_event_t* e){
    WorkRecordWind* pWind = reinterpret_cast<WorkRecordWind*>(lv_event_get_user_data(e));
    lv_obj_t* btns = reinterpret_cast<lv_obj_t*>(lv_event_get_target(e));
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_VALUE_CHANGED){
        if(lv_btnmatrix_get_selected_btn(btns) == 0){
            pWind->load_first_table_record();
        }
        else if(lv_btnmatrix_get_selected_btn(btns) == 1){
            pWind->load_previous_table_record();
        }
        else if(lv_btnmatrix_get_selected_btn(btns) == 2){
            pWind->load_next_table_record();
        }
        pWind->update_table();
    }
}

/*==================ModifyPasswordWind==================*/

ModifyPasswordWind::ModifyPasswordWind(MainWind* pParentWind)
:_pKeyboard(Keyboard::getInstance()), _pParentWind(pParentWind),
ui(nullptr), _pIdentity(Identity::getInstance()){
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
    lv_obj_add_event_cb(ui->opassword_textarea, Keyboard::keyboard_event_cb, 
                        LV_EVENT_ALL, _pKeyboard);
    lv_obj_add_event_cb(ui->npassword_textarea, Keyboard::keyboard_event_cb,
                        LV_EVENT_ALL, _pKeyboard);
    lv_obj_add_event_cb(ui->cpassword_textarea, Keyboard::keyboard_event_cb,
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
            pWindow->_pIdentity->get_password(0, opassword);
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
                    if(pWindow->_pIdentity->modify_password(0, npwd)){
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


LoggingWind::LoggingWind(MainWind* pParent)
:_pParentWind(pParent), _pKeyboard(Keyboard::getInstance()),
_pIdentity(Identity::getInstance()){
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
    lv_obj_add_event_cb(ui->password_textarea, Keyboard::keyboard_event_cb,
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
                pWindow->_pIdentity->identity2manu();
            }
            else if(index == 1){
                std::string pwd = lv_textarea_get_text(pWindow->ui->password_textarea);
                if(pWindow->_pIdentity->identity2manager(pwd)){
                    LOG_DEBUG << "switch to manager";
                    //pWindow->_pParentWind->delete_modify_password_wind();
                    pWindow->hide();
                }
                else{
                    std::string msg("密码错误！");
                    pWindow->_messageWind.create_error_wind(msg);
                }
            }
            pWindow->_pParentWind->set_identity();
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

MainWind::MainWind():
_pPLCManager(LinkSettingWind::getPLCManagerInstance()),
_pWiFiManager(WIFISettingWind::getWiFiManagerInstance()),
_pModifyPasswordWind(ModifyPasswordWind::getInstance(this)),
_pLoggingWind(LoggingWind::getInstance(this)),
_pIdentity(Identity::getInstance()){
    /* 物理键盘 */
    keyboard_opa_t opa;
    memset(&opa, 0, sizeof(opa));
    keyboard_cb_t mainwind_cb = {
        .pPressedCallback = mainwindKeyPress_cb,
        .pReleasedCallback = mainwindKeyRelease_cb,
        .pData = this
    };
    keyboard_cb_t previouswind_cb = {
        .pPressedCallback = prewindKeyPress_cb,
        .pReleasedCallback = prewindKeyRelease_cb,
        .pData = this
    };
    keyboard_cb_t nextwind_cb = {
        .pPressedCallback = nextwindKeyPress_cb,
        .pReleasedCallback = nextwindKeyRelease_cb,
        .pData = this
    };
    keyboard_cb_t modifypwd_cb = {
        .pPressedCallback = modifyPwdPress_cb,
        .pReleasedCallback = modifyPwdRelease_cb,
        .pData = this
    };
    keyboard_cb_t loggingwind_cb = {
        .pPressedCallback = loggingKeyPress_cb,
        .pReleasedCallback = loggingKeyRelease_cb,
        .pData = this
    };
    keyboard_cb_t logginoutkey_cb = {
        .pPressedCallback = logginoutKeyPress_cb,
        .pReleasedCallback = logginoutKeyRelease_cb,
        .pData = this
    };
    KEYXCALLBACK(0);
    KEYXCALLBACK(1);
    KEYXCALLBACK(2);
    KEYXCALLBACK(3);
    KEYXCALLBACK(4);
    KEYXCALLBACK(5);
    KEYXCALLBACK(6);
    KEYXCALLBACK(7);
    KEYXCALLBACK(9);
    KEYXCALLBACK(10);
    KEYXCALLBACK(11);
    KEYXCALLBACK(12);
    KEYXCALLBACK(13);
    KEYXCALLBACK(14);
    KEYXCALLBACK(15);
    KEYXCALLBACK(18);
    KEYXCALLBACK(19);
    KEYXCALLBACK(20);
    KEYXCALLBACK(21);
    opa.key0_cb = key0_cb;
    opa.key1_cb = key1_cb;
    opa.key2_cb = key2_cb;
    opa.key3_cb = key3_cb;
    opa.key4_cb = key4_cb;
    opa.key5_cb = key5_cb;
    opa.key6_cb = key6_cb;
    opa.key7_cb = key7_cb;
    opa.key8_cb = loggingwind_cb;
    opa.key9_cb = key9_cb;
    opa.key10_cb = key10_cb;
    opa.key11_cb = key11_cb;
    opa.key12_cb = key12_cb;
    opa.key13_cb = key13_cb;
    opa.key14_cb = key14_cb;
    opa.key15_cb = key15_cb;
    opa.key16_cb = modifypwd_cb;
    opa.key17_cb = logginoutkey_cb;
    opa.key18_cb = key18_cb;
    opa.key19_cb = key19_cb;
    opa.key20_cb = key20_cb;
    opa.key21_cb = key21_cb;
    opa.key21_cb = mainwind_cb;
    opa.key22_cb = previouswind_cb;
    opa.key23_cb = nextwind_cb;
    _pPhyKeyboard = PhyKeyboard::getInstance(opa);
    /* 顶部部件 */
    static lv_style_t topbox_st;
    lv_style_init(&topbox_st);
    lv_style_set_width(&topbox_st, lv_pct(100));
    lv_style_set_text_font(&topbox_st, &lv_font_cn_songti_medium_21);
    lv_style_set_flex_flow(&topbox_st, LV_FLEX_FLOW_ROW);
    lv_style_set_flex_cross_place(&topbox_st, LV_FLEX_ALIGN_CENTER);
    lv_style_set_flex_track_place(&topbox_st, LV_FLEX_ALIGN_CENTER);
    lv_style_set_layout(&topbox_st, LV_LAYOUT_FLEX);
    lv_style_set_pad_all(&topbox_st, 0);
    lv_style_set_flex_grow(&topbox_st, 1);
    lv_style_set_border_width(&topbox_st, 0);
    // 图片部件style
    static lv_style_t img_label_box_style;
    lv_style_init(&img_label_box_style);
    lv_style_set_height(&img_label_box_style, lv_pct(100));
    lv_style_set_flex_grow(&img_label_box_style, 1);
    lv_style_set_flex_flow(&img_label_box_style, LV_FLEX_FLOW_ROW);
    lv_style_set_flex_main_place(&img_label_box_style, LV_FLEX_ALIGN_START);
    lv_style_set_flex_cross_place(&img_label_box_style, LV_FLEX_ALIGN_CENTER);
    lv_style_set_flex_track_place(&img_label_box_style, LV_FLEX_ALIGN_CENTER);
    lv_style_set_layout(&img_label_box_style, LV_LAYOUT_FLEX);
    lv_style_set_pad_all(&img_label_box_style, 0);
    lv_style_set_border_width(&img_label_box_style, 0);

    lv_obj_t* topbox = lv_obj_create(lv_scr_act());
    lv_obj_add_style(topbox, &topbox_st, 0);
    lv_obj_set_size(topbox, lv_pct(100), 50);
    //工作主页标签
    top_label = lv_label_create(topbox);
    //lv_label_set_text(top_label, "嘉盟电力设备");
    lv_obj_set_style_text_color(top_label, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_set_style_text_font(top_label, &lv_font_cn_songti_bold_25, LV_PART_MAIN);
    lv_obj_set_style_pad_left(top_label, 20, LV_STATE_DEFAULT);
    lv_obj_set_style_flex_grow(top_label, 3, 0);
    //时间部件
    top_datetime = lv_label_create(topbox);
    lv_label_set_text(top_datetime, "2024-10-28 18:00:00");
    lv_obj_set_style_text_font(top_datetime, &lv_font_cn_songti_medium_21, LV_PART_MAIN);
    lv_obj_set_style_flex_grow(top_datetime, 3, 0);
    lv_obj_set_style_pad_left(top_datetime, 20, 0);
    //材料容量
    lv_obj_t* material_volumn_box = lv_obj_create(topbox);
    lv_obj_add_style(material_volumn_box, &img_label_box_style, 0);
    lv_obj_t* material_img = lv_img_create(material_volumn_box);
    lv_img_set_src(material_img, &column_30x30);
    material_label = lv_label_create(material_volumn_box);
    lv_label_set_text(material_label, "50%");
    //电池容量
    lv_obj_t* battery_volumn_box = lv_obj_create(topbox);
    lv_obj_add_style(battery_volumn_box, &img_label_box_style, 0);
    battery_img = lv_img_create(battery_volumn_box);
    lv_img_set_src(battery_img, &battery_4);
    battery_label = lv_label_create(battery_volumn_box);
    lv_label_set_text(battery_label, "50%");
    /* 顶部部件 */

    /* 底部部件 */
    _tile_wind = lv_tileview_create(lv_scr_act());
    lv_obj_set_size(_tile_wind, lv_pct(100), 768 - 50 - 5);
    lv_obj_set_style_border_width(_tile_wind, 0, LV_STATE_DEFAULT);
    lv_obj_align_to(_tile_wind, topbox, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

    _pMainWind = lv_tileview_add_tile(_tile_wind, 0, 0, LV_DIR_RIGHT);

    /* 摄像头BOX */
    lv_obj_t* camera_box = lv_obj_create(_pMainWind);
    lv_obj_set_size(camera_box, lv_pct(100), lv_pct(50));
    lv_obj_set_style_outline_width(camera_box, 0, LV_STATE_DEFAULT);
    lv_obj_align(camera_box, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_pad_all(camera_box, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(camera_box, 4, LV_STATE_DEFAULT);
    lv_obj_clear_flag(camera_box, LV_OBJ_FLAG_SCROLLABLE);
    //左图像
    _left_img_box = lv_obj_create(camera_box);
    lv_obj_set_size(_left_img_box, lv_pct(50), lv_pct(100));
    lv_obj_align(_left_img_box, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_scrollbar_mode(_left_img_box, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_pad_all(_left_img_box, 0, LV_STATE_DEFAULT);
    lv_obj_clear_flag(_left_img_box, LV_OBJ_FLAG_SCROLLABLE);
    left_img_domain = lv_img_create(_left_img_box);
    camera_set_image(left_img_domain, CAMERA_STATE_NO);
    //右图像
    _right_img_box = lv_obj_create(camera_box);
    lv_obj_set_size(_right_img_box, lv_pct(50), lv_pct(100));
    //lv_obj_align(_right_img_box, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_align_to(_right_img_box, _left_img_box, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    lv_obj_set_scrollbar_mode(_right_img_box, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_pad_all(_right_img_box, 0, LV_STATE_DEFAULT);
    lv_obj_clear_flag(_right_img_box, LV_OBJ_FLAG_SCROLLABLE);
    right_img_domain = lv_img_create(_right_img_box);
    camera_set_image(right_img_domain, CAMERA_STATE_NO);

    /* 消息框BOX */
    /* 行style */
    static lv_style_t row_style;
    lv_style_init(&row_style);
    lv_style_set_width(&row_style, lv_pct(100));
    lv_style_set_text_font(&row_style, &lv_font_cn_songti_medium_21);
    lv_style_set_flex_flow(&row_style, LV_FLEX_FLOW_ROW);
    lv_style_set_flex_cross_place(&row_style, LV_FLEX_ALIGN_CENTER);
    lv_style_set_flex_track_place(&row_style, LV_FLEX_ALIGN_CENTER);
    lv_style_set_layout(&row_style, LV_LAYOUT_FLEX);
    lv_style_set_pad_all(&row_style, 0);
    lv_style_set_flex_grow(&row_style, 1);
    lv_style_set_border_width(&row_style, 0);

    /* 数据box style */
    static lv_style_t data_show_box_style;
    lv_style_init(&data_show_box_style);
    lv_style_set_height(&data_show_box_style, lv_pct(100));
    lv_style_set_flex_grow(&data_show_box_style, 10);

    /* 状态box style */
    static lv_style_t status_show_box_style;
    lv_style_init(&status_show_box_style);
    lv_style_set_height(&status_show_box_style, lv_pct(100));
    lv_style_set_flex_grow(&status_show_box_style, 7);
    lv_style_set_flex_flow(&status_show_box_style, LV_FLEX_FLOW_ROW);
    lv_style_set_flex_main_place(&status_show_box_style, LV_FLEX_ALIGN_START);
    lv_style_set_flex_cross_place(&status_show_box_style, LV_FLEX_ALIGN_CENTER);
    lv_style_set_flex_track_place(&status_show_box_style, LV_FLEX_ALIGN_CENTER);
    lv_style_set_layout(&status_show_box_style, LV_LAYOUT_FLEX);
    lv_style_set_pad_all(&status_show_box_style, 0);
    lv_style_set_border_width(&status_show_box_style, 0);

    lv_obj_t* message_box = lv_obj_create(_pMainWind);
    lv_obj_set_size(message_box, lv_pct(100), lv_pct(50));
    lv_obj_set_style_outline_width(message_box, 0, LV_STATE_DEFAULT);
    lv_obj_align(message_box, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_pad_all(message_box, 0, LV_STATE_DEFAULT);
    //lv_obj_set_style_pad_row(message_box, 4, LV_STATE_DEFAULT);
    lv_obj_clear_flag(message_box, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_flex_flow(message_box, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(message_box, 0, LV_STATE_DEFAULT);

    /* 运行信息 */
    lv_obj_t* running_info_box = lv_obj_create(message_box);
    lv_obj_add_style(running_info_box, &row_style, 0);
    lv_obj_set_style_bg_color(running_info_box, lv_palette_main(LV_PALETTE_YELLOW), 0);

    lv_obj_t* running_info_label = lv_label_create(running_info_box);
    lv_obj_set_style_pad_all(running_info_label, 0, 0);
    lv_label_set_text(running_info_label, "运行信息：");
    lv_obj_set_style_text_font(running_info_label, &lv_font_cn_songti_bold, 0);
    running_info = lv_textarea_create(running_info_box);
    lv_obj_add_style(running_info, &data_show_box_style, 0);
    lv_obj_set_style_bg_color(running_info, lv_palette_main(LV_PALETTE_YELLOW), 0);
    lv_obj_set_style_border_width(running_info, 0, 0);
    lv_textarea_set_one_line(running_info, true);
    lv_obj_set_style_flex_grow(running_info, 11, 0);

    /* row 0*/
    lv_obj_t* row0_box = lv_obj_create(message_box);
    lv_obj_add_style(row0_box, &row_style, 0);

    row0_ptr_list[3];
    for (int i = 0; i < 3; i++) {
        row0_ptr_list[i] = lv_textarea_create(row0_box);
        lv_obj_add_style(row0_ptr_list[i], &data_show_box_style, 0);
        lv_textarea_set_one_line(row0_ptr_list[i], true);
    }
    lv_textarea_set_text(row0_ptr_list[0], "行走速度：");
    lv_textarea_set_text(row0_ptr_list[1], "顶漆速度：");
    lv_textarea_set_text(row0_ptr_list[2], "自动涂覆长度：");

    /* row 1*/
    lv_obj_t* row1_box = lv_obj_create(message_box);
    lv_obj_add_style(row1_box, &row_style, 0);

    row1_ptr_list[3];
    for (int i = 0; i < 3; i++) {
        row1_ptr_list[i] = lv_textarea_create(row1_box);
        lv_obj_add_style(row1_ptr_list[i], &data_show_box_style, 0);
        lv_textarea_set_one_line(row1_ptr_list[i], true);
    }
    lv_textarea_set_text(row1_ptr_list[0], "涂覆厚度：");
    lv_textarea_set_text(row1_ptr_list[1], "电缆线径：");
    lv_textarea_set_text(row1_ptr_list[2], "手动涂覆长度：");

    /* row 2*/
    lv_obj_t* row2_box = lv_obj_create(message_box);
    lv_obj_add_style(row2_box, &row_style, 0);

    current_angle = lv_textarea_create(row2_box);
    lv_obj_add_style(current_angle, &data_show_box_style, 0);
    lv_textarea_set_one_line(current_angle, true);
    lv_textarea_set_text(current_angle, "当前角度：");

    lv_obj_t* status_box_ptr_list[3];
    lv_obj_t* status_label_ptr_list[3];
    status_image_ptr_list[3];
    for (int i = 0; i < 3; i++) {
        lv_obj_t* box_ptr = lv_obj_create(row2_box);
        lv_obj_add_style(box_ptr, &status_show_box_style, 0);

        lv_obj_t* label_ptr = lv_label_create(box_ptr);
        lv_obj_t* image_ptr = lv_img_create(box_ptr);
        lv_img_set_src(image_ptr, &red_point);
        //lv_img_set_zoom(image_ptr, 32);
        status_box_ptr_list[i] = box_ptr;
        status_label_ptr_list[i] = label_ptr;
        status_image_ptr_list[i] = image_ptr;
    }
    lv_label_set_text(status_label_ptr_list[0], "上升左限位");
    lv_label_set_text(status_label_ptr_list[1], "顶漆上限位");
    lv_label_set_text(status_label_ptr_list[2], "WiFi");
    _wifi_box = status_box_ptr_list[2];

    /* row 3 */
    lv_obj_t* row3_box = lv_obj_create(message_box);
    lv_obj_add_style(row3_box, &row_style, 0);

    battery = lv_textarea_create(row3_box);
    lv_obj_add_style(battery, &data_show_box_style, 0);
    lv_textarea_set_one_line(battery, true);
    lv_textarea_set_text(battery, "电池电压：");

    lv_obj_t* row3_status_box_ptr_list[3];
    lv_obj_t* row3_status_label_ptr_list[3];
    row3_status_image_ptr_list[3];
    for (int i = 0; i < 3; i++) {
        lv_obj_t* box_ptr = lv_obj_create(row3_box);
        lv_obj_add_style(box_ptr, &status_show_box_style, 0);

        lv_obj_t* label_ptr = lv_label_create(box_ptr);
        lv_obj_t* image_ptr = lv_img_create(box_ptr);;
        lv_img_set_src(image_ptr, &red_point);
        //lv_img_set_zoom(image_ptr, 32);
        row3_status_box_ptr_list[i] = box_ptr;
        row3_status_label_ptr_list[i] = label_ptr;
        row3_status_image_ptr_list[i] = image_ptr;
    }
    lv_label_set_text(row3_status_label_ptr_list[0], "上升右限位");
    lv_label_set_text(row3_status_label_ptr_list[1], "顶漆下限位");
    lv_label_set_text(row3_status_label_ptr_list[2], "Link");
    _link_box = row3_status_box_ptr_list[2];

    /* 开启摄像头 */
    camera1 = new Camera(left_img_domain, "camera1");
    camera2 = new Camera(right_img_domain, "camera2");

    left_cameraSettingWind = new CameraSettingWind(camera1);
    right_cameraSettingWind = new CameraSettingWind(camera2);

    /* 创建子窗口 */
    lv_obj_t* model_tile = lv_tileview_add_tile(_tile_wind, 1, 0, LV_DIR_LEFT | LV_DIR_RIGHT);
    lv_obj_t* workrecord_tile = lv_tileview_add_tile(_tile_wind, 2, 0, LV_DIR_LEFT | LV_DIR_RIGHT);
    lv_obj_t* faultrecord_tile = lv_tileview_add_tile(_tile_wind, 3, 0, LV_DIR_LEFT | LV_DIR_RIGHT);
    lv_obj_t* param1_tile = lv_tileview_add_tile(_tile_wind, 4, 0, LV_DIR_LEFT | LV_DIR_RIGHT);
    lv_obj_t* param2_tile = lv_tileview_add_tile(_tile_wind, 5, 0, LV_DIR_LEFT);
    _pParam1Wind = Param1SettingWind::getInstance(param1_tile);
    _pParam2Wind = Param2SettingWind::getInstance(param2_tile);
    _pModelWind = ModelSettingWind::getInstance(model_tile);
    _pWorkRecordWind = WorkRecordWind::getInstance(workrecord_tile);
    _pFultRecordWind = FaultRecordWind::getInstance(faultrecord_tile);

    _wifi_setting_wind = new WIFISettingWind(camera1, camera2);
    PLCManager::ConnectStateCallback_t callback;
    callback.connect_success_cb = MainWind::plc_connection_success_callback;
    callback.connect_faild_cb = MainWind::plc_connection_fail_callback;
    callback.data = this;
    _pPLCManager->set_callback(callback);
    _link_setting_wind = new LinkSettingWind();

    //连接wifi
    //wifiManager.setUP(true);
    WifiManager::ConnectStateCallback_t conn_callback;
    conn_callback.connect_success_cb = MainWind::wifi_connection_success_callback;
    conn_callback.connect_faild_cb = MainWind::wifi_connection_fail_callback;
    conn_callback.data = this;
    _pWiFiManager->set_callback(conn_callback);
    _pWiFiManager->connect_async();

    set_identity();
    /* 添加事件 */
    event_init_screen();
}

void MainWind::event_init_screen(){
    lv_obj_add_event_cb(_left_img_box, MainWind::image_source_event_cb, LV_EVENT_LONG_PRESSED, left_cameraSettingWind);
    lv_obj_add_event_cb(_right_img_box, MainWind::image_source_event_cb, LV_EVENT_LONG_PRESSED, right_cameraSettingWind);
    lv_obj_add_event_cb(_wifi_box, MainWind::setting_event_cb, LV_EVENT_CLICKED, _wifi_setting_wind);
    lv_obj_add_event_cb(_link_box, MainWind::link_event_cb, LV_EVENT_CLICKED, _link_setting_wind);

    _read_tasks = std::map<uint32_t, BoxValueItem>{
        std::make_pair((uint32_t)row0_ptr_list[0],
                    BoxValueItem{row0_ptr_list[0], "D144", REG_TYPE_FLOAT, REG_TYPE_FLOAT, calculate_nothing, ofmt_set_textarea<float>, (void*)"行走速度："}),
        std::make_pair((uint32_t)row0_ptr_list[1],
                    BoxValueItem{row0_ptr_list[1], "D146", REG_TYPE_FLOAT, REG_TYPE_FLOAT, calculate_nothing, ofmt_set_textarea<float>, (void*)"顶漆速度："}),
        std::make_pair((uint32_t)row0_ptr_list[2],
                    BoxValueItem{row0_ptr_list[2], "D148", REG_TYPE_FLOAT, REG_TYPE_FLOAT, calculate_nothing, ofmt_set_textarea<float>, (void*)"自动涂覆长度："}),
        std::make_pair((uint32_t)row1_ptr_list[0],
                    BoxValueItem{row1_ptr_list[0], "D142", REG_TYPE_FLOAT, REG_TYPE_FLOAT, calculate_nothing, ofmt_set_textarea<float>, (void*)"涂覆厚度："}),
        std::make_pair((uint32_t)row1_ptr_list[1],
                    BoxValueItem{row1_ptr_list[1], "D140", REG_TYPE_FLOAT, REG_TYPE_FLOAT, calculate_nothing, ofmt_set_textarea<float>, (void*)"电缆线径："}),
        std::make_pair((uint32_t)row1_ptr_list[2],
                    BoxValueItem{row1_ptr_list[2], "D150", REG_TYPE_FLOAT, REG_TYPE_FLOAT, calculate_nothing, ofmt_set_textarea<float>, (void*)"手动涂覆长度："}),
        std::make_pair((uint32_t)current_angle,
                    BoxValueItem{current_angle, "D376", REG_TYPE_FLOAT, REG_TYPE_FLOAT, calculate_nothing, ofmt_set_textarea<float>, (void*)"当前角度："}),
        std::make_pair((uint32_t)battery,
                    BoxValueItem{battery, "D156", REG_TYPE_FLOAT, REG_TYPE_FLOAT, calculate_nothing, ofmt_set_textarea_divide_10<float>, (void*)"电池电压："}),
        std::make_pair((uint32_t)status_image_ptr_list[0],
                    BoxValueItem{status_image_ptr_list[0], "M194", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_bool, (void*)""}),
        std::make_pair((uint32_t)status_image_ptr_list[1],
                    BoxValueItem{status_image_ptr_list[1], "M196", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_bool, (void*)""}),
        std::make_pair((uint32_t)row3_status_image_ptr_list[0],
                    BoxValueItem{row3_status_image_ptr_list[0], "M195", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_bool, (void*)""}),
        std::make_pair((uint32_t)row3_status_image_ptr_list[1],
                    BoxValueItem{row3_status_image_ptr_list[1], "M197", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_bool, (void*)""}),
        std::make_pair(0, 
                    BoxValueItem{this, "M360", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_running_info, (void*)"M360"}),
        std::make_pair(1, 
                    BoxValueItem{this, "M361", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_running_info, (void*)"M361"}),
        std::make_pair(2, 
                    BoxValueItem{this, "M362", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_running_info, (void*)"M362"}),
        std::make_pair(3, 
                    BoxValueItem{this, "M363", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_running_info, (void*)"M363"}),
        std::make_pair(4, 
                    BoxValueItem{this, "M364", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_running_info, (void*)"M364"}),
        std::make_pair(5, 
                    BoxValueItem{this, "M365", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_running_info, (void*)"M365"}),
        std::make_pair(6, 
                    BoxValueItem{this, "M366", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_running_info, (void*)"M366"}),
        std::make_pair(7, 
                    BoxValueItem{this, "M367", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_running_info, (void*)"M367"}),
        std::make_pair(8, 
                    BoxValueItem{this, "M368", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_running_info, (void*)"M368"}),
        std::make_pair(9, 
                    BoxValueItem{this, "M369", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_running_info, (void*)"M369"}),
        std::make_pair(10, 
                    BoxValueItem{this, "M370", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_running_info, (void*)"M370"}),
        std::make_pair(11, 
                    BoxValueItem{this, "M371", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_running_info, (void*)"M371"}),
        std::make_pair(12, 
                    BoxValueItem{this, "M372", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_running_info, (void*)"M372"}),
        std::make_pair(13, 
                    BoxValueItem{this, "M373", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_running_info, (void*)"M373"}),
        std::make_pair(14, 
                    BoxValueItem{this, "M374", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_running_info, (void*)"M374"}),
        std::make_pair(15, 
                    BoxValueItem{this, "M375", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_running_info, (void*)"M375"}),
        std::make_pair(16, 
                    BoxValueItem{this, "M376", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_running_info, (void*)"M376"}),
        std::make_pair(17, 
                    BoxValueItem{this, "M377", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_running_info, (void*)"M377"}),
        std::make_pair(18, 
                    BoxValueItem{this, "M380", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_running_info, (void*)"M380"}),
        std::make_pair(19, 
                    BoxValueItem{this, "M381", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_running_info, (void*)"M381"}),
        std::make_pair(20, 
                    BoxValueItem{this, "M382", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_running_info, (void*)"M382"}),
        std::make_pair(21, 
                    BoxValueItem{this, "M383", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_running_info, (void*)"M383"}),
        std::make_pair(22, 
                    BoxValueItem{_pPhyKeyboard, "M3030", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_keyboard, (void*)0}),
        std::make_pair(23, 
                    BoxValueItem{_pPhyKeyboard, "M3031", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_keyboard, (void*)1}),
        std::make_pair(24, 
                    BoxValueItem{_pPhyKeyboard, "M3032", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_keyboard, (void*)2}),
        std::make_pair(25, 
                    BoxValueItem{_pPhyKeyboard, "M3033", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_keyboard, (void*)3}),
        std::make_pair(26, 
                    BoxValueItem{_pPhyKeyboard, "M3034", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_keyboard, (void*)4}),
        std::make_pair(27, 
                    BoxValueItem{_pPhyKeyboard, "M3035", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_keyboard, (void*)5}),
        std::make_pair(28, 
                    BoxValueItem{_pPhyKeyboard, "M3036", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_keyboard, (void*)6}),
        std::make_pair(29, 
                    BoxValueItem{_pPhyKeyboard, "M3037", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_keyboard, (void*)7}),
        std::make_pair(30, 
                    BoxValueItem{_pPhyKeyboard, "M3038", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_keyboard, (void*)8}),
        std::make_pair(31, 
                    BoxValueItem{_pPhyKeyboard, "M3039", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_keyboard, (void*)9}),
        std::make_pair(32, 
                    BoxValueItem{_pPhyKeyboard, "M3040", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_keyboard, (void*)10}),
        std::make_pair(33, 
                    BoxValueItem{_pPhyKeyboard, "M3041", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_keyboard, (void*)11}),
        std::make_pair(34, 
                    BoxValueItem{_pPhyKeyboard, "M3042", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_keyboard, (void*)12}),
        std::make_pair(35, 
                    BoxValueItem{_pPhyKeyboard, "M3043", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_keyboard, (void*)13}),
        std::make_pair(36, 
                    BoxValueItem{_pPhyKeyboard, "M3044", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_keyboard, (void*)14}),
        std::make_pair(37, 
                    BoxValueItem{_pPhyKeyboard, "M3045", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_keyboard, (void*)15}),
        std::make_pair(38, 
                    BoxValueItem{_pPhyKeyboard, "M3046", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_keyboard, (void*)16}),
        std::make_pair(39, 
                    BoxValueItem{_pPhyKeyboard, "M3047", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_keyboard, (void*)17}),
        std::make_pair(40, 
                    BoxValueItem{_pPhyKeyboard, "M3048", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_keyboard, (void*)18}),
        std::make_pair(41, 
                    BoxValueItem{_pPhyKeyboard, "M3049", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_keyboard, (void*)19}),
        std::make_pair(42, 
                    BoxValueItem{_pPhyKeyboard, "M3050", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_set_keyboard, (void*)20}),
        std::make_pair(43, 
                    BoxValueItem{this, "M310", REG_TYPE_BIT, REG_TYPE_BIT, calculate_nothing, ofmt_work_record, (void*)"M310"}),

    };

    //注册任务
    for(auto ptr = _read_tasks.begin();
        ptr != _read_tasks.end(); ptr++){
        WindTask task = WindTask(ptr->second);
        task.is_read = true;
        persistWindTask.register_task(ptr->first, task);
    }
    BoxValueItem item = {
        .target = this,
        .address = "D160",
        .reg_type = REG_TYPE_BIT,
        .tar_type = REG_TYPE_BIT,
        .ifmt = ifmt_write_1,
        .ofmt = ofmt_nothing,
        .data = NULL,
    };
    WindTask writeTask = WindTask(item);
    writeTask.is_read = false;
    persistWindTask.register_task(456, writeTask);


    pthread_create(&sys_clock_pid, NULL, MainWind::sys_clock_handler, this);
    pthread_create(&plc_trans_pid, NULL, MainWind::mainWind_value_update_thread, this);
}

void MainWind::camera_set_image(lv_obj_t* img_domain, CameraState state){
    if(state == CAMERA_STATE_NO){
        lv_img_set_src(img_domain, &no_camera_200x200);
        lv_obj_set_size(img_domain, 200, 200);
        lv_obj_align(img_domain, LV_ALIGN_CENTER, 0, 0);
    }
    else if(state == CAMERA_STATE_LOADING){
        lv_img_set_src(img_domain, &loading_200x200);
        lv_obj_set_size(img_domain, 200, 200);
        lv_obj_align(img_domain, LV_ALIGN_CENTER, 0, 0);
    }
    else if(state == CAMERA_STATE_PLAYING){
        lv_obj_set_size(img_domain, lv_pct(100), lv_pct(100));
    }
}

void MainWind::set_running_info(const std::string & info_str){
    pthread_mutex_lock(&lv_lock);
    lv_textarea_set_text(running_info, info_str.c_str());
    pthread_mutex_unlock(&lv_lock);
}

void MainWind::read_fault(const std::string & reg){
    bool fault;
    if(_pPLCManager->is_connected() == false)
        return;
    pthread_mutex_lock(&_pPLCManager->rw_lock);
    err_code_t ret = _pPLCManager->read_bool(reg, &fault);
    pthread_mutex_unlock(&_pPLCManager->rw_lock);
    if(ret == 0){
        if(fault){
            auto item_ptr = FAULT_MESSAGE.find(reg);
            if(item_ptr == FAULT_MESSAGE.end())
                return;
            set_running_info(FAULT_MESSAGE.find(reg)->second);
        }
    }
    //掉线
    else{
        _pPLCManager->reconnect();
    }
}

void MainWind::read_indict_led(int led_index){
    bool turn = 0;
    auto item_ptr = KEY_LED_PLC_REG_MAP.find(led_index);
    if(item_ptr == KEY_LED_PLC_REG_MAP.end())
        return;
    if(_pPLCManager->is_connected() == false)
        return;
    pthread_mutex_lock(&_pPLCManager->rw_lock);
    err_code_t ret = _pPLCManager->read_bool(item_ptr->second, &turn);
    pthread_mutex_unlock(&_pPLCManager->rw_lock);
    if(ret == 0){
        _pPhyKeyboard->keyboard_led_turn(led_index, turn);
    }
    //掉线
    else{
        _pPLCManager->reconnect();
    }
}


void MainWind::set_identity(){
    if(_pIdentity->curent_identity() == MANUPULATOR){
        lv_label_set_text(top_label, "用户：(操作员)");
        _pParam1Wind->set_disable(true);
        _pParam2Wind->set_disable(true);
    }
    else if(_pIdentity->curent_identity() == MANAGER){
        lv_label_set_text(top_label, "用户：(管理员)");
        _pParam1Wind->set_disable(false);
        _pParam2Wind->set_disable(false);
    }
}

void* MainWind::sys_clock_handler(void* args){
    using namespace std;
    char time_str[64]{0};
    int time_count = 30;
    MainWind* pWind = (MainWind*)args;
    while(1){
        //设置时间
        auto time_stamp = time(NULL);
        auto* time_tm = localtime(&time_stamp);
        memset(time_str, 0, sizeof(time_str));
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", time_tm);
        pthread_mutex_lock(&lv_lock);
        lv_label_set_text(pWind->top_datetime, time_str);
        pthread_mutex_unlock(&lv_lock);
        //读PLC
        if(pWind->_pPLCManager->is_connected()){
            //pWind->read_plc_value();
        }
        if(++time_count >= 30){
            //更新材料容量

            //更新电池电量
            fstream fs;
            double adc_value;
            fs.open(BATTRERY_ADC_FS, ios::in);
            fs >> adc_value;
            double pct = (adc_value - 1400)/(1800-1400);
            int battery_val = static_cast<int>(pct * 100);
            pthread_mutex_lock(&lv_lock);
            lv_label_set_text_fmt(pWind->battery_label, "%d%", battery_val);
            pthread_mutex_unlock(&lv_lock);
            fs.close();
            time_count = 0;

        }
        this_thread::sleep_for(std::chrono::seconds(1));
    }
    pthread_exit(NULL);
}


void* MainWind::mainWind_value_update_thread(void* args){
    MainWind* pWind = reinterpret_cast<MainWind*>(args);
    while(1){
        if(pWind->_pPLCManager->is_connected() == false){
            sleep(1);
            continue;
        }
        pthread_mutex_lock(&pWind->_pPLCManager->rw_lock);
        for(auto ptr = persistWindTask.begin();
            ptr != persistWindTask.end(); ptr++){
            PLCTask task;
            task.id = (uint32_t)ptr->first;
            task.address = ptr->second.address;
            task.is_read = ptr->second.is_read;
            task.reg_type = ptr->second.reg_type;
            if(!task.is_read){
                if(ptr->second.tar_type == REG_TYPE_FLOAT){
                    ptr->second.ifmt(&task.value.fval, ptr->second.target);
                }
                else if(ptr->second.tar_type == REG_TYPE_UINT16){
                    ptr->second.ifmt(&task.value.u16val, ptr->second.target);
                }
                else if(ptr->second.tar_type == REG_TYPE_UINT32){
                    ptr->second.ifmt(&task.value.u32val, ptr->second.target);
                }
                else if(ptr->second.tar_type == REG_TYPE_BIT){
                    ptr->second.ifmt(&task.value.bval, ptr->second.target);
                }
            }
            pWind->_pPLCManager->add_task(task);
        }
        for(auto ptr = tempWindTask.begin();
            ptr != tempWindTask.end(); ptr++){
            PLCTask task;
            task.id = (uint32_t)ptr->first;
            task.address = ptr->second.address;
            task.is_read = ptr->second.is_read;
            task.reg_type = ptr->second.reg_type;
            if(!task.is_read){
                if(ptr->second.tar_type == REG_TYPE_FLOAT){
                    ptr->second.ifmt(&task.value.fval, ptr->second.target);
                }
                else if(ptr->second.tar_type == REG_TYPE_UINT16){
                    ptr->second.ifmt(&task.value.u16val, ptr->second.target);
                }
                else if(ptr->second.tar_type == REG_TYPE_UINT32){
                    ptr->second.ifmt(&task.value.u32val, ptr->second.target);
                }
                else if(ptr->second.tar_type == REG_TYPE_BIT){
                    ptr->second.ifmt(&task.value.bval, ptr->second.target);
                }
            }
            pWind->_pPLCManager->add_task(task);
        }
        pthread_cond_wait(&pWind->_pPLCManager->rw_cond, &pWind->_pPLCManager->rw_lock);
        for(auto ptr = pWind->_pPLCManager->get_result().begin();
            ptr != pWind->_pPLCManager->get_result().end(); ptr++){
            BoxValueItem* pBoxItem = nullptr;
            auto pTask = persistWindTask.find(ptr->id);
            if( pTask != persistWindTask.end()){
                pBoxItem = &pTask->second;
                pBoxItem->ofmt((void*)&ptr->value, pBoxItem->target, pBoxItem->data);
            }
            pTask = tempWindTask.find(ptr->id);
            if( pTask != tempWindTask.end()){
                if(ptr->is_read){
                    pBoxItem = &pTask->second;
                    pBoxItem->ofmt((void*)&ptr->value, pBoxItem->target, pBoxItem->data);
                }
                tempWindTask.unregister_task(pTask->first);
            }
        }
        pWind->_pPLCManager->get_result().clear();
        pthread_mutex_unlock(&pWind->_pPLCManager->rw_lock);
        pWind->_pPhyKeyboard->keyboard_led_update();
        usleep(1000);
    }
}

void MainWind::plc_connection_success_callback(void* args){
    MainWind* pWind = (MainWind*)args;
    pWind->_pParam1Wind->read_value();
    pWind->_pParam2Wind->read_value();
    pWind->_pModelWind->read_value();
    lv_img_set_src(pWind->row3_status_image_ptr_list[2], &green_point);
}

void MainWind::plc_connection_fail_callback(void* args){
    MainWind* pWind = (MainWind*)args;
    lv_img_set_src(pWind->row3_status_image_ptr_list[2], &red_point);
}

void MainWind::wifi_connection_success_callback(void* args){
    MainWind* pWind = (MainWind*)args;
    lv_img_set_src(pWind->status_image_ptr_list[2], &green_point);
    pWind->_pWiFiManager->setMask("255.255.0.0");
    //连接摄像头
    pWind->camera1->camera_connect_async(MainWind::lcamera_connection_connecting_cb,
                                MainWind::lcamera_connection_success_cb,
                                MainWind::lcamera_connection_fail_cb, pWind);
    
    pWind->camera2->camera_connect_async(MainWind::rcamera_connection_connection_cb,
                                MainWind::rcamera_connection_success_cb,
                                MainWind::rcamera_connection_fail_cb, pWind);
    
    //连接PLC
    PLCManager::ConnectStateCallback_t callback;
    callback.connect_success_cb = MainWind::plc_connection_success_callback;
    callback.connect_faild_cb = MainWind::plc_connection_fail_callback;
    callback.data = pWind;
    pWind->_pPLCManager->set_callback(callback);
    pWind->_link_setting_wind->plc_connect();
}

void MainWind::wifi_connection_fail_callback(void* args){
    MainWind* pWind = (MainWind*)args;

    MainWind::lcamera_connection_fail_cb(pWind);
    lv_img_set_src(pWind->status_image_ptr_list[2], &red_point);
}

void MainWind::setting_event_cb(lv_event_t* e){
    lv_event_code_t code = lv_event_get_code(e);
    WIFISettingWind* pWind = (WIFISettingWind*)lv_event_get_user_data(e);

    if (code = LV_EVENT_CLICKED) {
        pWind->show();
    }
}

void MainWind::link_event_cb(lv_event_t* e){
    lv_event_code_t code = lv_event_get_code(e);
    LinkSettingWind* pWind = (LinkSettingWind*)lv_event_get_user_data(e);
    if (code = LV_EVENT_CLICKED) {
        pWind->show();
    }
}

void MainWind::image_source_event_cb(lv_event_t* e){
    lv_event_code_t code = lv_event_get_code(e);
    CameraSettingWind* pWind = (CameraSettingWind*)lv_event_get_user_data(e);

    if (code = LV_EVENT_PRESSING) {
        pWind->show();
    }
}

void MainWind::lcamera_connection_connecting_cb(void* args){
    MainWind* pWind = reinterpret_cast<MainWind*>(args);
    pWind->camera_set_image(pWind->left_img_domain, CAMERA_STATE_LOADING);
}

void MainWind::lcamera_connection_success_cb(void* args){
    MainWind* pWind = reinterpret_cast<MainWind*>(args);
    pWind->camera_set_image(pWind->left_img_domain, CAMERA_STATE_PLAYING);
}

void MainWind::lcamera_connection_fail_cb(void* args){
    MainWind* pWind = reinterpret_cast<MainWind*>(args);
    pWind->camera_set_image(pWind->left_img_domain, CAMERA_STATE_NO);
}

void MainWind::rcamera_connection_connection_cb(void* args){
    MainWind* pWind = reinterpret_cast<MainWind*>(args);
    pWind->camera_set_image(pWind->right_img_domain, CAMERA_STATE_LOADING);
}

void MainWind::rcamera_connection_success_cb(void* args){
    MainWind* pWind = reinterpret_cast<MainWind*>(args);
    pWind->camera_set_image(pWind->right_img_domain, CAMERA_STATE_PLAYING);
}

void MainWind::rcamera_connection_fail_cb(void* args){
    MainWind* pWind = reinterpret_cast<MainWind*>(args);
    pWind->camera_set_image(pWind->right_img_domain, CAMERA_STATE_NO);
}


/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

MainWind* lv_main_widgets(){
    auto ptr = MainWind::getInstance();
    return ptr;
}

void MainWind::mainwindKeyPress_cb(void* pWind){
    MainWind* _pWind = (MainWind*)pWind;
    _pWind->_tile_index = 0;
    _pWind->_pPhyKeyboard->keyboard_led_turn(21, true);
    _pWind->_pPhyKeyboard->keyboard_led_update();
    lv_obj_set_tile_id(_pWind->_tile_wind, _pWind->_tile_index, 0, LV_ANIM_ON);
    printf("main window key pressed.\n");

}
void MainWind::mainwindKeyRelease_cb(void* pWind){
    MainWind* _pWind = (MainWind*)pWind;
    _pWind->_pPhyKeyboard->keyboard_led_turn(21, false);
    _pWind->_pPhyKeyboard->keyboard_led_update();
    printf("main window key released.\n");
}

void MainWind::prewindKeyPress_cb(void* pWind){
    MainWind* _pWind = (MainWind*)pWind;
    --_pWind->_tile_index;
    _pWind->_pPhyKeyboard->keyboard_led_turn(22, true);
    _pWind->_pPhyKeyboard->keyboard_led_update();
    if(_pWind->_tile_index < 0) _pWind->_tile_index = 0;
    lv_obj_set_tile_id(_pWind->_tile_wind, _pWind->_tile_index, 0, LV_ANIM_ON);
    printf("previous window key pressed.\n");
}

void MainWind::prewindKeyRelease_cb(void* pWind){
    MainWind* _pWind = (MainWind*)pWind;
    _pWind->_pPhyKeyboard->keyboard_led_turn(22, false);
    _pWind->_pPhyKeyboard->keyboard_led_update();
    printf("previous window key released.\n");
}

void MainWind::nextwindKeyPress_cb(void* pWind){
    MainWind* _pWind = (MainWind*)pWind;
    ++_pWind->_tile_index;
    _pWind->_pPhyKeyboard->keyboard_led_turn(23, true);
    _pWind->_pPhyKeyboard->keyboard_led_update();
    if(_pWind->_tile_index > 5)_pWind->_tile_index = 5;
    lv_obj_set_tile_id(_pWind->_tile_wind, _pWind->_tile_index, 0, LV_ANIM_ON);
    printf("next window key pressed.\n");
}

void MainWind::nextwindKeyRelease_cb(void* pWind){
    MainWind* _pWind = (MainWind*)pWind;
    _pWind->_pPhyKeyboard->keyboard_led_turn(23, false);
    _pWind->_pPhyKeyboard->keyboard_led_update();
    printf("next window key released.\n");
}

void MainWind::modifyPwdPress_cb(void* pWind){
    MainWind* _pWind = (MainWind*)pWind;
    _pWind->_pPhyKeyboard->keyboard_led_turn(16, true);
    _pWind->_pPhyKeyboard->keyboard_led_update();
    printf("modify password key pressed.\n");
    if(_pWind){
        _pWind->_pModifyPasswordWind->show();
    }
}

void MainWind::modifyPwdRelease_cb(void* pWind){
    MainWind* _pWind = (MainWind*)pWind;
    _pWind->_pPhyKeyboard->keyboard_led_turn(16, false);
    _pWind->_pPhyKeyboard->keyboard_led_update();
    printf("modify password key released.\n");
}

void MainWind::loggingKeyPress_cb(void* pWind){
    MainWind* _pWind = (MainWind*)pWind;
    _pWind->_pPhyKeyboard->keyboard_led_turn(8, true);
    _pWind->_pPhyKeyboard->keyboard_led_update();
    printf("logging key pressed.\n");
    if(_pWind){
        _pWind->_pLoggingWind->show();
    }
}

void MainWind::loggingKeyRelease_cb(void* pWind){
    MainWind* _pWind = (MainWind*)pWind;
    _pWind->_pPhyKeyboard->keyboard_led_turn(8, false);
    _pWind->_pPhyKeyboard->keyboard_led_update();
    printf("logging key released.\n");
}

void MainWind::logginoutKeyPress_cb(void* pWind){
    MainWind* _pWind = (MainWind*)pWind;
    printf("logging out key pressed.\n");
    if(_pWind){
        _pWind->_pIdentity->identity2manu();
        _pWind->set_identity();
    }
}

void MainWind::logginoutKeyRelease_cb(void* pWind){
    MainWind* _pWind = (MainWind*)pWind;
    printf("logging out key released.\n");
}


void ui_init_style(lv_style_t* style)
{
    if (style->prop_cnt > 1)
        lv_style_reset(style);
    else
        lv_style_init(style);
}