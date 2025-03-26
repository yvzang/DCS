#include <sstream>
#include "utils.h"
#include "task_manager.h"
#include "lv_page1.h"
#include "lv_page2.h"
#include "limlog.h"

LV_FONT_DECLARE(lv_font_cn_songti_bold_25);
LV_FONT_DECLARE(lv_font_cn_songti_medium_21);
LV_IMG_DECLARE(down_20x20);

extern WorkRecordCtx gWorkRecordCtx;
extern TaskManager gTaskManager;

void ui_init_style(lv_style_t* style)
{
    if (style->prop_cnt > 1)
        lv_style_reset(style);
    else
        lv_style_init(style);
}

UIPage1::UIPage1(lv_obj_t* pParent)
:pParamKeyboard_(ParamKeyboard::getInstance()){
    ui = new lv_ui;
    ui->screen_1_background_tile_3 = pParent;
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

	gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage1::getValue0, this, std::placeholders::_1));
	gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage1::getValue1, this, std::placeholders::_1));
	gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage1::getValue2_0, this, std::placeholders::_1));
	gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage1::getValue2_1, this, std::placeholders::_1));
	gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage1::getValue3_0, this, std::placeholders::_1));
	gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage1::getValue3_1, this, std::placeholders::_1));
	gTaskManager.registerDataArriveOnce("D140", std::bind(&UIPage1::getValue4, this, std::placeholders::_1));
	gTaskManager.registerDataArriveOnce("D140", std::bind(&UIPage1::getValue5, this, std::placeholders::_1));
}

UIPage1::~UIPage1(){

}


void UIPage1::event_init_screen(){
    lv_obj_add_event_cb(ui->screen_1_ddlist_1, dropdown_event_cb, LV_EVENT_VALUE_CHANGED, this);
    lv_obj_add_event_cb(ui->screen_1_btn_1, btn_model_std_event_cb, LV_EVENT_CLICKED, this);
    lv_obj_add_event_cb(ui->screen_1_btn_2, btn_model_menu_event_cb, LV_EVENT_CLICKED, this);

    lv_obj_add_event_cb(ui->screen_1_ta_27, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, (void*)this);
    lv_obj_add_event_cb(ui->screen_1_ta_28, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, (void*)this);
    lv_obj_add_event_cb(ui->screen_1_ta_29, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, (void*)this);
    lv_obj_add_event_cb(ui->screen_1_ta_30, ParamKeyboard::keyboard_event_cb, LV_EVENT_ALL, (void*)this);
}

void UIPage1::getValue0(DataPayload* payload){
	uint32 uintValue = payload->duint32[40];
	ushort value = uintValue | 0xffff;
	lv_dropdown_set_selected(ui->screen_1_ddlist_1, value - 1);
}
void UIPage1::getValue1(DataPayload* payload){
	uint32 uintValue = payload->duint32[40];
	ushort value = uintValue >> 16;
	switch_model((model_t)value);
}
void UIPage1::getValue2_0(DataPayload* payload){
	uint32 uintValue = payload->duint32[41];
	float floatValue = *(float*)&uintValue;
	std::stringstream ss;
	ss << floatValue;
	lv_textarea_set_text(ui->screen_1_ta_27, ss.str().c_str());
}
void UIPage1::getValue2_1(DataPayload* payload){
	uint32 uintValue = payload->duint32[43];
	float floatValue = *(float*)&uintValue;
	std::stringstream ss;
	ss << floatValue;
	lv_textarea_set_text(ui->screen_1_ta_29, ss.str().c_str());
}
void UIPage1::getValue3_0(DataPayload* payload){
	uint32 uintValue = payload->duint32[42];
	float floatValue = *(float*)&uintValue;
	std::stringstream ss;
	ss << floatValue;
	lv_textarea_set_text(ui->screen_1_ta_28, ss.str().c_str());
}
void UIPage1::getValue3_1(DataPayload* payload){
	uint32 uintValue = payload->duint32[44];
	float floatValue = *(float*)&uintValue;
	std::stringstream ss;
	ss << floatValue;
	lv_textarea_set_text(ui->screen_1_ta_30, ss.str().c_str());
}
void UIPage1::getValue4(DataPayload* payload){
	uint32 uintValue = payload->duint32[6];
	float floatValue = *(float*)&uintValue;
	std::stringstream ss;
	ss << "顶漆工作速度：" << floatValue;
	lv_textarea_set_text(ui->screen_1_ta_31, ss.str().c_str());
}
void UIPage1::getValue5(DataPayload* payload){
	uint32 uintValue = payload->duint32[7];
	float floatValue = *(float*)&uintValue;
	std::stringstream ss;
	ss << "行走工作速度" << floatValue;
	lv_textarea_set_text(ui->screen_1_ta_32, ss.str().c_str());
}

void UIPage1::setValue0(){
	uint16_t index = lv_dropdown_get_selected(ui->screen_1_ddlist_1)+1;
	uint16_t model = (uint16_t)CurrentModle_;
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_UINT32;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1080";
	task->payload = std::make_shared<DataPayload>();
	task->payload->duint32.push_back((uint32_t)model << 16 | index);
	task->PLCTaskSuccessCallback = std::bind([](){});
	task->PLCTaskFailedCallback = std::bind([](){});
	gTaskManager.registerTempTask(task);
}
void UIPage1::setValue2_0(){
	const char* pStrValue = lv_textarea_get_text(ui->screen_1_ta_27);
	float floatValue = std::stof(pStrValue);
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_FLOAT;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1082";
	task->payload = std::make_shared<DataPayload>();
	task->payload->dfloat.push_back(floatValue);
	task->PLCTaskSuccessCallback = std::bind([=](){LOG_DEBUG << "write D1082 " << pStrValue << " success!";});
	task->PLCTaskFailedCallback = std::bind([=](){LOG_DEBUG << "write D1082 " << pStrValue << " failed!";});
	gTaskManager.registerTempTask(task);
}
void UIPage1::setValue2_1(){
	const char* pStrValue = lv_textarea_get_text(ui->screen_1_ta_29);
	std::stringstream ss(pStrValue);
	float floatValue;
	ss >> floatValue;
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_FLOAT;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1086";
	task->payload = std::make_shared<DataPayload>();
	task->payload->dfloat.push_back(floatValue);
	task->PLCTaskSuccessCallback = std::bind([](){});
	task->PLCTaskFailedCallback = std::bind([](){});
	gTaskManager.registerTempTask(task);
}
void UIPage1::setValue3_0(){
	const char* pStrValue = lv_textarea_get_text(ui->screen_1_ta_28);
	std::stringstream ss(pStrValue);
	float floatValue;
	ss >> floatValue;
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_FLOAT;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1084";
	task->payload = std::make_shared<DataPayload>();
	task->payload->dfloat.push_back(floatValue);
	task->PLCTaskSuccessCallback = std::bind([](){});
	task->PLCTaskFailedCallback = std::bind([](){});
	gTaskManager.registerTempTask(task);
}
void UIPage1::setValue3_1(){
	const char* pStrValue = lv_textarea_get_text(ui->screen_1_ta_30);
	std::stringstream ss(pStrValue);
	float floatValue;
	ss >> floatValue;
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_FLOAT;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1088";
	task->payload = std::make_shared<DataPayload>();
	task->payload->dfloat.push_back(floatValue);
	task->PLCTaskSuccessCallback = std::bind([](){});
	task->PLCTaskFailedCallback = std::bind([](){});
	gTaskManager.registerTempTask(task);
}

void UIPage1::setValue(void* target){
	if(target == ui->screen_1_ta_27){
		setValue2_0();
        workRecord();
	}
	else if(target == ui->screen_1_ta_29){
		setValue2_1();
        workRecord();
	}
	else if(target == ui->screen_1_ta_28){
		setValue3_0();
        workRecord();
	}
	else if(target == ui->screen_1_ta_30){
		setValue3_1();
        workRecord();
	}
}

void UIPage1::dropdown_event_cb(lv_event_t* e){
    lv_event_code_t code = lv_event_get_code(e);
    UIPage1* pWind = (UIPage1*)lv_event_get_user_data(e);
    lv_obj_t* dropdown = lv_event_get_target(e);
    lv_obj_t* pDropdownList = lv_dropdown_get_list(dropdown);
    if(pDropdownList == NULL){
        return;
    }
    lv_obj_set_style_text_font(pDropdownList, &lv_font_cn_songti_medium_21, 0);

    if(code == LV_EVENT_VALUE_CHANGED){
        pWind->setValue0();
    }
}

void UIPage1::btn_model_std_event_cb(lv_event_t* e){
    UIPage1* pWind = (UIPage1*)lv_event_get_user_data(e);
    lv_obj_t* btn = lv_event_get_target(e);
    pWind->switch_model(MODEL_STD);
    pWind->setValue0();
}

void UIPage1::btn_model_menu_event_cb(lv_event_t* e){
    UIPage1* pWind = (UIPage1*)lv_event_get_user_data(e);
    lv_obj_t* btn = lv_event_get_target(e);
    pWind->switch_model(MODEL_MANU);
    pWind->setValue0();
}

void UIPage1::switch_model(model_t model){
    if(model == MODEL_STD){
		CurrentModle_ = MODEL_STD;
        lv_obj_set_style_bg_color(ui->screen_1_btn_1, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui->screen_1_btn_2, lv_color_hex(0xbbbbbb), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else if(model == MODEL_MANU){
		CurrentModle_ = MODEL_MANU;
        lv_obj_set_style_bg_color(ui->screen_1_btn_2, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui->screen_1_btn_1, lv_color_hex(0xbbbbbb), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

void UIPage1::workRecord(){
    gWorkRecordCtx.pUIPage2->insert_record(toString(static_cast<uint32_t>(time(NULL))),
                                        lv_textarea_get_text(ui->screen_1_ta_27),
                                        lv_textarea_get_text(ui->screen_1_ta_28),
                                        lv_textarea_get_text(ui->screen_1_ta_30),
                                        lv_textarea_get_text(ui->screen_1_ta_29),
                                        lv_textarea_get_text(ui->screen_1_ta_27));
}
