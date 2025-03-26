#include <time.h>
#include <pthread.h>
#include <fstream>
#include <functional>
#include "setting.h"
#include "lv_UITopPage.h"
#include "task_manager.h"
#include "lv_message_window.h"
#include "helper.h"
#include "limlog.h"

#define BATTRERY_ADC_FS "/sys/class/gpadc/data"

LV_FONT_DECLARE(lv_font_cn_songti_medium_21);
LV_FONT_DECLARE(lv_font_cn_songti_bold_25);
LV_IMG_DECLARE(column_30x30);
LV_IMG_DECLARE(battery_4);

extern pthread_mutex_t lv_lock;
extern TaskManager gTaskManager;

UITopPage::UITopPage()
:pAccessContrl_(AccessContrl::getInstance()),
pLoggingWind_(LoggingWind::getInstance()),
pModifyPasswordWind_(ModifyPasswordWind::getInstance()){
    /* 顶部部件 */
    static lv_style_t topbox_st;
    lv_style_init(&topbox_st);
    lv_style_set_width(&topbox_st, lv_pct(100));
    lv_style_set_text_font(&topbox_st, &lv_font_cn_songti_medium_21);
    lv_style_set_flex_flow(&topbox_st, LV_FLEX_FLOW_ROW);
    lv_style_set_flex_cross_place(&topbox_st, LV_FLEX_ALIGN_CENTER);
    lv_style_set_flex_track_place(&topbox_st, LV_FLEX_ALIGN_CENTER);
    lv_style_set_layout(&topbox_st, LV_LAYOUT_FLEX);
    lv_style_set_pad_all(&topbox_st, 0);
    lv_style_set_flex_grow(&topbox_st, 1);
    lv_style_set_border_width(&topbox_st, 0);
    // 图片部件style
    static lv_style_t img_label_box_style;
    lv_style_init(&img_label_box_style);
    lv_style_set_height(&img_label_box_style, lv_pct(100));
    lv_style_set_flex_grow(&img_label_box_style, 1);
    lv_style_set_flex_flow(&img_label_box_style, LV_FLEX_FLOW_ROW);
    lv_style_set_flex_main_place(&img_label_box_style, LV_FLEX_ALIGN_START);
    lv_style_set_flex_cross_place(&img_label_box_style, LV_FLEX_ALIGN_CENTER);
    lv_style_set_flex_track_place(&img_label_box_style, LV_FLEX_ALIGN_CENTER);
    lv_style_set_layout(&img_label_box_style, LV_LAYOUT_FLEX);
    lv_style_set_pad_all(&img_label_box_style, 0);
    lv_style_set_border_width(&img_label_box_style, 0);

    lv_obj_t* topbox = lv_obj_create(lv_scr_act());
    lv_obj_add_style(topbox, &topbox_st, 0);
    lv_obj_set_size(topbox, lv_pct(100), 50);
    //工作主页标签
    top_label = lv_label_create(topbox);
	lv_label_set_text(top_label, "用户：(操作员)");
    //lv_label_set_text(top_label, "嘉盟电力设备");
    lv_obj_set_style_text_color(top_label, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_set_style_text_font(top_label, &lv_font_cn_songti_bold_25, LV_PART_MAIN);
    lv_obj_set_style_pad_left(top_label, 20, LV_STATE_DEFAULT);
    lv_obj_set_style_flex_grow(top_label, 3, 0);
    //时间部件
    top_datetime = lv_label_create(topbox);
    lv_label_set_text(top_datetime, "2024-10-28 18:00:00");
    lv_obj_set_style_text_font(top_datetime, &lv_font_cn_songti_medium_21, LV_PART_MAIN);
    lv_obj_set_style_flex_grow(top_datetime, 3, 0);
    lv_obj_set_style_pad_left(top_datetime, 20, 0);
    //材料容量
    lv_obj_t* material_volumn_box = lv_obj_create(topbox);
    lv_obj_add_style(material_volumn_box, &img_label_box_style, 0);
    lv_obj_t* material_img = lv_img_create(material_volumn_box);
    lv_img_set_src(material_img, &column_30x30);
    material_label = lv_label_create(material_volumn_box);
    lv_label_set_text(material_label, "50%");
    //电池容量
    lv_obj_t* battery_volumn_box = lv_obj_create(topbox);
    lv_obj_add_style(battery_volumn_box, &img_label_box_style, 0);
    battery_img = lv_img_create(battery_volumn_box);
    lv_img_set_src(battery_img, &battery_4);
    battery_label = lv_label_create(battery_volumn_box);
    lv_label_set_text(battery_label, "50%");
    /* 顶部部件 */

    /* 底部部件 */
    _tile_wind = lv_tileview_create(lv_scr_act());
    lv_obj_set_size(_tile_wind, lv_pct(100), 768 - 50 - 5);
    lv_obj_set_style_border_width(_tile_wind, 0, LV_STATE_DEFAULT);
    lv_obj_align_to(_tile_wind, topbox, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
	/* 底部部件 */

    /* 物理键盘 */
    keyboard_opa_t opa;
    opa.key0_cb = keyboard_cb_t{
        std::bind(&UITopPage::keyxPress_cb, this, 0),
        std::bind(&UITopPage::keyxRelease_cb, this, 0)};
    opa.key1_cb = keyboard_cb_t{
        std::bind(&UITopPage::keyxPress_cb, this, 1),
        std::bind(&UITopPage::keyxRelease_cb, this, 1)};
    opa.key2_cb = keyboard_cb_t{
        std::bind(&UITopPage::keyxPress_cb, this, 2),
        std::bind(&UITopPage::keyxRelease_cb, this, 2)};
    opa.key3_cb = keyboard_cb_t{
        std::bind(&UITopPage::keyxPress_cb, this, 3),
        std::bind(&UITopPage::keyxRelease_cb, this, 3)};
    opa.key4_cb = keyboard_cb_t{
        std::bind(&UITopPage::keyxPress_cb, this, 4),
        std::bind(&UITopPage::keyxRelease_cb, this, 4)};
    opa.key5_cb = keyboard_cb_t{
        std::bind(&UITopPage::keyxPress_cb, this, 5),
        std::bind(&UITopPage::keyxRelease_cb, this, 5)};
    opa.key6_cb = keyboard_cb_t{
        std::bind(&UITopPage::keyxPress_cb, this, 6),
        std::bind(&UITopPage::keyxRelease_cb, this, 6)};
    opa.key7_cb = keyboard_cb_t{
        std::bind(&UITopPage::keyxPress_cb, this, 7),
        std::bind(&UITopPage::keyxRelease_cb, this, 7)};
    opa.key8_cb = keyboard_cb_t{
        std::bind(&UITopPage::keyxPress_cb, this, 8),
        std::bind(&UITopPage::keyxRelease_cb, this, 8)};
    opa.key9_cb = keyboard_cb_t{
        std::bind(&UITopPage::keyxPress_cb, this, 9),
        std::bind(&UITopPage::keyxRelease_cb, this, 9)};
    opa.key10_cb = keyboard_cb_t{
        std::bind(&UITopPage::keyxPress_cb, this, 10),
        std::bind(&UITopPage::keyxRelease_cb, this, 10)};
    opa.key11_cb = keyboard_cb_t{
        std::bind(&UITopPage::keyxPress_cb, this, 11),
        std::bind(&UITopPage::keyxRelease_cb, this, 11)};
    opa.key12_cb = keyboard_cb_t{
        std::bind(&UITopPage::keyxPress_cb, this, 12),
        std::bind(&UITopPage::keyxRelease_cb, this, 12)};
    opa.key13_cb = keyboard_cb_t{
        std::bind(&UITopPage::keyxPress_cb, this, 13),
        std::bind(&UITopPage::keyxRelease_cb, this, 13)};
    opa.key14_cb = keyboard_cb_t{
        std::bind(&UITopPage::keyxPress_cb, this, 14),
        std::bind(&UITopPage::keyxRelease_cb, this, 14)};
    opa.key15_cb = keyboard_cb_t{
        std::bind(&UITopPage::loggingKeyPress_cb, this),
        std::bind(&UITopPage::loggingKeyRelease_cb, this)};
    opa.key16_cb = keyboard_cb_t{
        std::bind(&UITopPage::modifyPwdPress_cb, this),
        std::bind(&UITopPage::modifyPwdRelease_cb, this)};
    opa.key17_cb = keyboard_cb_t{
        std::bind(&UITopPage::logginoutKeyPress_cb, this),
        std::bind(&UITopPage::logginoutKeyRelease_cb, this)};
    opa.key18_cb = keyboard_cb_t{
        std::bind(&UITopPage::keyxPress_cb, this, 18),
        std::bind(&UITopPage::keyxRelease_cb, this, 18)};
    opa.key19_cb = keyboard_cb_t{
        std::bind(&UITopPage::keyxPress_cb, this, 19),
        std::bind(&UITopPage::keyxRelease_cb, this, 19)};
    opa.key20_cb = keyboard_cb_t{
        std::bind(&UITopPage::keyxPress_cb, this, 20),
        std::bind(&UITopPage::keyxRelease_cb, this, 20)};
    opa.key21_cb = keyboard_cb_t{
        std::bind(&UITopPage::mainwindKeyPress_cb, this),
        std::bind(&UITopPage::mainwindKeyRelease_cb, this)};
    opa.key22_cb = keyboard_cb_t{
        std::bind(&UITopPage::prewindKeyPress_cb, this),
        std::bind(&UITopPage::prewindKeyRelease_cb, this)};
    opa.key23_cb = keyboard_cb_t{
        std::bind(&UITopPage::nextwindKeyPress_cb, this),
        std::bind(&UITopPage::nextwindKeyRelease_cb, this)};
    pPhyKeyboard_ = PhyKeyboard::getInstance(opa);
    /* 物理键盘 */

	//开启时钟
	SystemClockThread_ = std::thread(std::bind(&UITopPage::systemClockHandler_, this));

	// page0
	lv_obj_t* page0_tile = lv_tileview_add_tile(_tile_wind, 0, 0, LV_DIR_RIGHT);
	lv_obj_t* page1_tile = lv_tileview_add_tile(_tile_wind, 1, 0, LV_DIR_LEFT|LV_DIR_RIGHT);
	lv_obj_t* page2_tile = lv_tileview_add_tile(_tile_wind, 2, 0, LV_DIR_LEFT|LV_DIR_RIGHT);
	lv_obj_t* page3_tile = lv_tileview_add_tile(_tile_wind, 3, 0, LV_DIR_LEFT|LV_DIR_RIGHT);
    lv_obj_t* page4_tile = lv_tileview_add_tile(_tile_wind, 4, 0, LV_DIR_LEFT|LV_DIR_RIGHT);
    lv_obj_t* page5_tile = lv_tileview_add_tile(_tile_wind, 5, 0, LV_DIR_LEFT);
	pUIPage0_ = std::make_shared<UIPage0>(page0_tile);
	pUIPage1_ = std::make_shared<UIPage1>(page1_tile);
	pUIPage2_ = std::make_shared<UIPage2>(page2_tile);
	pUIPage3_ = std::make_shared<UIPage3>(page3_tile);
    pUIPage4_ = std::make_shared<UIPage4>(page4_tile);
    pUIPage5_ = std::make_shared<UIPage5>(page5_tile);

    setIdentity(pAccessContrl_->curent_identity());
    //access contrl callback
    pAccessContrl_->setCallback(std::bind(&UITopPage::setIdentity, this, std::placeholders::_1));

}

UITopPage::~UITopPage(){

}

void UITopPage::setIdentity(identity_t identity){
    if(identity == MANUPULATOR){
        lv_label_set_text(top_label, "用户：(操作员)");
        pUIPage4_->set_disable(true);
        pUIPage5_->set_disable(true);
    }
    else if(identity == MANAGER){
        lv_label_set_text(top_label, "用户：(管理员)");
        pUIPage4_->set_disable(false);
        pUIPage5_->set_disable(false);
    }
}

void UITopPage::systemClockHandler_(){
    using namespace std;
    char time_str[64]{0};
    int time_count = 30;
    while(1){
        //设置时间
        auto time_stamp = time(NULL);
        auto* time_tm = localtime(&time_stamp);
        memset(time_str, 0, sizeof(time_str));
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", time_tm);
        pthread_mutex_lock(&lv_lock);
        lv_label_set_text(top_datetime, time_str);
        pthread_mutex_unlock(&lv_lock);
        if(++time_count >= 30){
            //更新材料容量

            //更新电池电量
            fstream fs;
            double adc_value;
            fs.open(BATTRERY_ADC_FS, ios::in);
            fs >> adc_value;
            double pct = (adc_value - 1400)/(1800-1400);
            int battery_val = static_cast<int>(pct * 100);
            pthread_mutex_lock(&lv_lock);
            lv_label_set_text_fmt(battery_label, "%d%", battery_val);
            pthread_mutex_unlock(&lv_lock);
            fs.close();
            time_count = 0;

        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void UITopPage::keyxPress_cb(int x){
    auto item_ptr = KEY_PLC_REG_MAP.find(x);
    if(item_ptr == KEY_PLC_REG_MAP.end())
        return;
    printf("key \"%d\" pressed.\n", x);

	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_BOOL;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = item_ptr->second;
	task->payload = std::make_shared<DataPayload>();
	task->payload->dbool.push_back(true);
	task->PLCTaskSuccessCallback = std::bind([=](){});
	task->PLCTaskFailedCallback = std::bind([=](){});
	gTaskManager.registerTempTask(task);
}
void UITopPage::keyxRelease_cb(int x){
    auto item_ptr = KEY_PLC_REG_MAP.find(x);
    if(item_ptr == KEY_PLC_REG_MAP.end())
        return;
    printf("key \"%d\" release.\n", x);

	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_BOOL;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = item_ptr->second;
	task->payload = std::make_shared<DataPayload>();
	task->payload->dbool.push_back(false);
	task->PLCTaskSuccessCallback = std::bind([=](){});
	task->PLCTaskFailedCallback = std::bind([=](){});
	gTaskManager.registerTempTask(task);
}

void UITopPage::mainwindKeyPress_cb(){
    _tile_index = 0;
    pPhyKeyboard_->keyboard_led_turn(21, true);
    pPhyKeyboard_->keyboard_led_update();
    lv_obj_set_tile_id(_tile_wind, _tile_index, 0, LV_ANIM_ON);
    printf("main window key pressed.\n");
}
void UITopPage::mainwindKeyRelease_cb(){
    pPhyKeyboard_->keyboard_led_turn(21, false);
    pPhyKeyboard_->keyboard_led_update();
    printf("main window key released.\n");
}
void UITopPage::prewindKeyPress_cb(){
    --_tile_index;
    pPhyKeyboard_->keyboard_led_turn(22, true);
    pPhyKeyboard_->keyboard_led_update();
    if(_tile_index < 0) _tile_index = 0;
    lv_obj_set_tile_id(_tile_wind, _tile_index, 0, LV_ANIM_ON);
    printf("previous window key pressed.\n");
}
void UITopPage::prewindKeyRelease_cb(){
    pPhyKeyboard_->keyboard_led_turn(22, false);
    pPhyKeyboard_->keyboard_led_update();
    printf("previous window key released.\n");
}
void UITopPage::nextwindKeyPress_cb(){
    ++_tile_index;
    pPhyKeyboard_->keyboard_led_turn(23, true);
    pPhyKeyboard_->keyboard_led_update();
    if(_tile_index > 5)_tile_index = 5;
    lv_obj_set_tile_id(_tile_wind, _tile_index, 0, LV_ANIM_ON);
    printf("next window key pressed.\n");
}
void UITopPage::nextwindKeyRelease_cb(){
    pPhyKeyboard_->keyboard_led_turn(23, false);
    pPhyKeyboard_->keyboard_led_update();
    printf("next window key released.\n");
}
void UITopPage::modifyPwdPress_cb(){
    pPhyKeyboard_->keyboard_led_turn(16, true);
    pPhyKeyboard_->keyboard_led_update();
    printf("modify password key pressed.\n");
    pModifyPasswordWind_->show();
}
void UITopPage::modifyPwdRelease_cb(){
    pPhyKeyboard_->keyboard_led_turn(16, false);
    pPhyKeyboard_->keyboard_led_update();
    printf("modify password key released.\n");
}
void UITopPage::loggingKeyPress_cb(){
    pPhyKeyboard_->keyboard_led_turn(15, true);
    pPhyKeyboard_->keyboard_led_update();
    printf("logging key pressed.\n");
    pLoggingWind_->show();
}
void UITopPage::loggingKeyRelease_cb(){
    pPhyKeyboard_->keyboard_led_turn(15, false);
    pPhyKeyboard_->keyboard_led_update();
    printf("logging key released.\n");
}
void UITopPage::logginoutKeyPress_cb(){
    pPhyKeyboard_->keyboard_led_turn(17, true);
    pPhyKeyboard_->keyboard_led_update();
    printf("logging out key pressed.\n");
    pAccessContrl_->logout();
}
void UITopPage::logginoutKeyRelease_cb(){
    pPhyKeyboard_->keyboard_led_turn(17, false);
    pPhyKeyboard_->keyboard_led_update();
    printf("logging out key released.\n");
}