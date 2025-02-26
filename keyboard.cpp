#include <functional>
#include "lv_uiparampage.h"
#include "lvgl/lvgl.h"
#include "keyboard.h"
#include "limlog.h"


PhyKeyboard* PhyKeyboard::pKeyboard = nullptr;

void* physical_keyboard_listen_thread(void* args){
	PhyKeyboard* keyboard = (PhyKeyboard*)args;
	std::string readstr;
	int readlen;
	while(1){
		/*keyboard->keyboard_led_update();
		readlen = keyboard->serial.readData(readstr, 10000);
		if(readlen <= 0)continue;
		LOG_DEBUG << "serial read " << readlen << " data: ";
		for(int i = 0; i < readlen; i++){
			uint8_t cmd = readstr[i];
			printf("%2x ", cmd);
			keyboard_cb_t cb = keyboard->keyboard_callback(cmd & 0x7f);
			if(cmd >> 7){	//released
				if(cb.pReleasedCallback){
					cb.pReleasedCallback(cb.pData);
				}
			}
			else{
				if(cb.pPressedCallback){
					cb.pPressedCallback(cb.pData);
				}
			}
		}
		printf("\n");
		*/
		char cmd;
		//keyboard->keyboard_led_update();
		readlen = keyboard->serial.readByte(cmd);
		if(readlen > 0){
			printf("read %d byte: %2x\n", readlen, cmd);
			keyboard_cb_t cb = keyboard->keyboard_callback(cmd & 0x7f);
			if(cmd >> 7){	//released
				if(cb.pReleasedCallback){
					cb.pReleasedCallback();
				}
			}
			else{
				if(cb.pPressedCallback){
					cb.pPressedCallback();
				}
			}
		}
		usleep(10000);
	}
}

PhyKeyboard::PhyKeyboard(const keyboard_opa_t & opa)
:_opa(opa), serial("/dev/ttyAS2", 9600)
{
	int readlen;
	std::string readstr;
	for(int i = 0; i < KEYBOARD_LED_ROW; i++){
        keyboard_led_row_t row;
        for(int j = 0; j < KEYBOARD_LED_COL; j++)
            row.push_back(std::make_shared<std::atomic<bool>>());
		_led_matrix.push_back(row);
	}
	//假读
	readlen = this->serial.readData(readstr, 10000);
	pthread_create(&listen_thread, NULL, physical_keyboard_listen_thread, this);
    pthread_detach(listen_thread);
}

PhyKeyboard::~PhyKeyboard(){

}

void PhyKeyboard::keyboard_led_turn(uint8_t index, bool on){
	if(index > 23){
		LOG_ERROR << "invalid keyboard led index.";
		return;
	}
	uint8_t row = index % KEYBOARD_LED_ROW;
	uint8_t col = index / KEYBOARD_LED_ROW;
	if(_led_matrix[row][col]->load() != on)
		_update_flag = true;
    _led_matrix[row][col]->store(on);
}

void PhyKeyboard::keyboard_led_update(){
	uint8_t sendbuf[KEYBOARD_LED_ROW * 2]{0};
	if(_update_flag == false)return;
	for(int i = 0; i < _led_matrix.size(); i++){
		keyboard_led_row_t row = _led_matrix[i];
		uint8_t ch = ((uint8_t)row[0]->load() << 0) | 
                    ((uint8_t)row[1]->load() << 1) | 
                    ((uint8_t)row[2]->load() << 2) | 
                    ((uint8_t)row[3]->load() << 3);
		sendbuf[i * 2] = i;
		sendbuf[i * 2 + 1] = ch;
		printf("%2x %2x ", i, ch);
	}
	printf("\n");
	serial.sendData((char*)sendbuf, size_t(KEYBOARD_LED_ROW * 2));
	_update_flag = false;
}

keyboard_cb_t PhyKeyboard::keyboard_callback(int index){
	keyboard_cb_t* ptr = &(_opa.key0_cb);
	if(index < 0 || index > 23){
		return keyboard_cb_t{0};
	}
	ptr += index;
	return *ptr;
}

PhyKeyboard* PhyKeyboard::getInstance(const keyboard_opa_t & opa){
	if(pKeyboard == nullptr){
		pKeyboard = new PhyKeyboard(opa);
	}
	return pKeyboard;
}

VirtualKeyboard::VirtualKeyboard(){
    /* 创建键盘 */
    _keyboard = lv_keyboard_create(lv_layer_sys());
    lv_obj_add_flag(_keyboard, LV_OBJ_FLAG_HIDDEN);
}

VirtualKeyboard::~VirtualKeyboard(){
    lv_obj_del(_keyboard);
}

VirtualKeyboard* VirtualKeyboard::getInstance(){
    static VirtualKeyboard _keyboard;
    return &_keyboard;
}

void VirtualKeyboard::show(){
    lv_keyboard_set_textarea(_keyboard, _target);
    lv_obj_set_style_max_height(_keyboard, LV_HOR_RES * 2 / 1, 0);
    lv_obj_clear_flag(_keyboard, LV_OBJ_FLAG_HIDDEN);
    lv_obj_scroll_to_view_recursive(_target, LV_ANIM_OFF);
    /* 移动构件 */
    lv_area_t text_area, kb_area;
    lv_obj_get_coords(_target, &text_area);
    lv_obj_get_coords(_keyboard, &kb_area);
    int offset = text_area.y1 + 50 - kb_area.y1;
    /* 保存原始坐标 */
    lv_area_t wind_area;
    lv_obj_get_coords(_parent_ant, &wind_area);
    wind_pre_area = wind_area;
    if (offset > 0) {
        uint32_t x = wind_area.x1;
        uint32_t y = wind_area.y1 - offset;
        lv_obj_move_to(_parent_ant, x, y);
    }
}

void VirtualKeyboard::hind(){
    /* 恢复原始坐标 */
    lv_obj_move_to(_parent_ant, wind_pre_area.x1, wind_pre_area.y1);
    lv_keyboard_set_textarea(_keyboard, NULL);
    lv_obj_add_flag(_keyboard, LV_OBJ_FLAG_HIDDEN);
}

void VirtualKeyboard::keyboard_event_cb(lv_event_t* e){
    static lv_area_t wind_pre_area;
    VirtualKeyboard* keyboard = (VirtualKeyboard*)lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);
    keyboard->_target = lv_event_get_target(e);
    /* 获得顶层父节点 */
    lv_obj_t* parent = keyboard->_target;
    keyboard->_parent_ant = keyboard->_target;
    for (;parent != lv_layer_top() && parent != lv_scr_act();) {
        parent = lv_obj_get_parent(keyboard->_parent_ant);
        if (parent != lv_layer_top() && parent != lv_scr_act()) {
            keyboard->_parent_ant = parent;
        }
    }
    
    if (code == LV_EVENT_FOCUSED) {
        keyboard->show();
    }
    else if (code == LV_EVENT_DEFOCUSED) {
        keyboard->hind();
    }
    else if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
        keyboard->hind();
        lv_obj_clear_state(keyboard->_target, LV_STATE_FOCUSED);
    }
}

void keyboard_event_cb(lv_event_t* e){
    static lv_area_t wind_pre_area;
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* ta = lv_event_get_target(e);
    lv_obj_t* keyboard = (lv_obj_t*)lv_event_get_user_data(e);
    /* 获得顶层父节点 */
    lv_obj_t* parent = ta, * current = ta;
    for (;parent != lv_layer_top();) {
        parent = lv_obj_get_parent(current);
        if (parent != lv_layer_top()) {
            current = parent;
        }
    }
    
    if (code == LV_EVENT_FOCUSED) {
        lv_keyboard_set_textarea(keyboard, ta);
        lv_obj_set_style_max_height(keyboard, LV_HOR_RES * 2 / 1, 0);
        lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
        lv_obj_scroll_to_view_recursive(ta, LV_ANIM_OFF);
        /* 移动构件 */
        lv_area_t text_area, kb_area;
        lv_obj_get_coords(ta, &text_area);
        lv_obj_get_coords(keyboard, &kb_area);
        int offset = text_area.y1 + 50 - kb_area.y1;
        /* 保存原始坐标 */
        lv_area_t wind_area;
        lv_obj_get_coords(current, &wind_area);
        wind_pre_area = wind_area;
        if (offset > 0) {
            uint32_t x = wind_area.x1;
            uint32_t y = wind_area.y1 - offset;
            lv_obj_move_to(current, x, y);
        }
    }
    else if (code == LV_EVENT_DEFOCUSED) {
        /* 恢复原始坐标 */
        lv_obj_move_to(current, wind_pre_area.x1, wind_pre_area.y1);
        lv_keyboard_set_textarea(keyboard, NULL);
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    }
    else if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
        lv_obj_move_to(current, wind_pre_area.x1, wind_pre_area.y1);
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
        lv_keyboard_set_textarea(keyboard, NULL);
        lv_obj_clear_state(ta, LV_STATE_FOCUSED);
    }
}

void ParamKeyboard::registerCallback(void* pObj, void* pFunc){
	CallbackList_.emplace(pObj, pFunc);
}


ParamKeyboard* ParamKeyboard::getInstance(){
    static ParamKeyboard keyboard;
    return &keyboard;
}

void ParamKeyboard::keyboard_event_cb(lv_event_t* e){
    static lv_area_t wind_pre_area;         
    UIParamPageABC* pPage = (UIParamPageABC*)lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);
	ParamKeyboard* keyboard = ParamKeyboard::getInstance();
    keyboard->_target = lv_event_get_target(e);
    /* 获得顶层父节点 */
    lv_obj_t* parent = keyboard->_target;
    keyboard->_parent_ant = keyboard->_target;
    for (;parent != lv_layer_top() && parent != lv_scr_act();) {
        parent = lv_obj_get_parent(keyboard->_parent_ant);
        if (parent != lv_layer_top() && parent != lv_scr_act()) {
            keyboard->_parent_ant = parent;
        }
    }
    
    if (code == LV_EVENT_FOCUSED) {
        keyboard->show();
    }
    else if(code == LV_EVENT_CANCEL){
        keyboard->hind();
        //lv_obj_clear_state(keyboard->_target, LV_STATE_FOCUSED);
    }
    else if (code == LV_EVENT_READY) {
        pPage->setValue((void*)keyboard->_target);
		keyboard->hind();
    }
}