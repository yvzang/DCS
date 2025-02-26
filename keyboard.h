#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__
#include <pthread.h>
#include <functional>
#include <vector>
#include <memory>
#include <map>
#include <atomic>
#include "serial.h"

#define KEYBOARD_LED_ROW				6
#define KEYBOARD_LED_COL				4

typedef void (*pKeyboard_cb_t)(void *);
typedef std::function<void(void)> key_callback_func;

typedef struct{
	key_callback_func pPressedCallback;
	key_callback_func pReleasedCallback;
} keyboard_cb_t;

typedef struct{
	keyboard_cb_t key0_cb;
	keyboard_cb_t key1_cb;
	keyboard_cb_t key2_cb;
	keyboard_cb_t key3_cb;
	keyboard_cb_t key4_cb;
	keyboard_cb_t key5_cb;
	keyboard_cb_t key6_cb;
	keyboard_cb_t key7_cb;
	keyboard_cb_t key8_cb;
	keyboard_cb_t key9_cb;
	keyboard_cb_t key10_cb;
	keyboard_cb_t key11_cb;
	keyboard_cb_t key12_cb;
	keyboard_cb_t key13_cb;
	keyboard_cb_t key14_cb;
	keyboard_cb_t key15_cb;
	keyboard_cb_t key16_cb;
	keyboard_cb_t key17_cb;
	keyboard_cb_t key18_cb;
	keyboard_cb_t key19_cb;
	keyboard_cb_t key20_cb;
	keyboard_cb_t key21_cb;
	keyboard_cb_t key22_cb;
	keyboard_cb_t key23_cb;
} keyboard_opa_t;

class PhyKeyboard{
private:
	typedef std::vector<std::shared_ptr<std::atomic<bool>>> keyboard_led_row_t;
	typedef std::vector<keyboard_led_row_t> keyboard_led_matrix_t;

	unsigned int press_flag = 0;
	keyboard_opa_t _opa;
	static PhyKeyboard* pKeyboard;
	Serial serial;
	pthread_t listen_thread;
	keyboard_led_matrix_t _led_matrix;
	bool _update_flag;
public:
	static PhyKeyboard* getInstance(const keyboard_opa_t & opa);
	void keyboard_led_turn(uint8_t index, bool on);
	void keyboard_led_update();
private:
	PhyKeyboard() = delete;
	PhyKeyboard(const keyboard_opa_t & opa);
	keyboard_cb_t keyboard_callback(int index);
	~PhyKeyboard();
	friend void* physical_keyboard_listen_thread(void* args);
};

class VirtualKeyboard{
protected:
    lv_obj_t* _keyboard;
    lv_obj_t* _target;
    lv_obj_t* _parent_ant;
    lv_area_t wind_pre_area;
    VirtualKeyboard();
    ~VirtualKeyboard();
public:
    void show();
    void hind();
    static void keyboard_event_cb(lv_event_t* e);
    static VirtualKeyboard* getInstance();
};

class ParamKeyboard: public VirtualKeyboard{
protected:
    ParamKeyboard(){};
    ~ParamKeyboard(){};
public:
	typedef std::function<void()> KeyboardEnterCallback;

	void registerCallback(void* pObject, void* pFunc);

    static ParamKeyboard* getInstance();
    static void keyboard_event_cb(lv_event_t* e);

private:
	std::map<void*, void*> CallbackList_;
};

#endif