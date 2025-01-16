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
					cb.pReleasedCallback(cb.pData);
				}
			}
			else{
				if(cb.pPressedCallback){
					cb.pPressedCallback(cb.pData);
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
		_led_matrix.push_back(keyboard_led_row_t());
	}
	pthread_mutex_init(&_lock, NULL);
	//假读
	readlen = this->serial.readData(readstr, 10000);
	pthread_create(&listen_thread, NULL, physical_keyboard_listen_thread, this);
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
	pthread_mutex_lock(&_lock);
	if(_led_matrix[row][col] != on)
		_update_flag = true;
	_led_matrix[row][col] = on;
	pthread_mutex_unlock(&_lock);
}

void PhyKeyboard::keyboard_led_update(){
	uint8_t sendbuf[KEYBOARD_LED_ROW * 2]{0};
	if(_update_flag == false)return;
	pthread_mutex_lock(&_lock);
	for(int i = 0; i < _led_matrix.size(); i++){
		keyboard_led_row_t row = _led_matrix[i];
		uint8_t ch = (row[0] << 0) | (row[1] << 1) | (row[2] << 2) | (row[3] << 3);
		sendbuf[i * 2] = i;
		sendbuf[i * 2 + 1] = ch;
		printf("%2x %2x ", i, ch);
	}
	pthread_mutex_unlock(&_lock);
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