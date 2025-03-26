#ifndef __LV_PAGE4_H__
#define __LV_PAGE4_H__
#include "lvgl/lvgl.h"
#include "lv_uiparampage.h"
#include "keyboard.h"


class UIPage4: public UIParamPageABC{
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
	UIPage4(lv_obj_t* parent);

    void getValue0_0(DataPayload* payload);
    void getValue0_1(DataPayload* payload);
    void getValue1_0(DataPayload* payload);
    void getValue1_1(DataPayload* payload);
    void getValue1_2(DataPayload* payload);
    void getValue2_0(DataPayload* payload);
    void getValue2_1(DataPayload* payload);
    void getValue3_0(DataPayload* payload);
    void getValue3_1(DataPayload* payload);
    void getValue4(DataPayload* payload);
    void getValue5(DataPayload* payload);
    void getValue6(DataPayload* payload);

	void setValue0_0();
	void setValue0_1();
	void setValue1_0();
	void setValue1_1();
	void setValue1_2();
	void setValue2_0();
	void setValue2_1();
	void setValue3_0();
	void setValue3_1();
	void setValue4();
	void setValue5();
	void setValue6();

    void set_disable(bool disable);
	void event_init_screen();
private:
	virtual void setValue(void* target);
};

#endif