#include <fstream>
#include <functional>
#include <sstream>
#include "lv_page0.h"
#include "lv_message_window.h"
#include "task_manager.h"
#include "setting.h"


extern pthread_mutex_t lv_lock;

LV_FONT_DECLARE(lv_font_cn_songti_medium_21);
LV_FONT_DECLARE(lv_font_cn_songti_bold);
LV_FONT_DECLARE(lv_font_cn_songti_bold_25);
LV_IMG_DECLARE(column_30x30);
LV_IMG_DECLARE(battery_4);
LV_IMG_DECLARE(loading_200x200);
LV_IMG_DECLARE(no_camera_200x200);
LV_IMG_DECLARE(down_20x20);
LV_IMG_DECLARE(off_20x20);
LV_IMG_DECLARE(green_point);
LV_IMG_DECLARE(red_point);

TaskManager gTaskManager;

UIPage0::UIPage0(lv_obj_t* pParent)
:_pParent(pParent){
    /* 摄像头BOX */
    lv_obj_t* camera_box = lv_obj_create(_pParent);
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
	lv_img_set_src(left_img_domain, &no_camera_200x200);
	lv_obj_set_size(left_img_domain, 200, 200);
	lv_obj_align(left_img_domain, LV_ALIGN_CENTER, 0, 0);
    //右图像
    _right_img_box = lv_obj_create(camera_box);
    lv_obj_set_size(_right_img_box, lv_pct(50), lv_pct(100));
    //lv_obj_align(_right_img_box, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_align_to(_right_img_box, _left_img_box, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    lv_obj_set_scrollbar_mode(_right_img_box, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_pad_all(_right_img_box, 0, LV_STATE_DEFAULT);
    lv_obj_clear_flag(_right_img_box, LV_OBJ_FLAG_SCROLLABLE);
    right_img_domain = lv_img_create(_right_img_box);
	lv_img_set_src(right_img_domain, &no_camera_200x200);
	lv_obj_set_size(right_img_domain, 200, 200);
	lv_obj_align(right_img_domain, LV_ALIGN_CENTER, 0, 0);

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

    lv_obj_t* message_box = lv_obj_create(_pParent);
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

	//摄像头
	pCameraLeft_ = std::make_shared<Camera>(left_img_domain, "camera1");
	pCameraRight_ = std::make_shared<Camera>(right_img_domain, "camera2");
	pCameraSettingWindLeft_ = std::make_shared<CameraSettingWind>(pCameraLeft_.get());
	pCameraSettingWindRight_ = std::make_shared<CameraSettingWind>(pCameraRight_.get());
	//WIFI
	pWIFISettingWind_ = std::make_shared<WIFISettingWind>(std::bind(&UIPage0::statusSwitch1_3, this, true),
														std::bind(&UIPage0::statusSwitch1_3, this, false),
														pCameraLeft_.get(), pCameraRight_.get());
	//PLC
	gTaskManager.setCallback(std::bind(&UIPage0::statusSwitch2_3, this, false));
	pLinkSettingWind_ = std::make_shared<LinkSettingWind>(std::bind(&UIPage0::statusSwitch2_3, this, true),
														std::bind(&UIPage0::statusSwitch2_3, this, false));
	//设置事件
	eventInitScreen();

	//注册数据刷新事件
	gTaskManager.registerUIFlashTask("M360", std::bind(&UIPage0::setValue0_0, this, std::placeholders::_1));
	gTaskManager.registerUIFlashTask("D140", std::bind(&UIPage0::setValue1_0, this, std::placeholders::_1));
	gTaskManager.registerUIFlashTask("D140", std::bind(&UIPage0::setValue1_1, this, std::placeholders::_1));
	gTaskManager.registerUIFlashTask("D140", std::bind(&UIPage0::setValue1_2, this, std::placeholders::_1));
	gTaskManager.registerUIFlashTask("D140", std::bind(&UIPage0::setValue2_0, this, std::placeholders::_1));
	gTaskManager.registerUIFlashTask("D140", std::bind(&UIPage0::setValue2_1, this, std::placeholders::_1));
	gTaskManager.registerUIFlashTask("D140", std::bind(&UIPage0::setValue2_2, this, std::placeholders::_1));
	gTaskManager.registerUIFlashTask("D140", std::bind(&UIPage0::setValue3_0, this, std::placeholders::_1));
	gTaskManager.registerUIFlashTask("M194", std::bind(&UIPage0::setValue3_1, this, std::placeholders::_1));
	gTaskManager.registerUIFlashTask("M194", std::bind(&UIPage0::setValue3_2, this, std::placeholders::_1));
	gTaskManager.registerUIFlashTask("D140", std::bind(&UIPage0::setValue4_0, this, std::placeholders::_1));
	gTaskManager.registerUIFlashTask("M194", std::bind(&UIPage0::setValue4_1, this, std::placeholders::_1));
	gTaskManager.registerUIFlashTask("M194", std::bind(&UIPage0::setValue4_2, this, std::placeholders::_1));
}

UIPage0::~UIPage0(){

}

void UIPage0::setValue0_0(DataPayload* payload){
	for(int i = 0; i < 24; i++){
        if(payload->dbool[i]){
            int iAddr = 360 + i;
            std::stringstream ss;
            ss << "M" << iAddr;
            auto messageIter = FAULT_MESSAGE.find(ss.str());
            if(messageIter != FAULT_MESSAGE.end()){
                auto message = messageIter->second;
                lv_textarea_set_text(running_info, message.c_str());
            }
        }
    }
}
void UIPage0::setValue1_0(DataPayload* payload){
	uint32 u32Value = payload->duint32[2];
	float floatValue = *(float*)&u32Value;
	std::stringstream ss;
	ss << "行走速度：" << floatValue;
	lv_textarea_set_text(row0_ptr_list[0], ss.str().c_str());
}
void UIPage0::setValue1_1(DataPayload* payload){
	uint32 u32Value = payload->duint32[3];
	float floatValue = *(float*)&u32Value;
	std::stringstream ss;
	ss << "顶漆速度：" << floatValue;
	lv_textarea_set_text(row0_ptr_list[1], ss.str().c_str());
}
void UIPage0::setValue1_2(DataPayload* payload){
	uint32 u32Value = payload->duint32[4];
	float floatValue = *(float*)&u32Value;
	std::stringstream ss;
	ss << "自动涂覆长度：" << floatValue;
	lv_textarea_set_text(row0_ptr_list[2], ss.str().c_str());
}
void UIPage0::setValue2_0(DataPayload* payload){
	uint32 u32Value = payload->duint32[1];
	float floatValue = *(float*)&u32Value;
	std::stringstream ss;
	ss << "涂覆厚度：" << floatValue;
	lv_textarea_set_text(row1_ptr_list[0], ss.str().c_str());
}
void UIPage0::setValue2_1(DataPayload* payload){
	uint32 u32Value = payload->duint32[0];
	float floatValue = *(float*)&u32Value;
	std::stringstream ss;
	ss << "电缆线径：" << floatValue;
	lv_textarea_set_text(row1_ptr_list[1], ss.str().c_str());
}
void UIPage0::setValue2_2(DataPayload* payload){
	uint32 u32Value = payload->duint32[5];
	float floatValue = *(float*)&u32Value;
	std::stringstream ss;
	ss << "手动涂覆厚度：" << floatValue;
	lv_textarea_set_text(row1_ptr_list[2], ss.str().c_str());
}
void UIPage0::setValue3_0(DataPayload* payload){
	
}
void UIPage0::setValue3_1(DataPayload* payload){
	bool boolValue = payload->dbool[0];
	statusSwitch1_1(boolValue);
}
void UIPage0::setValue3_2(DataPayload* payload){
	bool boolValue = payload->dbool[2];
	statusSwitch1_2(boolValue);
}
void UIPage0::setValue4_0(DataPayload* payload){
	uint32 u32Value = payload->duint32[8];
	float floatValue = *(float*)&u32Value;
	std::stringstream ss;
	ss << "当前电压：" << floatValue;
	lv_textarea_set_text(battery, ss.str().c_str());
}
void UIPage0::setValue4_1(DataPayload* payload){
	bool boolValue = payload->dbool[1];
	statusSwitch2_1(boolValue);
}
void UIPage0::setValue4_2(DataPayload* payload){
	bool boolValue = payload->dbool[3];
	statusSwitch2_2(boolValue);
}

void UIPage0::statusSwitch(lv_obj_t* pImageOBJ, bool status){
	if(pImageOBJ == NULL) return;
	if(status){
		lv_img_set_src(pImageOBJ, &green_point);
	}
	else{
		lv_img_set_src(pImageOBJ, &red_point);
	}
}
void UIPage0::statusSwitch1_1(bool status){
	statusSwitch(status_image_ptr_list[0], status);
}
void UIPage0::statusSwitch1_2(bool status){
	statusSwitch(status_image_ptr_list[1], status);
}
void UIPage0::statusSwitch1_3(bool status){
	statusSwitch(status_image_ptr_list[2], status);
}
void UIPage0::statusSwitch2_1(bool status){
	statusSwitch(row3_status_image_ptr_list[0], status);
}
void UIPage0::statusSwitch2_2(bool status){
	statusSwitch(row3_status_image_ptr_list[1], status);
}
void UIPage0::statusSwitch2_3(bool status){
	statusSwitch(row3_status_image_ptr_list[2], status);
}

void UIPage0::eventInitScreen(){
    lv_obj_add_event_cb(_left_img_box, UIPage0::imageSorceEventCallback, LV_EVENT_LONG_PRESSED, pCameraSettingWindLeft_.get());
    lv_obj_add_event_cb(_right_img_box, UIPage0::imageSorceEventCallback, LV_EVENT_LONG_PRESSED, pCameraSettingWindRight_.get());
    lv_obj_add_event_cb(_wifi_box, UIPage0::wifiSettingEventCallback, LV_EVENT_CLICKED, pWIFISettingWind_.get());
    lv_obj_add_event_cb(_link_box, UIPage0::plcSettingEventCallback, LV_EVENT_CLICKED, pLinkSettingWind_.get());
}

void UIPage0::imageSorceEventCallback(lv_event_t* e){
    lv_event_code_t code = lv_event_get_code(e);
    CameraSettingWind* pWind = (CameraSettingWind*)lv_event_get_user_data(e);

    if (code = LV_EVENT_PRESSING) {
        pWind->show();
    }
}

void UIPage0::wifiSettingEventCallback(lv_event_t* e){
    lv_event_code_t code = lv_event_get_code(e);
    WIFISettingWind* pWind = (WIFISettingWind*)lv_event_get_user_data(e);

    if (code = LV_EVENT_CLICKED) {
        pWind->show();
    }
}

void UIPage0::plcSettingEventCallback(lv_event_t* e){
    lv_event_code_t code = lv_event_get_code(e);
    LinkSettingWind* pWind = (LinkSettingWind*)lv_event_get_user_data(e);
    if (code = LV_EVENT_CLICKED) {
        pWind->show();
    }
}
