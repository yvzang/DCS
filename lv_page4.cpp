#include "task_manager.h"
#include "lv_page4.h"
#include "utils.h"

extern TaskManager gTaskManager;

LV_FONT_DECLARE(lv_font_cn_songti_bold_25);
LV_FONT_DECLARE(lv_font_cn_songti_medium_21);

UIPage4::UIPage4(lv_obj_t* parent)
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

    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage4::getValue0_0, this, std::placeholders::_1));
    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage4::getValue0_1, this, std::placeholders::_1));
    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage4::getValue1_0, this, std::placeholders::_1));
    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage4::getValue1_1, this, std::placeholders::_1));
    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage4::getValue1_2, this, std::placeholders::_1));
    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage4::getValue2_0, this, std::placeholders::_1));
    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage4::getValue2_1, this, std::placeholders::_1));
    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage4::getValue3_0, this, std::placeholders::_1));
    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage4::getValue3_1, this, std::placeholders::_1));
    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage4::getValue4, this, std::placeholders::_1));
    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage4::getValue5, this, std::placeholders::_1));
    gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage4::getValue6, this, std::placeholders::_1));
}

void UIPage4::getValue0_0(DataPayload* payload){
    uint32 uintValue = payload->duint32[35];
    float floatValue = *(float*)&uintValue;
	std::stringstream ss;
	ss << floatValue;
	lv_textarea_set_text(ui->screen_fast_mv_spd, ss.str().c_str());
}
void UIPage4::getValue0_1(DataPayload* payload){
    uint32 uintValue = payload->duint32[0];
	std::stringstream ss;
	ss << uintValue;
	lv_textarea_set_text(ui->screen_single_mmt, ss.str().c_str());
}
void UIPage4::getValue1_0(DataPayload* payload){
    uint32 uintValue = payload->duint32[36];
    float floatValue = *(float*)&uintValue;
	std::stringstream ss;
	ss << floatValue;
	lv_textarea_set_text(ui->screen_slow_mv_psd, ss.str().c_str());
}
void UIPage4::getValue1_1(DataPayload* payload){
    uint32 uintValue = payload->duint32[1];
	std::stringstream ss;
	ss << uintValue;
	lv_textarea_set_text(ui->screen_mv_alt_spd_1, ss.str().c_str());
}
void UIPage4::getValue1_2(DataPayload* payload){
    uint32 uintValue = payload->duint32[2];
	std::stringstream ss;
	ss << uintValue;
	lv_textarea_set_text(ui->screen_mv_alt_spd_2, ss.str().c_str());
}
void UIPage4::getValue2_0(DataPayload* payload){
    uint32 uintValue = payload->duint32[37];
    float floatValue = *(float*)&uintValue;
	std::stringstream ss;
	ss << floatValue;
	lv_textarea_set_text(ui->screen_fast_mv_spd_scl, ss.str().c_str());
}
void UIPage4::getValue2_1(DataPayload* payload){
    uint32 uintValue = payload->duint32[3];
    float floatValue = *(float*)&uintValue;
	std::stringstream ss;
	ss << floatValue;
	lv_textarea_set_text(ui->screen_mv_tl_in_len, ss.str().c_str());
}
void UIPage4::getValue3_0(DataPayload* payload){
    uint32 uintValue = payload->duint32[38];
    float floatValue = *(float*)&uintValue;
	std::stringstream ss;
	ss << floatValue;
	lv_textarea_set_text(ui->screen_mv_len_scl, ss.str().c_str());
}
void UIPage4::getValue3_1(DataPayload* payload){
    uint32 uintValue = payload->duint32[4];
    float floatValue = *(float*)&uintValue;
	std::stringstream ss;
	ss << floatValue;
	lv_textarea_set_text(ui->screen_mv_tl_ang, ss.str().c_str());
}
void UIPage4::getValue4(DataPayload* payload){
    uint32 uintValue = payload->duint32[31] / 1000;
	std::stringstream ss;
	ss << uintValue;
	lv_textarea_set_text(ui->screen_param1_fast_up_spd, ss.str().c_str());
}
void UIPage4::getValue5(DataPayload* payload){
    uint32 uintValue = payload->duint32[32] / 1000;
	std::stringstream ss;
	ss << uintValue;
	lv_textarea_set_text(ui->screen_param1_fast_down_spd, ss.str().c_str());
}
void UIPage4::getValue6(DataPayload* payload){
    uint32 uintValue = payload->duint32[30] / 1000;
	std::stringstream ss;
	ss << uintValue;
	lv_textarea_set_text(ui->screen_param1_slow_ud_spd, ss.str().c_str());
}

void UIPage4::setValue0_0(){
	auto pStrValue = lv_textarea_get_text(ui->screen_fast_mv_spd);
	float floatValue = str2float(pStrValue);
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_FLOAT;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1070";
	task->payload = std::make_shared<DataPayload>();
	task->payload->dfloat.push_back(floatValue);
	task->PLCTaskSuccessCallback = std::bind([=](){gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage4::getValue0_0, this, std::placeholders::_1));});
	task->PLCTaskFailedCallback = std::bind([=](){});
	gTaskManager.registerTempTask(task);
}
void UIPage4::setValue0_1(){
	auto pStrValue = lv_textarea_get_text(ui->screen_single_mmt);
	uint32_t floatValue = str2uint32(pStrValue);
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_UINT32;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1000";
	task->payload = std::make_shared<DataPayload>();
	task->payload->duint32.push_back(floatValue);
	task->PLCTaskSuccessCallback = std::bind([=](){gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage4::getValue0_1, this, std::placeholders::_1));});
	task->PLCTaskFailedCallback = std::bind([=](){});
	gTaskManager.registerTempTask(task);
}
void UIPage4::setValue1_0(){
	auto pStrValue = lv_textarea_get_text(ui->screen_slow_mv_psd);
	float floatValue = str2float(pStrValue);
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_FLOAT;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1072";
	task->payload = std::make_shared<DataPayload>();
	task->payload->dfloat.push_back(floatValue);
	task->PLCTaskSuccessCallback = std::bind([=](){gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage4::getValue1_0, this, std::placeholders::_1));});
	task->PLCTaskFailedCallback = std::bind([=](){});
	gTaskManager.registerTempTask(task);
}
void UIPage4::setValue1_1(){
	auto pStrValue = lv_textarea_get_text(ui->screen_mv_alt_spd_1);
	uint32_t floatValue = str2uint32(pStrValue);
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_UINT32;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1002";
	task->payload = std::make_shared<DataPayload>();
	task->payload->duint32.push_back(floatValue);
	task->PLCTaskSuccessCallback = std::bind([=](){gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage4::getValue1_1, this, std::placeholders::_1));});
	task->PLCTaskFailedCallback = std::bind([=](){});
	gTaskManager.registerTempTask(task);
}
void UIPage4::setValue1_2(){
	auto pStrValue = lv_textarea_get_text(ui->screen_mv_alt_spd_2);
	uint32_t floatValue = str2uint32(pStrValue);
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_UINT32;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1004";
	task->payload = std::make_shared<DataPayload>();
	task->payload->duint32.push_back(floatValue);
	task->PLCTaskSuccessCallback = std::bind([=](){gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage4::getValue1_2, this, std::placeholders::_1));});
	task->PLCTaskFailedCallback = std::bind([=](){});
	gTaskManager.registerTempTask(task);
}
void UIPage4::setValue2_0(){
	auto pStrValue = lv_textarea_get_text(ui->screen_fast_mv_spd_scl);
	float floatValue = str2float(pStrValue);
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_FLOAT;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1074";
	task->payload = std::make_shared<DataPayload>();
	task->payload->dfloat.push_back(floatValue);
	task->PLCTaskSuccessCallback = std::bind([=](){gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage4::getValue2_0, this, std::placeholders::_1));});
	task->PLCTaskFailedCallback = std::bind([=](){});
	gTaskManager.registerTempTask(task);
}
void UIPage4::setValue2_1(){
	auto pStrValue = lv_textarea_get_text(ui->screen_mv_tl_in_len);
	float floatValue = str2float(pStrValue);
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_FLOAT;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1006";
	task->payload = std::make_shared<DataPayload>();
	task->payload->dfloat.push_back(floatValue);
	task->PLCTaskSuccessCallback = std::bind([=](){gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage4::getValue2_1, this, std::placeholders::_1));});
	task->PLCTaskFailedCallback = std::bind([=](){});
	gTaskManager.registerTempTask(task);
}
void UIPage4::setValue3_0(){
	auto pStrValue = lv_textarea_get_text(ui->screen_mv_len_scl);
	float floatValue = str2float(pStrValue);
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_FLOAT;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1076";
	task->payload = std::make_shared<DataPayload>();
	task->payload->dfloat.push_back(floatValue);
	task->PLCTaskSuccessCallback = std::bind([=](){gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage4::getValue3_0, this, std::placeholders::_1));});
	task->PLCTaskFailedCallback = std::bind([=](){});
	gTaskManager.registerTempTask(task);
}
void UIPage4::setValue3_1(){
	auto pStrValue = lv_textarea_get_text(ui->screen_mv_tl_ang);
	float floatValue = str2float(pStrValue);
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_FLOAT;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1008";
	task->payload = std::make_shared<DataPayload>();
	task->payload->dfloat.push_back(floatValue);
	task->PLCTaskSuccessCallback = std::bind([=](){gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage4::getValue3_1, this, std::placeholders::_1));});
	task->PLCTaskFailedCallback = std::bind([=](){});
	gTaskManager.registerTempTask(task);
}
void UIPage4::setValue4(){
	auto pStrValue = lv_textarea_get_text(ui->screen_param1_fast_up_spd);
	float floatValue = str2float(pStrValue) * 1000;
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_UINT32;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1062";
	task->payload = std::make_shared<DataPayload>();
	task->payload->duint32.push_back(floatValue);
	task->PLCTaskSuccessCallback = std::bind([=](){gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage4::getValue4, this, std::placeholders::_1));});
	task->PLCTaskFailedCallback = std::bind([=](){});
	gTaskManager.registerTempTask(task);
}
void UIPage4::setValue5(){
	auto pStrValue = lv_textarea_get_text(ui->screen_param1_fast_down_spd);
	float floatValue = str2float(pStrValue) * 1000;
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_UINT32;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1064";
	task->payload = std::make_shared<DataPayload>();
	task->payload->duint32.push_back(floatValue);
	task->PLCTaskSuccessCallback = std::bind([=](){gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage4::getValue5, this, std::placeholders::_1));});
	task->PLCTaskFailedCallback = std::bind([=](){});
	gTaskManager.registerTempTask(task);
}
void UIPage4::setValue6(){
	auto pStrValue = lv_textarea_get_text(ui->screen_param1_slow_ud_spd);
	float floatValue = str2float(pStrValue) * 1000;
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_UINT32;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D1060";
	task->payload = std::make_shared<DataPayload>();
	task->payload->duint32.push_back(floatValue);
	task->PLCTaskSuccessCallback = std::bind([=](){gTaskManager.registerDataArriveOnce("D1000", std::bind(&UIPage4::getValue6, this, std::placeholders::_1));});
	task->PLCTaskFailedCallback = std::bind([=](){});
	gTaskManager.registerTempTask(task);
}

void UIPage4::set_disable(bool disable){
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

void UIPage4::event_init_screen(){
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

void UIPage4::setValue(void* target){
	if(target == ui->screen_fast_mv_spd){
        setValue0_0();
    }
    else if(target == ui->screen_single_mmt){
        setValue0_1();
    }
    else if(target == ui->screen_slow_mv_psd){
        setValue1_0();
    }
    else if(target == ui->screen_mv_alt_spd_1){
        setValue1_1();
    }
    else if(target == ui->screen_mv_alt_spd_2){
        setValue1_2();
    }
    else if(target == ui->screen_fast_mv_spd_scl){
        setValue2_0();
    }
    else if(target == ui->screen_mv_tl_in_len){
        setValue2_1();
    }
    else if(target == ui->screen_mv_len_scl){
        setValue3_0();
    }
    else if(target == ui->screen_mv_tl_ang){
        setValue3_1();
    }
    else if(target == ui->screen_param1_fast_up_spd){
        setValue4();
    }
    else if(target == ui->screen_param1_fast_down_spd){
        setValue5();
    }
    else if(target == ui->screen_param1_slow_ud_spd){
        setValue6();
    }
}