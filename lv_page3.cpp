#include <sstream>
#include "Format.h"
#include "limlog.h"
#include "lv_page3.h"

LV_FONT_DECLARE(lv_font_cn_songti_bold_25);
LV_FONT_DECLARE(lv_font_cn_songti_medium_21);
LV_FONT_DECLARE(lv_font_cn_songti_medium_21_media_cfg);

UIPage3::UIPage3(lv_obj_t* parent)
:_pDatabase(DATABASE_FILE){
    ui = new lv_ui;
    ui->screen_1_background_tile_5 = parent;
    if(parent == NULL){
        printf("Receive nullptr as parent.\n");
        return;
    }

    //Write codes screen_1_label_85
    ui->screen_1_label_85 = lv_label_create(ui->screen_1_background_tile_5);
    lv_label_set_text(ui->screen_1_label_85, "故障事件记录");
    lv_label_set_long_mode(ui->screen_1_label_85, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_85, 372, 21);
    lv_obj_set_size(ui->screen_1_label_85, 263, 26);

    //Write style for screen_1_label_85, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_85, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_85, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_85, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_85, &lv_font_cn_songti_bold_25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_85, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_85, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_85, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_85, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_85, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_85, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_85, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_85, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_85, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_85, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_1_fault_record
    ui->screen_1_fault_record = lv_table_create(ui->screen_1_background_tile_5);
    lv_table_set_col_cnt(ui->screen_1_fault_record, 3);
    lv_table_set_row_cnt(ui->screen_1_fault_record, 1);
    lv_table_set_cell_value(ui->screen_1_fault_record, 0, 0, "序号");
    lv_table_set_cell_value(ui->screen_1_fault_record, 0, 1, "日期时间");
    lv_table_set_cell_value(ui->screen_1_fault_record, 0, 2, "故障事件");
    lv_obj_set_pos(ui->screen_1_fault_record, 0, 63);
    lv_obj_set_scrollbar_mode(ui->screen_1_fault_record, LV_SCROLLBAR_MODE_ACTIVE);
    lv_table_set_col_width(ui->screen_1_fault_record, 2, 600);

    //Write style for screen_1_fault_record, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_align_to(ui->screen_1_fault_record, ui->screen_1_label_85, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_pad_top(ui->screen_1_fault_record, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_fault_record, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_fault_record, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_fault_record, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_fault_record, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_fault_record, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_fault_record, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_fault_record, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_fault_record, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_fault_record, lv_color_hex(0xd5dee6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_fault_record, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_fault_record, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_fault_record, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_1_fault_record, Part: LV_PART_MAIN, State: LV_STATE_FOCUSED.
    lv_obj_set_style_pad_top(ui->screen_1_fault_record, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_pad_bottom(ui->screen_1_fault_record, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_pad_left(ui->screen_1_fault_record, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_pad_right(ui->screen_1_fault_record, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui->screen_1_fault_record, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_color(ui->screen_1_fault_record, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_fault_record, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_width(ui->screen_1_fault_record, 2, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_opa(ui->screen_1_fault_record, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_color(ui->screen_1_fault_record, lv_color_hex(0xd5dee6), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(ui->screen_1_fault_record, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_radius(ui->screen_1_fault_record, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_width(ui->screen_1_fault_record, 0, LV_PART_MAIN | LV_STATE_FOCUSED);

    //Write style for screen_1_fault_record, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_fault_record, lv_color_hex(0x393c41), LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_fault_record, &lv_font_cn_songti_medium_21, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_fault_record, 255, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_fault_record, LV_TEXT_ALIGN_CENTER, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_fault_record, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_fault_record, 3, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_fault_record, 255, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_fault_record, lv_color_hex(0xd5dee6), LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_fault_record, LV_BORDER_SIDE_FULL, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_fault_record, 10, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_fault_record, 10, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_fault_record, 10, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_fault_record, 10, LV_PART_ITEMS | LV_STATE_DEFAULT);

    //Write style for screen_1_fault_record, Part: LV_PART_ITEMS, State: LV_STATE_FOCUSED.
    lv_obj_set_style_text_color(ui->screen_1_fault_record, lv_color_hex(0x393c41), LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_text_font(ui->screen_1_fault_record, &lv_font_cn_songti_medium_21, LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_text_opa(ui->screen_1_fault_record, 255, LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_text_align(ui->screen_1_fault_record, LV_TEXT_ALIGN_CENTER, LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui->screen_1_fault_record, 0, LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_border_width(ui->screen_1_fault_record, 3, LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_border_opa(ui->screen_1_fault_record, 255, LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_border_color(ui->screen_1_fault_record, lv_color_hex(0xd5dee6), LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(ui->screen_1_fault_record, LV_BORDER_SIDE_FULL, LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_pad_top(ui->screen_1_fault_record, 10, LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_pad_bottom(ui->screen_1_fault_record, 10, LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_pad_left(ui->screen_1_fault_record, 10, LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_pad_right(ui->screen_1_fault_record, 10, LV_PART_ITEMS | LV_STATE_FOCUSED);
    ui->table_row = 31;
    lv_table_set_row_cnt(ui->screen_1_fault_record, ui->table_row);

    /* button widget */
    ui->button_widget = lv_obj_create(ui->screen_1_background_tile_5);
    lv_obj_set_size(ui->button_widget, 375, 40);
    lv_obj_align_to(ui->button_widget, ui->screen_1_fault_record, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
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
    lv_obj_set_style_text_font(ui->btnmx, &lv_font_cn_songti_medium_21_media_cfg, 0);
    //lv_obj_add_event_cb(btnmx, setting_btnmatrix_event_cb, LV_EVENT_VALUE_CHANGED, this);
    
    events_init_screen();
    //连接数据库
    _pDatabase.connect();
    //加载表
    load_first_table_record();
    update_table();
}

void UIPage3::events_init_screen(){
    lv_obj_add_event_cb(ui->btnmx, btn_event_cb, LV_EVENT_VALUE_CHANGED, this);
}

bool UIPage3::insert_record(const std::string & timestamp, 
                        const std::string & event){
    SQL_Response resp;
    std::string create_table_sql = "CREATE TABLE IF NOT EXISTS fault_record(\n"\
                                "timestamp TIMESTAMP PRIMARY KEY,\n"\
                                "event VARCHAR(50));";
    std::string insert_sql_fmt = "INSERT INTO fault_record VALUES({0}, {1});";
    std::string insert_sql = util::Format(insert_sql_fmt, timestamp, event);
    _faultRecordList.push_front(FaultRecordItem{timestamp, event});
    if(_faultRecordList.size() > 30){
        _faultRecordList.pop_back();
    }
    //创建数据库表
    bool ret = _pDatabase.excute(create_table_sql, resp);
    if(!ret) return ret;
    //插入数据
    ret = _pDatabase.excute(insert_sql, resp);
    if(!ret) return ret;
    //更新表
    update_table();
    return true;
}

bool UIPage3::load_first_table_record(){
    SQL_Response resp;
    std::string sql = "SELECT * FROM fault_record "\
                    "ORDER BY timestamp DESC "\
                    "LIMIT 30;";
    bool ret = _pDatabase.excute(sql, resp);
    if(!ret) return ret;
    if(resp.size() > 0){
        _faultRecordList.clear();
        for(auto iter = resp.begin(); iter != resp.end(); iter++){
            FaultRecordItem item{iter->at("timestamp"), iter->at("event")};
            _faultRecordList.push_back(item);
        }
    }
    return true;
}

bool UIPage3::load_next_table_record(){
    SQL_Response resp;
    std::string sql_fmt = "SELECT * FROM fault_record "\
                    "WHERE timestamp < {0} "\
                    "ORDER BY timestamp DESC "\
                    "LIMIT 30;";
    auto item = _faultRecordList.back();
    std::string sql = util::Format(sql_fmt, item.timestamp);
    bool ret = _pDatabase.excute(sql, resp);
    if(!ret) return ret;
    if(resp.size() > 0){
        _faultRecordList.clear();
        for(auto iter = resp.begin(); iter != resp.end(); iter++){
            FaultRecordItem item{iter->at("timestamp"), iter->at("event")};
            _faultRecordList.push_back(item);
        }
    }
    return true;
}

bool UIPage3::load_previous_table_record(){
    SQL_Response resp;
    std::string sql_fmt = "SELECT * FROM fault_record "\
                    "WHERE timestamp > {0} "\
                    "ORDER BY timestamp DESC "\
                    "LIMIT 30;";
    auto item = _faultRecordList.front();
    std::string sql = util::Format(sql_fmt, item.timestamp);
    bool ret = _pDatabase.excute(sql, resp);
    if(!ret) return ret;
    if(resp.size() > 0){
        _faultRecordList.clear();
        for(auto iter = resp.begin(); iter != resp.end(); iter++){
            FaultRecordItem item{iter->at("timestamp"), iter->at("event")};
            _faultRecordList.push_back(item);
        }
    }
    return true;
}

void UIPage3::update_table(){
    std::stringstream ss;
    for(int row = 1; row < _faultRecordList.size()&&row < ui->table_row; row++){
        //序号
        ss.str("");
        ss << (row);
        lv_table_set_cell_value(ui->screen_1_fault_record, row, 0, ss.str().c_str());
        //本地时间
        ss.str("");
        time_t timestamp = (time_t)atoi(std::next(_faultRecordList.begin(), row)->timestamp.c_str());
        struct tm* local_time = localtime(&timestamp);
        char formatted_time[100];
        size_t size = strftime(formatted_time, sizeof(formatted_time),
                            "%Y-%m-%d %H:%M:%S", local_time);
        if (size == 0) {
            LOG_ERROR << "Formating time error: " << timestamp;
        }
        ss << formatted_time;
        lv_table_set_cell_value(ui->screen_1_fault_record, row, 1, ss.str().c_str());
        ss.str("");
        ss << std::next(_faultRecordList.begin(), row)->evenet;
        lv_table_set_cell_value(ui->screen_1_fault_record, row, 2, ss.str().c_str());
    }
    lv_obj_align_to(ui->button_widget, ui->screen_1_fault_record, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
}

void UIPage3::btn_event_cb(lv_event_t* e){
    UIPage3* pWind = reinterpret_cast<UIPage3*>(lv_event_get_user_data(e));
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