#ifndef __LV_PAGE5_H__
#define __LV_PAGE5_H__
#include "lvgl/lvgl.h"
#include "keyboard.h"
#include "task_manager.h"
#include "lv_uiparampage.h"

class UIPage5: public UIParamPageABC{
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
	UIPage5(lv_obj_t* parent);

	void getValue0_0(DataPayload* payload);
	void getValue0_1(DataPayload* payload);
	void getValue1_0(DataPayload* payload);
	void getValue1_1(DataPayload* payload);
	void getValue2_0(DataPayload* payload);
	void getValue2_1(DataPayload* payload);
	void getValue2_2(DataPayload* payload);
	void getValue3_0(DataPayload* payload);
	void getValue3_1(DataPayload* payload);
	void getValue4_0(DataPayload* payload);
	void getValue4_1(DataPayload* payload);
	void getValue5_0(DataPayload* payload);
	void getValue5_1(DataPayload* payload);
	void getValue6_0(DataPayload* payload);
	void getValue6_1(DataPayload* payload);
	void getValue7(DataPayload* payload);

	void setValue0_0();
	void setValue0_1();
	void setValue1_0();
	void setValue1_1();
	void setValue2_0();
	void setValue2_1();
	void setValue2_2();
	void setValue3_0();
	void setValue3_1();
	void setValue4_0();
	void setValue4_1();
	void setValue5_0();
	void setValue5_1();
	void setValue6_0();
	void setValue6_1();
	void setValue7();

    void set_disable(bool disable);
	void events_init_screen();
private:
    virtual void setValue(void* target);
};


#endif