#include <sstream>
#include "lv_page2.h"
#include "Format.h"
#include "limlog.h"

WorkRecordCtx gWorkRecordCtx;

LV_FONT_DECLARE(lv_font_cn_songti_bold_25);
LV_FONT_DECLARE(lv_font_cn_songti_medium_21);
LV_FONT_DECLARE(lv_font_cn_songti_medium_21_media_cfg);

UIPage2::UIPage2(lv_obj_t* parent)
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
    ui->table_row = 30;
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
    lv_obj_set_style_text_font(ui->btnmx, &lv_font_cn_songti_medium_21, 0);
    //lv_obj_add_event_cb(btnmx, setting_btnmatrix_event_cb, LV_EVENT_VALUE_CHANGED, this);

    events_init_screen();

    gWorkRecordCtx.pUIPage2 = this;

    //连接数据库
    _pDatabase.connect();
    //加载表
    load_first_table_record();
    update_table();
}

void UIPage2::events_init_screen(){
    lv_obj_add_event_cb(ui->btnmx, btn_event_cb, LV_EVENT_VALUE_CHANGED, this);
}

bool UIPage2::insert_record(const std::string & timestamp, 
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
    std::string checksize_sql = "DELETE FROM work_record "      \
                                "WHERE timestamp = ( "          \
                                    "SELECT timestamp "         \
                                    "FROM work_record "         \
                                    "ORDER BY timestamp ASC "   \
                                    "LIMIT 1 "                  \
                                ") "                            \
                                "AND (SELECT COUNT(*) FROM work_record) > 2000";
    _workRecordList.push_front(WorkRecordItem{timestamp, arg1, arg2, arg3, arg4, arg5});
    if(_workRecordList.size() > ui->table_row){
        _workRecordList.pop_back();
    }
    //创建数据库表
    bool ret = _pDatabase.excute(create_table_sql, resp);
    if(!ret) return ret;
    //插入数据
    ret = _pDatabase.excute(insert_sql, resp);
    if(!ret) return ret;
    //检查大小
    ret = _pDatabase.excute(checksize_sql, resp);
    if(!ret) return ret;
    //更新表
    update_table();
    return true;
}

bool UIPage2::load_first_table_record(){
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
    lv_obj_scroll_to(ui->screen_1_background_tile_4, 0, 0, LV_ANIM_OFF);
    return true;
}

bool UIPage2::load_next_table_record(){
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
    lv_obj_scroll_to(ui->screen_1_background_tile_4, 0, 0, LV_ANIM_OFF);
    return true;
}

bool UIPage2::load_previous_table_record(){
    SQL_Response resp;
    std::string sql_fmt = "SELECT * FROM work_record "\
                    "WHERE timestamp > {0} "\
                    "ORDER BY timestamp DESC "\
                    "LIMIT 30;";
    auto item = _workRecordList.front();
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
    lv_obj_scroll_to(ui->screen_1_background_tile_4, 0, 0, LV_ANIM_OFF);
    return true;
}


void UIPage2::update_table(){
    std::stringstream ss;
    int row;
    for(row = 0; row < _workRecordList.size()&&row < ui->table_row; row++){
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

    for(; row < ui->table_row; row++){
        lv_table_set_cell_value(ui->screen_1_work_record, row, 0, "");
        lv_table_set_cell_value(ui->screen_1_work_record, row, 1, "");
        lv_table_set_cell_value(ui->screen_1_work_record, row, 2, "");
        lv_table_set_cell_value(ui->screen_1_work_record, row, 3, "");
        lv_table_set_cell_value(ui->screen_1_work_record, row, 4, "");
        lv_table_set_cell_value(ui->screen_1_work_record, row, 5, "");
        lv_table_set_cell_value(ui->screen_1_work_record, row, 6, "");
    }
    lv_obj_align_to(ui->button_widget, ui->screen_1_work_record, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
}

void UIPage2::btn_event_cb(lv_event_t* e){
    UIPage2* pWind = reinterpret_cast<UIPage2*>(lv_event_get_user_data(e));
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