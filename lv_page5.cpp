#include <sstream>
#include <functional>
#include "limlog.h"
#include "lv_page5.h"
#include "utils.h"

extern TaskManager gTaskManager;

LV_FONT_DECLARE(lv_font_cn_songti_bold_25);
LV_FONT_DECLARE(lv_font_cn_songti_medium_21);

UIPage5::UIPage5(lv_obj_t* parent){
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

    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue0_0, this, std::placeholders::_1));
    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue0_1, this, std::placeholders::_1));
    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue1_0, this, std::placeholders::_1));
    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue1_1, this, std::placeholders::_1));
    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue2_0, this, std::placeholders::_1));
    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue2_1, this, std::placeholders::_1));
    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue2_2, this, std::placeholders::_1));
    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue3_0, this, std::placeholders::_1));
    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue3_1, this, std::placeholders::_1));
    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue4_0, this, std::placeholders::_1));
    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue4_1, this, std::placeholders::_1));
    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue5_0, this, std::placeholders::_1));
    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue5_1, this, std::placeholders::_1));
    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue6_0, this, std::placeholders::_1));
    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue6_1, this, std::placeholders::_1));
    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue7, this, std::placeholders::_1));
}

void UIPage5::getValue0_0(DataPayload* payload){
	uint32 uintValue = payload->duint32[20];
	float floatValue = *(float*)&uintValue;
	std::stringstream ss;
	ss << floatValue;
	lv_textarea_set_text(ui->screen_1_ta_15, ss.str().c_str());
}
void UIPage5::getValue0_1(DataPayload* payload){
	uint32 uintValue = payload->duint32[19];
	float floatValue = *(float*)&uintValue;
	std::stringstream ss;
	ss << floatValue;
	lv_textarea_set_text(ui->screen_1_ta_16, ss.str().c_str());
}
void UIPage5::getValue1_0(DataPayload* payload){
	uint32 uintValue = payload->duint32[21];
	float floatValue = *(float*)&uintValue;
	std::stringstream ss;
	ss << floatValue;
	lv_textarea_set_text(ui->screen_1_ta_13, ss.str().c_str());
}
void UIPage5::getValue1_1(DataPayload* payload){
	uint32 uintValue = payload->duint32[6];
	std::stringstream ss;
	ss << uintValue;
	lv_textarea_set_text(ui->screen_1_ta_17, ss.str().c_str());
}
void UIPage5::getValue2_0(DataPayload* payload){
	uint32 uintValue = payload->duint32[22];
	float floatValue = *(float*)&uintValue;
	std::stringstream ss;
	ss << floatValue;
	lv_textarea_set_text(ui->screen_1_ta_18, ss.str().c_str());
}
void UIPage5::getValue2_1(DataPayload* payload){
	uint32 uintValue = payload->duint32[7];
	std::stringstream ss;
	ss << uintValue;
	lv_textarea_set_text(ui->screen_1_ta_12, ss.str().c_str());
}
void UIPage5::getValue2_2(DataPayload* payload){
	uint32 uintValue = payload->duint32[8];
	std::stringstream ss;
	ss << uintValue;
	lv_textarea_set_text(ui->screen_1_ta_11, ss.str().c_str());
}
void UIPage5::getValue3_0(DataPayload* payload){
	uint32 uintValue = payload->duint32[23];
	float floatValue = *(float*)&uintValue;
	std::stringstream ss;
	ss << floatValue;
	lv_textarea_set_text(ui->screen_1_ta_21, ss.str().c_str());
}
void UIPage5::getValue3_1(DataPayload* payload){
	uint32 uintValue = payload->duint32[9];
	float floatValue = *(float*)&uintValue;
	std::stringstream ss;
	ss << floatValue;
	lv_textarea_set_text(ui->screen_1_ta_23, ss.str().c_str());
}
void UIPage5::getValue4_0(DataPayload* payload){
	uint32 uintValue = payload->duint32[24];
	float floatValue = *(float*)&uintValue;
	std::stringstream ss;
	ss << floatValue;
	lv_textarea_set_text(ui->screen_1_ta_22, ss.str().c_str());
}
void UIPage5::getValue4_1(DataPayload* payload){
	uint32 uintValue = payload->duint32[10];
	float floatValue = *(float*)&uintValue;
	std::stringstream ss;
	ss << floatValue;
	lv_textarea_set_text(ui->screen_1_ta_24, ss.str().c_str());
}
void UIPage5::getValue5_0(DataPayload* payload){

}
void UIPage5::getValue5_1(DataPayload* payload){
	uint32 uintValue = payload->duint32[26];
	ushort int16Value = uintValue >> 16;
	std::stringstream ss;
	ss << int16Value;
	lv_textarea_set_text(ui->screen_1_ta_5, ss.str().c_str());
}
void UIPage5::getValue6_0(DataPayload* payload){
	
}
void UIPage5::getValue6_1(DataPayload* payload){
	uint32 uintValue = payload->duint32[27];
	ushort int16Value = uintValue & 0xffff;
	std::stringstream ss;
	ss << int16Value;
	lv_textarea_set_text(ui->screen_1_ta_4, ss.str().c_str());
}
void UIPage5::getValue7(DataPayload* payload){
	
}

void UIPage5::setValue0_0(){
	auto pStrValue = lv_textarea_get_text(ui->screen_1_ta_15);
	float floatValue = str2float(pStrValue);
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_FLOAT;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1040";
	task->payload = std::make_shared<DataPayload>();
	task->payload->dfloat.push_back(floatValue);
	task->PLCTaskSuccessCallback = std::bind([&](){gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue0_0, this, std::placeholders::_1));});
	task->PLCTaskFailedCallback = std::bind([=](){});
	gTaskManager.registerTempTask(task);
}
void UIPage5::setValue0_1(){
	auto pStrValue = lv_textarea_get_text(ui->screen_1_ta_16);
	float floatValue = str2float(pStrValue);
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_FLOAT;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1038";
	task->payload = std::make_shared<DataPayload>();
	task->payload->dfloat.push_back(floatValue);
	task->PLCTaskSuccessCallback = std::bind([&](){gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue0_1, this, std::placeholders::_1));});
	task->PLCTaskFailedCallback = std::bind([=](){});
	gTaskManager.registerTempTask(task);
}
void UIPage5::setValue1_0(){
	auto pStrValue = lv_textarea_get_text(ui->screen_1_ta_13);
	float floatValue = str2float(pStrValue);
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_FLOAT;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1042";
	task->payload = std::make_shared<DataPayload>();
	task->payload->dfloat.push_back(floatValue);
	task->PLCTaskSuccessCallback = std::bind([=](){gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue1_0, this, std::placeholders::_1));});
	task->PLCTaskFailedCallback = std::bind([=](){});
	gTaskManager.registerTempTask(task);
}
void UIPage5::setValue1_1(){
	auto pStrValue = lv_textarea_get_text(ui->screen_1_ta_17);
	float floatValue = str2float(pStrValue);
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_UINT32;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1012";
	task->payload = std::make_shared<DataPayload>();
	task->payload->duint32.push_back(floatValue);
	task->PLCTaskSuccessCallback = std::bind([=](){gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue1_1, this, std::placeholders::_1));});
	task->PLCTaskFailedCallback = std::bind([=](){});
	gTaskManager.registerTempTask(task);
}
void UIPage5::setValue2_0(){
	auto pStrValue = lv_textarea_get_text(ui->screen_1_ta_18);
	float floatValue = str2float(pStrValue);
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_FLOAT;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1044";
	task->payload = std::make_shared<DataPayload>();
	task->payload->dfloat.push_back(floatValue);
	task->PLCTaskSuccessCallback = std::bind([=](){gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue2_0, this, std::placeholders::_1));});
	task->PLCTaskFailedCallback = std::bind([=](){});
	gTaskManager.registerTempTask(task);
}
void UIPage5::setValue2_1(){
	auto pStrValue = lv_textarea_get_text(ui->screen_1_ta_12);
	uint32_t floatValue = str2uint32(pStrValue);
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_UINT32;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1014";
	task->payload = std::make_shared<DataPayload>();
	task->payload->duint32.push_back(floatValue);
    LOG_DEBUG << "write D1014 " << floatValue;
	task->PLCTaskSuccessCallback = std::bind([=](){gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue2_1, this, std::placeholders::_1));});
	task->PLCTaskFailedCallback = std::bind([=](){});
	gTaskManager.registerTempTask(task);
}
void UIPage5::setValue2_2(){
	auto pStrValue = lv_textarea_get_text(ui->screen_1_ta_11);
	uint32_t floatValue = str2uint32(pStrValue);
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_UINT32;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1016";
	task->payload = std::make_shared<DataPayload>();
	task->payload->duint32.push_back(floatValue);
	task->PLCTaskSuccessCallback = std::bind([=](){gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue2_2, this, std::placeholders::_1));});
	task->PLCTaskFailedCallback = std::bind([=](){});
	gTaskManager.registerTempTask(task);
}
void UIPage5::setValue3_0(){
	auto pStrValue = lv_textarea_get_text(ui->screen_1_ta_21);
	float floatValue = str2float(pStrValue);
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_FLOAT;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1046";
	task->payload = std::make_shared<DataPayload>();
	task->payload->dfloat.push_back(floatValue);
	task->PLCTaskSuccessCallback = std::bind([=](){gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue3_0, this, std::placeholders::_1));});
	task->PLCTaskFailedCallback = std::bind([=](){});
	gTaskManager.registerTempTask(task);
}
void UIPage5::setValue3_1(){
	auto pStrValue = lv_textarea_get_text(ui->screen_1_ta_23);
	float floatValue = str2float(pStrValue);
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_FLOAT;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1018";
	task->payload = std::make_shared<DataPayload>();
	task->payload->dfloat.push_back(floatValue);
	task->PLCTaskSuccessCallback = std::bind([=](){gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue3_1, this, std::placeholders::_1));});
	task->PLCTaskFailedCallback = std::bind([=](){});
	gTaskManager.registerTempTask(task);
}
void UIPage5::setValue4_0(){
	auto pStrValue = lv_textarea_get_text(ui->screen_1_ta_22);
	float floatValue = str2float(pStrValue);
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_FLOAT;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1048";
	task->payload = std::make_shared<DataPayload>();
	task->payload->dfloat.push_back(floatValue);
	task->PLCTaskSuccessCallback = std::bind([=](){gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue4_0, this, std::placeholders::_1));});
	task->PLCTaskFailedCallback = std::bind([=](){});
	gTaskManager.registerTempTask(task);
}
void UIPage5::setValue4_1(){
	auto pStrValue = lv_textarea_get_text(ui->screen_1_ta_24);
	float floatValue = str2float(pStrValue);
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_FLOAT;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1020";
	task->payload = std::make_shared<DataPayload>();
	task->payload->dfloat.push_back(floatValue);
	task->PLCTaskSuccessCallback = std::bind([=](){gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue4_1, this, std::placeholders::_1));});
	task->PLCTaskFailedCallback = std::bind([=](){});
	gTaskManager.registerTempTask(task);
}
void UIPage5::setValue5_0(){
	
}
void UIPage5::setValue5_1(){
	auto pStrValueLow = lv_textarea_get_text(ui->screen_1_ta_5);
    auto pStrValueHeight = lv_textarea_get_text(ui->screen_1_ta_4);
	ushort ValueLow = str2uint32(pStrValueLow);
    ushort ValueHeight = str2uint32(pStrValueHeight);
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_UINT32;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1053";
	task->payload = std::make_shared<DataPayload>();
	task->payload->duint32.push_back((uint32)ValueHeight << 16 | ValueLow);
	task->PLCTaskSuccessCallback = std::bind([=](){gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue5_1, this, std::placeholders::_1));});
	task->PLCTaskFailedCallback = std::bind([=](){});
	gTaskManager.registerTempTask(task);
}
void UIPage5::setValue6_0(){
	
}
void UIPage5::setValue6_1(){
	auto pStrValueLow = lv_textarea_get_text(ui->screen_1_ta_5);
    auto pStrValueHeight = lv_textarea_get_text(ui->screen_1_ta_4);
	ushort ValueLow = str2uint32(pStrValueLow);
    ushort ValueHeight = str2uint32(pStrValueHeight);
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_UINT32;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1053";
	task->payload = std::make_shared<DataPayload>();
	task->payload->duint32.push_back((uint32)ValueHeight << 16 | ValueLow);
	task->PLCTaskSuccessCallback = std::bind([=](){gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage5::getValue6_1, this, std::placeholders::_1));});
	task->PLCTaskFailedCallback = std::bind([=](){});
	gTaskManager.registerTempTask(task);
}
void UIPage5::setValue7(){
	
}

void UIPage5::set_disable(bool disable){
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

void UIPage5::events_init_screen(){
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

void UIPage5::setValue(void* target){
    if(target == ui->screen_1_ta_15){
        setValue0_0();
    }
    else if(target == ui->screen_1_ta_16){
        setValue0_1();
    }
    else if(target == ui->screen_1_ta_13){
        setValue1_0();
    }
    else if(target == ui->screen_1_ta_17){
        setValue1_1();
    }
    else if(target == ui->screen_1_ta_18){
        setValue2_0();
    }
    else if(target == ui->screen_1_ta_11){
        setValue2_1();
    }
    else if(target == ui->screen_1_ta_12){
        setValue2_2();
    }
    else if(target == ui->screen_1_ta_21){
        setValue3_0();
    }
    else if(target == ui->screen_1_ta_23){
        setValue3_1();
    }
    else if(target == ui->screen_1_ta_22){
        setValue4_0();
    }
    else if(target == ui->screen_1_ta_24){
        setValue4_1();
    }
    else if(target == ui->screen_1_ta_25){
        setValue5_0();
    }
    else if(target == ui->screen_1_ta_5){
        setValue5_1();
    }
    else if(target == ui->screen_1_ta_26){
        setValue6_0();
    }
    else if(target == ui->screen_1_ta_4){
        setValue6_1();
    }
    else if(target == ui->screen_1_ta_1){
        setValue7();
    }
}