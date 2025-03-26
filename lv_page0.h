#ifndef __LV_PAGE0_H__
#define __LV_PAGE0_H__

#include <thread>
#include <memory>
#include "lvgl/lvgl.h"
#include "access.h"
#include "camera_helper.h"
#include "lv_message_window.h"

class UIPage0{
private:
    int8_t _tile_index = 0;
    lv_obj_t* _pParent;
    lv_obj_t* _left_img_box;
    lv_obj_t* _right_img_box;
    lv_obj_t* left_img_domain;
    lv_obj_t* right_img_domain;
    lv_obj_t* _wifi_box;
    lv_obj_t* _link_box;
    lv_obj_t* current_angle;
    lv_obj_t* battery;
    lv_obj_t* running_info;
    lv_obj_t* status_image_ptr_list[3];
    lv_obj_t* row3_status_image_ptr_list[3];
    lv_obj_t* row0_ptr_list[3];
    lv_obj_t* row1_ptr_list[3];

	std::shared_ptr<Camera> pCameraLeft_;
	std::shared_ptr<Camera> pCameraRight_;
	std::shared_ptr<CameraSettingWind> pCameraSettingWindLeft_;
	std::shared_ptr<CameraSettingWind> pCameraSettingWindRight_;

	std::shared_ptr<WIFISettingWind> pWIFISettingWind_;
	std::shared_ptr<LinkSettingWind> pLinkSettingWind_;

public:
	UIPage0(lv_obj_t* pParent);
	~UIPage0();

	void setValue0_0(DataPayload* payload);
	void setValue1_0(DataPayload* payload);
	void setValue1_1(DataPayload* payload);
	void setValue1_2(DataPayload* payload);
	void setValue2_0(DataPayload* payload);
	void setValue2_1(DataPayload* payload);
	void setValue2_2(DataPayload* payload);
	void setValue3_0(DataPayload* payload);
	void setValue3_1(DataPayload* payload);
	void setValue3_2(DataPayload* payload);
	void setValue4_0(DataPayload* payload);
	void setValue4_1(DataPayload* payload);
	void setValue4_2(DataPayload* payload);

	void statusSwitch1_1(bool status);
	void statusSwitch1_2(bool status);
	void statusSwitch1_3(bool status);
	void statusSwitch2_1(bool status);
	void statusSwitch2_2(bool status);
	void statusSwitch2_3(bool status);

	void cameraLeftImage(CameraState state);
	void cameraRightImage(CameraState state);
private:
	void statusSwitch(lv_obj_t* pImageOBJ, bool status);
	void eventInitScreen();

	static void imageSorceEventCallback(lv_event_t* e);
	static void wifiSettingEventCallback(lv_event_t* e);
	static void plcSettingEventCallback(lv_event_t* e);
};

#endif