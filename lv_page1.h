#ifndef __LV_PAGE1_H__
#define __LV_PAGE1_H__
#include "lvgl/lvgl.h"
#include "keyboard.h"
#include "lv_uiparampage.h"

class UIPage1 : public UIParamPageABC{
public:
public:
    typedef enum{
        MODEL_STD = 1,
        MODEL_MANU
    } model_t;
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
	ParamKeyboard* pParamKeyboard_;
	model_t CurrentModle_;
public:
	UIPage1(lv_obj_t* pParent);
	~UIPage1();

	void event_init_screen();

	void getValue0(DataPayload* payload);
	void getValue1(DataPayload* payload);
	void getValue2_0(DataPayload* payload);
	void getValue2_1(DataPayload* payload);
	void getValue3_0(DataPayload* payload);
	void getValue3_1(DataPayload* payload);
	void getValue4(DataPayload* payload);
	void getValue5(DataPayload* payload);

	void setValue0();
	void setValue2_0();
	void setValue2_1();
	void setValue3_0();
	void setValue3_1();
	virtual void setValue(void* target);

private:
    static void btn_model_std_event_cb(lv_event_t* e);
    static void btn_model_menu_event_cb(lv_event_t* e);
	static void dropdown_event_cb(lv_event_t* e);
    void switch_model(model_t model);
    void workRecord();
};


#endif