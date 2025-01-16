#ifndef __DEMOVDECODER_H__
#define __DEMOVDECODER_H__
#include "lvgl/lvgl.h"

#define DEMO_PARSER_ERROR (1 << 0)
#define DEMO_DECODER_ERROR (1 << 1)
#define DEMO_DISPLAY_ERROR (1 << 2)
#define DEMO_DECODE_FINISH (1 << 3)
#define DEMO_PARSER_EXIT   (1 << 5)
#define DEMO_DECODER_EXIT  (1 << 6)
#define DEMO_DISPLAY_EXIT  (1 << 7)
#define DEMO_ERROR    (DEMO_PARSER_ERROR | DEMO_DECODER_ERROR | DEMO_DISPLAY_ERROR)
#define DEMO_EXIT    (DEMO_ERROR | DEMO_DECODE_FINISH)


typedef struct{
    char filename[64];
    bool prepare;
}vbuffer_t;


#if 0
int initMyDecoder();
int decodeOneFrame(const uint8_t* inputData, size_t inputSize);
int player_start();
#endif

void decoder_start(void** ppDecoder, const char* inputpath, lv_obj_t* img_domain, 
                    lv_img_dsc_t* IMG_ptr, int* state);
    
void decoder_stop(void* pDecoder, int* state);

#endif