#ifndef __LV_PAGE3_H__
#define __LV_PAGE3_H__
#include <string>
#include <list>
#include "database.h"
#include "lvgl/lvgl.h"

class DefaultRecorderCtx;

class UIPage3{
public:
    typedef struct{
        std::string timestamp;
        std::string evenet;
    }FaultRecordItem;
private:
    typedef struct{
        lv_obj_t* screen_1_background_tile_5;
        lv_obj_t* screen_1_label_85;
        lv_obj_t* screen_1_fault_record;
        lv_obj_t* btnmx;
        lv_obj_t* button_widget;
        int table_row;
    } lv_ui;
    lv_ui* ui;
    std::list<FaultRecordItem> _faultRecordList;
    Database _pDatabase;

    static void btn_event_cb(lv_event_t* e);
public:
    static UIPage3* getInstance(lv_obj_t* parent);
	UIPage3(lv_obj_t* parent);
    void events_init_screen();
    bool insert_record(const std::string & timestamp, 
                        const std::string & event);
    bool load_first_table_record();
    bool load_next_table_record();
    bool load_previous_table_record();
    void update_table();
};


class DefaultRecorderCtx{
public:
    UIPage3* pUIPage3;

};

#endif