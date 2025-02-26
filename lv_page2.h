#ifndef __LV_PAGE2_H__
#define __LV_PAGE2_H__
#include <string>
#include <list>
#include "database.h"
#include "lvgl/lvgl.h"

class UIPage2{
public:
    struct WorkRecordItem{
        std::string timestamp;
        std::string arg1;
        std::string arg2;
        std::string arg3;
        std::string arg4;
        std::string arg5;
    };
private:
    typedef struct{
        int table_row;
        lv_obj_t* screen_1_background_tile_4;
        lv_obj_t* screen_1_label_84;
        lv_obj_t* screen_1_work_record;
        lv_obj_t* btnmx;
        lv_obj_t* button_widget;
    }lv_ui;
    lv_ui* ui;
    std::list<WorkRecordItem> _workRecordList;
    Database _pDatabase;
    static void btn_event_cb(lv_event_t* e);
public:
    static UIPage2* getInstance(lv_obj_t* parent);
	UIPage2(lv_obj_t* parent);
    void events_init_screen();
    bool insert_record(const std::string & timestamp, 
                        const std::string & arg1, 
                        const std::string & arg2, 
                        const std::string & arg3, 
                        const std::string & arg4, 
                        const std::string & arg5);
    bool load_first_table_record();
    bool load_next_table_record();
    bool load_previous_table_record();
    void update_table();
};


#endif