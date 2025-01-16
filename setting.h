#ifndef __SETTING_H__
#define __SETTING_H__
#include <map>
#include <string>


#define KEYXPRESS_CB(x)					void key##x##Press_cb(void* pWind){				\
    										MainWind* _pWind = (MainWind*)pWind;		\
											auto item_ptr = KEY_PLC_REG_MAP.find(x);	\
											if(item_ptr == KEY_PLC_REG_MAP.end())		\
												return;									\
											printf("key "#x" pressed.\n");				\
											pthread_mutex_lock(&_pWind->_pPLCManager->rw_lock);		\
    										_pWind->_pPLCManager->write_bool(item_ptr->second, true);\
											pthread_mutex_unlock(&_pWind->_pPLCManager->rw_lock);		\
										};
#define KEYXRELEASE_CB(x)				void key##x##Release_cb(void* pWind){			\
    										MainWind* _pWind = (MainWind*)pWind;		\
											auto item_ptr = KEY_PLC_REG_MAP.find(x);	\
											if(item_ptr == KEY_PLC_REG_MAP.end())		\
												return;									\
											printf("key "#x" released.\n");				\
											pthread_mutex_lock(&_pWind->_pPLCManager->rw_lock);		\
    										_pWind->_pPLCManager->write_bool(item_ptr->second, false);\
											pthread_mutex_unlock(&_pWind->_pPLCManager->rw_lock);		\
										};
#define KEYXCALLBACK(x)					keyboard_cb_t key##x##_cb = {					\
            								.pPressedCallback = key##x##Press_cb,		\
            								.pReleasedCallback = key##x##Release_cb,	\
            								.pData = this								\
        								};

const std::map<std::string, std::string> FAULT_MESSAGE = {
	{"M360", "1#行走电机故障"},
	{"m361", "2#新走电机故障"},
	{"M362", "1#上线电机故障"},
	{"M363", "2#上线电机故障"},
	{"M364", "顶漆电机故障"},
	{"M365", "喷头电机故障"},
	{"M366", "喷头未闭合，无法自动启动"},
	{"M367", "设备不处于闭合状态，无法自动启动"},
	{"M368", "工作模式未选择，无法自动启动"},
	{"M369", ""},
	{"M370", "设备处于急停状态中"},
	{"M371", "设备电量低，请注意"},
	{"M372", "设备角度倾斜过大，请注意"},
	{"M373", ""},
	{"M374", ""},
	{"M375", ""},
	{"M376", ""},
	{"M377", ""},
	{"M380", "上升左限位"},
	{"M381", "上升右限位"},
	{"M382", "顶漆上限位"},
	{"M383", "顶漆下限位"}
};

const std::vector<std::string> FAULT_RECORD_REG = {
	"M360", "M361", "M362",
	"M363", "M364", "M365",
	"M366", "M367", "M368"
};

const std::map<int, std::string> KEY_PLC_REG_MAP = {
	{0, "M3000"},
	{1, "M3001"},
	{2, "M3002"},
	{3, "M3003"},
	{4, "M3004"},
	{5, "M3005"},
	{6, "M3006"},
	{7, "M3007"},
	{8, "M3008"},
	{9, "M3009"},
	{10, "M3010"},
	{11, "M3011"},
	{12, "M3012"},
	{13, "M3013"},
	{14, "M3014"},
	{15, "M3015"},
	{16, "M3016"},
	{17, "M3017"},
	{18, "M3018"},
	{19, "M3019"},
	{20, "M3020"}
};

const std::map<int, std::string> KEY_LED_PLC_REG_MAP = {
	{0, "M3030"},
	{1, "M3031"},
	{2, "M3032"},
	{3, "M3033"},
	{4, "M3034"},
	{5, "M3035"},
	{6, "M3036"},
	{7, "M3037"},
	{8, "M3038"},
	{9, "M3039"},
	{10, "M3040"},
	{11, "M3041"},
	{12, "M3042"},
	{13, "M3043"},
	{14, "M3044"},
	{15, "M3045"},
	{16, "M3046"},
	{17, "M3047"},
	{18, "M3048"},
	{19, "M3049"},
	{20, "M3050"}
};

#endif