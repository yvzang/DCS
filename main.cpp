#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <iostream>
#include <string>
#include <time.h>
#include "limlog.h"
#include "keyboard.h"
#include "lv_UITopPage.h"
#include "setting.h"
extern "C"{
    #include "G2dApi.h"
    #include "sunxiMemInterface.h"
    //#include "lv_demos/lv_demo.h"
    #include "lvgl/lvgl.h"
    #include "lv_drivers/display/fbdev.h"
    #include "lv_drivers/indev/evdev.h"
    #include "AWVideoDecoder.h"
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libavutil/avutil.h"
    #include "libswscale/swscale.h"
    #include "libavutil/imgutils.h"
    #include "demoVdecoder.h"
    #include "external/melsec_mc_net/melsec_mc_bin.h"
    #include "external/siemens_plc_s7_net/siemens_s7.h"
}


#define SCREEN_W            1024
#define SCREEN_H            768
#define DEFAULT_BUF_LEN   (500*1024)

#define DISP_BUF_SIZE (256 * 1024)

pthread_mutex_t lv_lock;
pthread_attr_t thread_attr;
int plc_fd;

extern TaskManager gTaskManager;

void* lv_my_widget(void* args){
    std::string videourl= "rtsp://admin:admin@192.168.1.10:554/h265/ch1/sub/av_stream";
    AVFormatContext *pFormatCtx = NULL, *ofmt_ctx;
    AVCodecContext* pCodecCtx = NULL;
    AVCodec* pCodec = NULL;
    AVDictionary *options = NULL;
    AVPacket *av_packet = NULL; // AVPacket暂存解码之前的媒体数据
    AVFrame* pFrameYUV = NULL;
    av_packet = (AVPacket *)av_malloc(sizeof(AVPacket));

    //initMyDecoder();

    avformat_network_init();
    //执行网络库的全局初始化。
    //此函数仅用于解决旧版GNUTLS或OpenSSL库的线程安全问题。
    //一旦删除对较旧的GNUTLS和OpenSSL库的支持，此函数将被弃用，并且此函数将不再有任何用途。
    av_dict_set(&options, "buffer_size", "4096000", 0); //设置缓存大小
    av_dict_set(&options, "rtsp_transport", "tcp", 0);  //以tcp的方式打开,
    av_dict_set(&options, "stimeout", "5000000", 0);    //设置超时断开链接时间，单位us,   5s
    av_dict_set(&options, "max_delay", "500000", 0);    //设置最大时延

    pFormatCtx = avformat_alloc_context(); //用来申请AVFormatContext类型变量并初始化默认参数,申请的空间

    //打开网络流或文件流
    int err_code;
    char buf[1024] = {0};
    if (err_code = avformat_open_input(&pFormatCtx, videourl.c_str(), NULL, &options))
    {
        av_strerror(err_code, buf, 1024);
        std::cout << "Couldn't open input stream.\n"
                  << buf << "\n"
                  << "\n";

        return NULL;
    }

    //获取视频文件信息
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
    {

        std::cout << "Couldn't find stream information." << std::endl;
        return NULL;
    }

    std::cout << "av_dict_get:" << std::endl;
    AVDictionaryEntry *tag = NULL;
    //av_dict_set(&pFormatCtx->metadata, "rotate", "0", 0);这里可以设置一些属性
    while ((tag = av_dict_get(pFormatCtx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX)))
    {
        std::string key = tag->key;
        std::string value = tag->value;
        std::cout << "av_dict_get:" << key << ":" << value << std::endl;
    }

    //查找码流中是否有视频流
    int videoindex = -1;
    AVStream* inVideoStream;
    unsigned i = 0;
    for (i = 0; i < pFormatCtx->nb_streams; i++)
    {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            inVideoStream = pFormatCtx->streams[i];
            videoindex = i;
            break;
        }
    }
    if (videoindex == -1)
    {
        std::cout << "Didn't find a video stream." << std::endl;;

        return NULL;
    }

    pCodec = (AVCodec*)avcodec_find_decoder(pFormatCtx->streams[videoindex]->codecpar->codec_id);
    if(pCodec == NULL){
        printf("codec not found.\n");
        return NULL;
    }
    std::cout << "使用解码器：" << pCodec->name << std::endl;
    //player_start();

    int ret;
    int yuvsize;
    char* temp;
    int decflag = 1;
    //mkfifo("/tmp/shared", 0666);
    //int fifofd = open("/tmp/shared", O_WRONLY|O_NONBLOCK);
    //FILE* fifofd = fopen("/tmp/shared", "wb");
    while(true)
    {
        if ((ret = av_read_frame(pFormatCtx, av_packet)) >= 0)
        {
            if (av_packet->stream_index == videoindex)
            {
                //decodeOneFrame(av_packet->data, av_packet->size);
                //fwrite(av_packet->data, 1, av_packet->size, fifofd);
                if(decflag){
                    decflag = 0;
                    //decoder_start();
                }
                //write(fifofd, av_packet->data, av_packet->size);
            }
            if (av_packet != NULL){
                av_packet_unref(av_packet);
            }
        }
    }
    av_free(av_packet);
    avformat_close_input(&pFormatCtx);
}

void registerPLCTask(){
    gTaskManager.registerReadDatablockTask("mc_plc",
                                        PLC_ADDRESS_UINT32,
                                        "D140", 10);
    gTaskManager.registerReadDatablockTask("mc_plc",
                                        PLC_ADDRESS_BOOL,
                                        "M194", 4);
    gTaskManager.registerReadDatablockTask("mc_plc",
                                        PLC_ADDRESS_UINT32,
                                        "D1000", 45);
    gTaskManager.registerReadDatablockTask("mc_plc",
                                        PLC_ADDRESS_BOOL,
                                        "M360", 24);
    gTaskManager.registerReadDatablockTask("mc_plc",
                                        PLC_ADDRESS_BOOL,
                                        "M3000", 51);
    // PLC heart
	auto task = std::make_shared<DeviceTask>();
	task->deviceID = CONFIG_PLC_DEVICE_ID;
	task->addrType = PLC_ADDRESS_UINT32;
	task->rw = PLC_ACCESS_WRITE;
	task->size = 1;
	task->address = "D160";
	task->payload = std::make_shared<DataPayload>();
	task->payload->duint32.push_back(1);
	task->PLCTaskSuccessCallback = std::bind([](){});
	task->PLCTaskFailedCallback = std::bind([](){});
	gTaskManager.registerPermenentTask(task);
}

int main(void)
{
    limlog::singleton()->setLogLevel(limlog::kDebug);
    pthread_mutex_init(&lv_lock, NULL);
    /*LittlevGL init*/
    lv_init();

    /*Linux frame buffer device init*/
    fbdev_init();

    /*A small buffer for LittlevGL to draw the screen's content*/
    static lv_color_t buf[DISP_BUF_SIZE];

    /*Initialize a descriptor for the buffer*/
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

    /*Initialize and register a display driver*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf   = &disp_buf;
    disp_drv.flush_cb   = fbdev_flush;
    disp_drv.hor_res    = 1024;
    disp_drv.ver_res    = 768;
    lv_disp_drv_register(&disp_drv);

    evdev_init();
    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;

    /*This function will be called periodically (by the library) to get the mouse position and state*/
    indev_drv_1.read_cb = evdev_read;
    lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);

    //lv_port_indev_init();

    /*Set a cursor for the mouse*/
    //LV_IMG_DECLARE(mouse_cursor_icon)
    //lv_obj_t * cursor_obj = lv_img_create(lv_scr_act()); /*Create an image object for the cursor */
    //lv_img_set_src(cursor_obj, &mouse_cursor_icon);           /*Set the image source*/
    //lv_indev_set_cursor(mouse_indev, cursor_obj);             /*Connect the image  object to the driver*/

    /*Create a Demo*/
    registerPLCTask();
    UITopPage topPage;
    //plc_connecter_init("192.168.2.90", 5551);

    EmergencyStopKey* pEStopKey = EmergencyStopKey::getInstance();

    int policy;
    sched_param main_sched_param;
    pthread_attr_init(&thread_attr);
    pthread_attr_setschedpolicy(&thread_attr, SCHED_FIFO);
    pthread_attr_getschedpolicy(&thread_attr, &policy);
    main_sched_param.sched_priority = sched_get_priority_max(policy);
    pthread_setschedparam(pthread_self(), policy, &main_sched_param);
    while(1) {
        if(pEStopKey->emergencyStopPressed()){
            auto task = std::make_shared<DeviceTask>();
            task->deviceID = CONFIG_PLC_DEVICE_ID;
            task->addrType = PLC_ADDRESS_BOOL;
            task->rw = PLC_ACCESS_WRITE;
            task->size = 1;
            task->address = "M3021";
            task->payload = std::make_shared<DataPayload>();
            task->payload->dbool.push_back(false);
            task->PLCTaskSuccessCallback = std::bind([=](){});
            task->PLCTaskFailedCallback = std::bind([=](){});
            gTaskManager.registerTempTask(task);
        }
        pthread_mutex_lock(&lv_lock);
        //pMainWind->mainWind_value_update();
        lv_timer_handler();
        pthread_mutex_unlock(&lv_lock);
        usleep(5000);
    }
    return 0;
}

/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}
