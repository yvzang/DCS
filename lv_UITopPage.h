#ifndef __LV_UITOPPAGE_H__
#define __LV_UITOPPAGE_H__
#include <thread>
#include <memory>
#include "lvgl/lvgl.h"
#include "lv_page0.h"
#include "lv_page1.h"
#include "lv_page2.h"
#include "lv_page3.h"
#include "lv_page4.h"
#include "lv_page5.h"
#include "keyboard.h"
#include "access.h"

class UITopPage{
private:
	int _tile_index;
    lv_obj_t* _tile_wind;
    lv_obj_t* top_label;
    lv_obj_t* top_datetime;
    lv_obj_t* battery_label;
    lv_obj_t* material_label;
    lv_obj_t* battery_img;

	std::shared_ptr<UIPage0> pUIPage0_;
	std::shared_ptr<UIPage1> pUIPage1_;
	std::shared_ptr<UIPage2> pUIPage2_;
	std::shared_ptr<UIPage3> pUIPage3_;
	std::shared_ptr<UIPage4> pUIPage4_;
	std::shared_ptr<UIPage5> pUIPage5_;

	std::thread SystemClockThread_;

	PhyKeyboard* pPhyKeyboard_;
	AccessContrl* pAccessContrl_;
	LoggingWind* pLoggingWind_;
	ModifyPasswordWind* pModifyPasswordWind_;
public:
	UITopPage();
	~UITopPage();

	void setIdentity(identity_t identity);

private:
	void systemClockHandler_();

	void keyxPress_cb(int x);
	void keyxRelease_cb(int x);
	void mainwindKeyPress_cb();
	void mainwindKeyRelease_cb();
	void prewindKeyPress_cb();
	void prewindKeyRelease_cb();
	void nextwindKeyPress_cb();
	void nextwindKeyRelease_cb();
	void modifyPwdPress_cb();
	void modifyPwdRelease_cb();
	void loggingKeyPress_cb();
	void loggingKeyRelease_cb();
	void logginoutKeyPress_cb();
	void logginoutKeyRelease_cb();
};


#endif