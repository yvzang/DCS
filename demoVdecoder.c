/*
 * Copyright (c) 2008-2016 Allwinner Technology Co. Ltd.
 * All rights reserved.
 *
 * File : demoVdecoder.c
 * Description : demoVdecoder
 * History :
 *
 */

#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <openssl/sha.h>
#include <iniparserapi.h>
#include "demoVdecoder.h"
#include "g2d_driver.h"
#include <pthread.h>
#include "cdx_config.h"
#include <cdx_log.h>
#include "CdxParser.h"
#include "vdecoder.h"
#include "sunxiMemInterface.h"
#include "memoryAdapter.h"

#define DEBUG_TIME_INFO 0
#define DEBUG_COST_DRAM_ENABLE 0

#define DRAM_COST_MAX_THREAD_NUM 8
#define FRAME_COUNT 64
#define DISPLAY_PICTUER_LIST_NUM 5
#define DISPLAY_HOLDING_BUFFERS 2
#define DISPLAY_HOLDING_NUM 0
#define DISPLAY_COST_TIME 2

#define DEMO_PARSER_MAX_STREAM_NUM 1024
#define DEMO_FILE_NAME_LEN (2*1024)


#define DECODER_MAX_NUM (5)
#define DEFAULT_BUF_LEN   (1024*1024)

#define INPUTPATH2 = "rtsp://admin:@192.168.1.10:554/h265/ch1/sub/av_stream"
#define INPUTPATH1 = "rtsp://admin:@192.168.1.11:554/h265/ch1/sub/av_stream"

LV_IMG_DECLARE(no_camera_200x200);

/*typedef struct CdxPacketS {
    void *buf;
    void *ringBuf;
    int buflen;
    int ringBufLen;
    int long pts;
    int long duration;
    int type;
    int length;
    unsigned int flags;
    int streamIndex;
    int long pcr;
    int infoVersion;
    void *info;
} CdxPacketT;*/

typedef struct _MultiThreadCtx
{
    pthread_rwlock_t rwrock;
    int nEndofStream;
    int loop;
    int* state; 
}MultiThreadCtx;

typedef struct _DecDemo
{
    VideoDecoder *pVideoDec;
    CdxParserT *parser;
    CdxDataSourceT source;
    CdxMediaInfoT mediaInfo;
    MultiThreadCtx thread;
    long long totalTime;
    long long DurationTime;
    int  nDispFrameCount;
    int  nFinishNum;
    int  nDramCostThreadNum;
    int  nDecodeFrameCount;
    char *pInputFile;
    char *pSavePicPath;
    int  nSavePicCount;
    char *pSaveShaFile;
    FILE *pSaveShaFp;
    char *pCompareShaFile;
    FILE *pCompareShaFp;
    int   nCompareShaErrorCount;
    pthread_mutex_t parserMutex;
    /* start to save yuv picture,
     * when decoded picture order >=  nSavePictureStartNumber*/
    int  nSavePictureStartNumber;
    /* the saved picture number */
    int  nSavePictureNumber;
    struct ScMemOpsS* memops;
    int nVeFreq;
    int nChannel;
    int argc;
    int iPicW;
    int iPicH;
    int oPicW;
    int oPicH;
    int index;
    char** argv;
    lv_img_dsc_t* IMG_ptr;
    lv_obj_t* img_domain;
    pthread_mutex_t img_lock;
}DecDemo;

VideoDecoder* pVideoDec = NULL;
VideoStreamInfo mVideoInfo;
VConfig VideoConf;
struct ScMemOpsS* mScMemOpsS = NULL;
paramStruct_t mSetparmMemops;
SHA_CTX shaCtx;
int g2d_fd;
extern pthread_mutex_t lv_lock;
extern pthread_attr_t thread_attr;
LV_IMG_DECLARE(no_camera_200x200);


typedef struct display
{
    VideoPicture* picture;
    int flag;
    struct display *next;
}display;

typedef enum {
    INPUT,
    HELP,
    DECODE_FRAME_NUM,
    SAVE_FRAME_START,
    SAVE_FRAME_NUM,
    SAVE_FRAME_PATH,
    COST_DRAM_THREAD_NUM,
    SAVE_SHA_FILE,
    SETUP_VE_FREQ,
    COMPARE_SHA,
    DECODER_NUM,
    INVALID
}ARGUMENT_T;

typedef struct {
    char Short[16];
    char Name[128];
    ARGUMENT_T argument;
    char Description[512];
}argument_t;

static const argument_t ArgumentMapping[] =
{
    { "-h",  "--help",    HELP,
        "Print this help" },
    { "-i",  "--input",   INPUT,
        "Input file" },
    { "-n",  "--decode_frame_num",   DECODE_FRAME_NUM,
        "After display n frames, decoder stop" },
    { "-ss",  "--save_frame_start",  SAVE_FRAME_START,
        "After display ss frames, saving pictures begin" },
    { "-sn",  "--save_frame_num",  SAVE_FRAME_NUM,
        "After sn frames saved, stop saving picture" },
    { "-o",  "--save_pic_path", SAVE_FRAME_PATH,
        "saving picture path" },
    { "-cn",  "--cost_dram_thread_num",  COST_DRAM_THREAD_NUM,
        "create cn threads to cost dram, or disturb cpu, test decoder robust" },
    { "-sha", "--save_sha",SAVE_SHA_FILE,
        "save sha file"},
    { "-vefreq", "--setup_ve_freq",SETUP_VE_FREQ,
        "setup ve freq"},
    { "-cmpsha", "--compare_sha",COMPARE_SHA,
        "compare sha value"},
    { "-decNum", "--decoder_num",DECODER_NUM,
        "decoder number"},
};


static void PrintDemoUsage(void)
{
    int i = 0;
    int num = sizeof(ArgumentMapping) / sizeof(argument_t);
    logd("Usage:");
    while(i < num)
    {
        logd("%s %-32s  %s", ArgumentMapping[i].Short, ArgumentMapping[i].Name,
                ArgumentMapping[i].Description);
        i++;
    }
}

ARGUMENT_T GetArgument(char *name)
{
    int i = 0;
    int num = sizeof(ArgumentMapping) / sizeof(argument_t);
    while(i < num)
    {
        if((0 == strcmp(ArgumentMapping[i].Name, name)) ||
            ((0 == strcmp(ArgumentMapping[i].Short, name)) &&
             (0 != strcmp(ArgumentMapping[i].Short, "--"))))
        {
            return ArgumentMapping[i].argument;
        }
        i++;
    }
    return INVALID;
}

void ParseArgument(DecDemo *Decoder, char *argument, char *value)
{
    ARGUMENT_T arg;
//    int len = strlen(value);
    int len = 0;
    if(len > DEMO_FILE_NAME_LEN)
        return;
    arg = GetArgument(argument);
    switch(arg)
    {
        case HELP:
            PrintDemoUsage();
            exit(-1);
        case INPUT:
            sprintf(Decoder->pInputFile, "file://");
            sscanf(value, "%2048s", Decoder->pInputFile + 7);
            logd(" get input file: %s ", Decoder->pInputFile);
            break;
        case DECODE_FRAME_NUM:
            sscanf(value, "%d", &Decoder->nFinishNum);
            break;
        case SAVE_FRAME_START:
            sscanf(value, "%d", &Decoder->nSavePictureStartNumber);
            break;
        case SAVE_FRAME_NUM:
            sscanf(value, "%d", &Decoder->nSavePictureNumber);
            break;
        case SAVE_FRAME_PATH:
            {
            Decoder->pSavePicPath = calloc(DEMO_FILE_NAME_LEN, 1);
            if(Decoder->pSavePicPath != NULL)
            {
                sscanf(value, "%128s", Decoder->pSavePicPath);
                logd(" get save pic path: %s ", Decoder->pSavePicPath);
            }
            else
                loge(" output path. calloc memory fail. ");
            break;
            }
        case COST_DRAM_THREAD_NUM:
            sscanf(value, "%d", &Decoder->nDramCostThreadNum);
            break;
        case SAVE_SHA_FILE:
            logd("log value %s",value);
            sscanf(value, "%2048s", Decoder->pSaveShaFile);
            logd(" get sha file: %s ", Decoder->pSaveShaFile);
            break;
        case SETUP_VE_FREQ:
            sscanf(value, "%d", &Decoder->nVeFreq);
            logd(" setup ve freq: %d ", Decoder->nVeFreq);
            break;
        case COMPARE_SHA:
            logd("log value %s",value);
            sscanf(value, "%2048s", Decoder->pCompareShaFile);
            logd(" get compare sha file: %s ", Decoder->pCompareShaFile);

            sprintf(Decoder->pSaveShaFile,"%s_cur",Decoder->pCompareShaFile);
            logd(" get current sha file: %s ", Decoder->pSaveShaFile);
            break;
        case INVALID:
        default:
            logd("unknowed argument :  %s", argument);
            break;
    }
}

static long long GetNowUs(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000000ll + tv.tv_usec;
}

int initDecoder(DecDemo *Decoder)
{
    int nRet, i;
    int bForceExit = 0;
    VConfig             VideoConf;
    VideoStreamInfo     VideoInfo;
    struct CdxProgramS *program;
    CdxStreamT *stream = NULL;

    memset(&VideoConf, 0, sizeof(VConfig));
    memset(&VideoInfo, 0, sizeof(VideoStreamInfo));
    memset(&Decoder->mediaInfo, 0, sizeof(CdxMediaInfoT));
    memset(&Decoder->source, 0, sizeof(CdxDataSourceT));
    Decoder->memops = MemAdapterGetOpsS();
    pthread_mutex_init(&Decoder->img_lock, NULL);
    g2d_fd = open("/dev/g2d", O_RDWR);
    if(Decoder->memops == NULL)
    {
        loge("memops is NULL");
        return -1;
    }
    CdcMemOpen(Decoder->memops);
    logv(" before strcpy(tmpUrl, url) ");
    Decoder->source.uri = Decoder->pInputFile;
    logv(" before CdxParserPrepare() %s", Decoder->source.uri);
    pthread_mutex_init(&Decoder->parserMutex, NULL);
    nRet = CdxParserPrepare(&Decoder->source, 0, &Decoder->parserMutex,
            &bForceExit, &Decoder->parser, &stream, NULL, NULL);
    if(nRet < 0 || Decoder->parser == NULL)
    {
        loge(" decoder open parser error nRet = %d, Decoder->parser: %p", nRet, Decoder->parser);
        return -1;
    }
    logv(" before CdxParserGetMediaInfo() ");
    nRet = CdxParserGetMediaInfo(Decoder->parser, &Decoder->mediaInfo);
    if(nRet != 0)
    {
        loge(" decoder parser get media info error ");
        return -1;
    }
    logv(" before CreateVideoDecoder() ");
    Decoder->pVideoDec = CreateVideoDecoder();
    if(Decoder->pVideoDec == NULL)
    {
        loge(" decoder demom CreateVideoDecoder() error ");
        return -1;
    }
    logv(" before InitializeVideoDecoder() ");
    program = &(Decoder->mediaInfo.program[Decoder->mediaInfo.programIndex]);
    for (i = 0; i < 1; i++)
    {
        VideoStreamInfo *vp = &VideoInfo;
        vp->eCodecFormat = program->video[i].eCodecFormat;
        vp->nWidth = program->video[i].nWidth;
        vp->nHeight = program->video[i].nHeight;
        vp->nFrameRate = program->video[i].nFrameRate;
        vp->nFrameDuration = program->video[i].nFrameDuration;
        vp->nAspectRatio = program->video[i].nAspectRatio;
        vp->bIs3DStream = program->video[i].bIs3DStream;
        vp->nCodecSpecificDataLen = program->video[i].nCodecSpecificDataLen;
        vp->pCodecSpecificData = program->video[i].pCodecSpecificData;
    }
    VideoConf.eOutputPixelFormat  = PIXEL_FORMAT_YUV_PLANER_420;

    VideoConf.nDeInterlaceHoldingFrameBufferNum = GetConfigParamterInt("pic_4di_num", 2);
    VideoConf.nDisplayHoldingFrameBufferNum = GetConfigParamterInt("pic_4list_num", 3);
    VideoConf.nRotateHoldingFrameBufferNum = GetConfigParamterInt("pic_4rotate_num", 0);
    VideoConf.nDecodeSmoothFrameBufferNum = GetConfigParamterInt("pic_4smooth_num", 3);
    VideoConf.memops = Decoder->memops;
    VideoConf.nVeFreq = Decoder->nVeFreq;

    logd("**** set the ve freq = %d",VideoConf.nVeFreq);

    nRet = InitializeVideoDecoder(Decoder->pVideoDec, &VideoInfo, &VideoConf);
    logv(" after InitializeVideoDecoder() ");
    if(nRet != 0)
    {
        loge("decoder demom initialize video decoder fail.");
        DestroyVideoDecoder(Decoder->pVideoDec);
        Decoder->pVideoDec = NULL;
    }

    pthread_rwlock_init(&Decoder->thread.rwrock, NULL);

    logd(" initDecoder OK ");
    return 0;
}

static char copyPicture(VideoPicture* pPicture)
{
#if 1
    CDX_UNUSE(pPicture);
    return 0;
#else
    int nWidth, nHeight;
    int i, j;
    char *pDst, *pSrc, ret;
    ret = 0;
    nHeight = pPicture->nHeight;
    nWidth = pPicture->nWidth;
    pDst = malloc(nWidth * nHeight);
    if(pDst == NULL)
        return ret;
    pSrc = pPicture->pData0;
    for(j = 0; j < nHeight; j++)
        for( i = 0; i < nWidth; i++)
        {
            pDst[j + nHeight + i] = pSrc[j + nHeight + i];
            pDst[i] = pSrc[j + nHeight + i] - pSrc[i];
        }

    pSrc = pPicture->pData1;
    nHeight = pPicture->nHeight / 2;
    nWidth = pPicture->nWidth / 2;
    for(j = 0; j < nHeight; j++)
        for( i = 0; i < nWidth; i++)
        {
            pDst[j + nHeight + i] = pSrc[j + nHeight + i];
            pDst[i] = pSrc[j + nHeight + i] - pSrc[i];
        }

    pSrc = pPicture->pData2;
    nHeight = pPicture->nHeight / 2;
    nWidth = pPicture->nWidth / 2;
    for(j = 0; j < nHeight; j++)
        for( i = 0; i < nWidth; i++)
        {
            pDst[j + nHeight + i] = pSrc[j + nHeight + i];
            pDst[i] = pSrc[j + nHeight + i] - pSrc[i];
        }
    ret = pDst[i];
    free(pDst);
    return ret;
#endif
}

int allocPicMem(paramStruct_t*pops,int size)
{
    int iRet = 0;

    iRet = allocOpen(MEM_TYPE_CDX_NEW, pops, NULL);
    if (iRet < 0) {
        printf("ion_alloc_open failed\n");
        return iRet;
    }
    pops->size =size;
    iRet = allocAlloc(MEM_TYPE_CDX_NEW, pops, NULL);
    if(iRet < 0) {
        printf("allocAlloc failed\n");
        return iRet;
    }

    return 0;
}

int freePicMem(paramStruct_t*pops)
{
    int iRet = 0;

	allocFree(MEM_TYPE_CDX_NEW, pops, NULL);

    return 0;
}

void g2d_yuv2rgb(int g2d_fd, VideoPicture* iPicture, 
                paramStruct_t* m_iPicture, paramStruct_t* m_oPicture, 
                int dstW, int dstH){
    int src_width = iPicture->nWidth;
    int src_height = iPicture->nHeight;
    int src_format = (int)G2D_FORMAT_YUV420_PLANAR;
    int src_size = src_width * src_height * 3/2;

    /* output param */
    int dst_width = dstW;
    int dst_height = dstH;
    int dst_format = (int)G2D_FORMAT_ARGB8888;
    int dst_size = dst_width * dst_height * 4;
    /* commond flag */
    int flag = (int)G2D_ROT_0;

    memset((void*)m_oPicture->vir, 0, dst_size);
    memcpy((void*)m_iPicture->vir, iPicture->pData0, src_size);
    flushCache(MEM_TYPE_CDX_NEW, m_iPicture, NULL);
    int src_fd = m_iPicture->ion_buffer.fd_data.aw_fd;
    int dst_fd = m_oPicture->ion_buffer.fd_data.aw_fd;

    g2d_blt_h blt;
    memset(&blt, 0, sizeof(blt));

    blt.flag_h = G2D_BLT_NONE_H;
    blt.src_image_h.format = (g2d_fmt_enh)src_format;
    blt.src_image_h.width = src_width;
    blt.src_image_h.height = src_height;
    blt.src_image_h.align[0] = 0;
    blt.src_image_h.align[1] = 0;
    blt.src_image_h.clip_rect.x = 0;
    blt.src_image_h.clip_rect.y = 0;
    blt.src_image_h.clip_rect.w = src_width;
    blt.src_image_h.clip_rect.h = src_height;
    blt.src_image_h.gamut = G2D_BT2020;
    blt.src_image_h.bpremul = 0;
    blt.src_image_h.mode = G2D_PIXEL_ALPHA;
    blt.src_image_h.use_phy_addr = 0;
    blt.src_image_h.fd = src_fd;

    blt.dst_image_h.format = (g2d_fmt_enh)dst_format;
    blt.dst_image_h.width = dst_width;
    blt.dst_image_h.height = dst_height;
    blt.dst_image_h.align[0] = 0;
    blt.dst_image_h.align[1] = 0;
    blt.dst_image_h.clip_rect.x = 0;
    blt.dst_image_h.clip_rect.y = 0;
    blt.dst_image_h.clip_rect.w = dst_width;
    blt.dst_image_h.clip_rect.h = dst_height;
    blt.dst_image_h.gamut = G2D_BT2020;
    blt.dst_image_h.bpremul = 0;
    blt.dst_image_h.mode = G2D_PIXEL_ALPHA;
    blt.dst_image_h.use_phy_addr = 0;
    blt.dst_image_h.fd = dst_fd;

    ioctl(g2d_fd, G2D_CMD_BITBLT_H, &blt);
    flushCache(MEM_TYPE_CDX_NEW, m_oPicture, NULL);
}

static void addPictureToList(VideoDecoder *pVideoDec, pthread_mutex_t* img_lock,
        lv_obj_t* img_domain, lv_img_dsc_t* img, 
        display *pDisList, display **h, display **r, VideoPicture* pPicture,
        paramStruct_t* m_iPicture, paramStruct_t* m_oPicture,
        int oPicW, int oPicH)
{
    int i;
    display *node = NULL;
    display *DisHeader = *h;
    display *DisRear = *r;
    if(pPicture == NULL || pDisList == NULL)
    {
        loge(" add picuter to list error");
        return;
    }
    for(i = 0; i < DISPLAY_PICTUER_LIST_NUM; i++)
    {
        if(pDisList[i].flag == 0)
        {
            node = &pDisList[i];
            node->flag = 1;
            node->picture = pPicture;
            break;
        }
    }
    if(DisHeader == NULL && DisRear == NULL)
    {
        DisHeader = DisRear = node;
    }
    else
    {
        DisRear->next = node;
        node->next = NULL;
        DisRear = node;
    }

    i = 1;
    node = DisHeader;
    while(node != NULL && node->next != NULL)
    {
        i += 1;
        node = node->next;
    }
    if(i >= DISPLAY_HOLDING_BUFFERS)
    {
        node = DisHeader;
        DisHeader = DisHeader->next;
        node->next = NULL;
        node->flag = 0;
        //copyPicture(node->picture);
        g2d_yuv2rgb(g2d_fd, node->picture, m_iPicture, m_oPicture, oPicW, oPicH);
        flushCache(MEM_TYPE_CDX_NEW, m_oPicture, NULL);
        img->data_size = m_oPicture->size;
        img->data = (uint8_t*)m_oPicture->vir;
        pthread_mutex_lock(img_lock);
        lv_img_set_src(img_domain, img);
        pthread_mutex_unlock(img_lock);
        usleep(9000);
        ReturnPicture(pVideoDec, node->picture);
    }
    *h = DisHeader;
    *r = DisRear;
}

static int ReturnAllPicture(VideoDecoder *pVideoDec, display *DisHeader)
{
    int num = 0;
    while(DisHeader)
    {
        ReturnPicture(pVideoDec, DisHeader->picture);
        DisHeader->flag = 0;
        DisHeader = DisHeader->next;
        num += 1;
    }
    return num;
}

void* DecodeThread(void* param)
{
    DecDemo  *pVideoDecoder;
    VideoDecoder *pVideoDec;
    int nRet, nStreamNum, i, state;
    int nEndOfStream;

    pVideoDecoder = (DecDemo *)param;
    nEndOfStream = 0;

    pVideoDec = pVideoDecoder->pVideoDec;
    logv(" DecodeThread(), thread created ");

    i = 0;
    nStreamNum = VideoStreamFrameNum(pVideoDec, 0);
    while(nStreamNum < 200)
    {
        usleep(2*1000);
        i++;
        if(i > 100)
            break;
        nStreamNum = VideoStreamFrameNum(pVideoDec, 0);
    }
    logv(" data trunk number: %d, i = %d ", nStreamNum, i);

    while(1)
    {
        /* step 1: get stream data */
        usleep(50);
        pthread_rwlock_wrlock(&pVideoDecoder->thread.rwrock);
        nEndOfStream = pVideoDecoder->thread.nEndofStream;
        state = *pVideoDecoder->thread.state;
        pthread_rwlock_unlock(&pVideoDecoder->thread.rwrock);
        if(state & DEMO_EXIT)
        {
            if(state & DEMO_ERROR)
            {
                loge(" decoer thread recieve an error singnal,  exit..... ");
            }
            if(state & DEMO_DECODE_FINISH)
            {
                logd(" decoer thread recieve a finish singnal,  exit.....  ");
            }
            break;
        }

        logv(" DecodeThread(), DecodeVideoStream() start .... ");
        nRet = DecodeVideoStream(pVideoDec, nEndOfStream /*eos*/,
                0/*key frame only*/, 0/*drop b frame*/,
                0/*current time*/);
//        logd(" ------- decoderThread. one frame cost time: %lld ", deltaTime);

        if(nEndOfStream == 1 && nRet == VDECODE_RESULT_NO_BITSTREAM)
        {
            logd(" decoer thread finish decoding.  exit..... ");
            break;
        }
        if(nRet == VDECODE_RESULT_KEYFRAME_DECODED ||
                nRet == VDECODE_RESULT_FRAME_DECODED)
            pVideoDecoder->nDecodeFrameCount++;

        if(nRet < 0)
        {
            loge(" decoder return error. decoder exit ");
            pthread_rwlock_wrlock(&pVideoDecoder->thread.rwrock);
            *pVideoDecoder->thread.state |= DEMO_DECODER_ERROR;
            pthread_rwlock_unlock(&pVideoDecoder->thread.rwrock);
            break;
        }
    }

    pthread_rwlock_wrlock(&pVideoDecoder->thread.rwrock);
    *pVideoDecoder->thread.state |= DEMO_DECODER_EXIT;
    pthread_rwlock_unlock(&pVideoDecoder->thread.rwrock);

    logd(" decoder thread exit.... ");
    pthread_exit(NULL);
    return 0;
}

static void savePicture(VideoPicture* pPicture, DecDemo  *pVideoDecoder)
{
    if(pPicture->pData0 == NULL)
    {
        logw("save picture failed: pPicture->pData0 is null");
        return ;
    }
    typedef struct PixelFormatMapping
    {
        unsigned char name[10];
        int nIndex ;
    } PixelFormatMapping;
    int i=0;
    char name[128];
    FILE *fp;
    int nMappSize;

    PixelFormatMapping mPixelFormatMapping[] =
        {{"non", 0},
        {"yuv420", 1},
        {"yuv422", 2},
        {"yuv444", 3},
        {"yv12", 4},
        {"nv21", 5},
        {"nv12", 6},
        {"mb32", 7}
    };

    nMappSize = sizeof(mPixelFormatMapping)/sizeof(PixelFormatMapping);
    for(; i < nMappSize; i++)
    {
        if(mPixelFormatMapping[i].nIndex == pPicture->ePixelFormat)
            break;
    }
    if(i >= nMappSize)
        i = 0;

    if(pVideoDecoder->pSavePicPath == NULL)
    {
        sprintf(name, "/data/camera/pic_%s_%dx%d_%d.dat",mPixelFormatMapping[i].name,
            pPicture->nWidth, pPicture->nHeight,pVideoDecoder->nSavePicCount);
    }
    else
    {
        sprintf(name, "%spic_%s_%dx%d_%d.dat",
            pVideoDecoder->pSavePicPath, mPixelFormatMapping[i].name,
            pPicture->nWidth, pPicture->nHeight,pVideoDecoder->nSavePicCount);
    }

    fp = fopen(name, "ab");
    if(fp != NULL)
    {
        logd("saving picture, size: %d x %d, format: %d, count: %d",
            pPicture->nWidth, pPicture->nHeight, pPicture->ePixelFormat,
            pVideoDecoder->nSavePicCount);

        int nSaveLen;
        if(pPicture->bEnableAfbcFlag)
            nSaveLen = pPicture->nAfbcSize;
        else
            nSaveLen = pPicture->nWidth * pPicture->nHeight * 3/2;

        CdcMemFlushCache(pVideoDecoder->memops,pPicture->pData0, nSaveLen);
        fwrite(pPicture->pData0, 1, nSaveLen, fp);
        fclose(fp);
    }
    else
    {
        loge("saving picture failed: open file error, frame number = %d", pVideoDecoder->nSavePicCount);
    }
}

void saveVideoPictureInfo(VideoPicture *p,DecDemo  *pVideoDecoder)
{
    int bNeedCompSha = 0;
    char shaInfo[256] = {0};
    char CmpSha[256] = {0};
    char* pShaInfo = shaInfo;
    int   nShaInfoLen = 0;
    if(pVideoDecoder->pCompareShaFp != NULL)
    {
        bNeedCompSha = 1;
        fgets(CmpSha, 256, pVideoDecoder->pCompareShaFp);
    }
    FILE *fp = pVideoDecoder->pSaveShaFp;
    #if 0
    fprintf(fp,
            "nStreamIndex:%d,ePixelFormat:%d,nWidth:%d,nHeight:%d,nLineStride;:%d,"
            "nTopOffset:%d,nLeftOffset: %d,nBottomOffset:%d,nRightOffset:%d,"
            "nFrameRate:%d,nAspectRatio:%d,bIsProgressive:%d,bTopFieldFirst:%d,"
            "bRepeatTopField:%d,nPts:%lld,nPcr:%lld,nMafFlagStride:%d,bPreFrmValid:%d\n",
            p->nStreamIndex, p->ePixelFormat, p->nWidth, p->nHeight,
            p->nLineStride, p->nTopOffset, p->nLeftOffset, p->nBottomOffset,
            p->nRightOffset, p->nFrameRate, p->nAspectRatio, p->bIsProgressive,
            p->bTopFieldFirst, p->bRepeatTopField, (long long int)p->nPts,
            (long long int)p->nPcr,p->nMafFlagStride, p->bPreFrmValid);
    #endif

    nShaInfoLen = sprintf(pShaInfo,"pts:%lld",(long long int)p->nPts);

    int nBufLen;
    if(p->bEnableAfbcFlag)
         nBufLen = p->nAfbcSize;
    else
         nBufLen = p->nWidth * p->nHeight * 3/2;

    CdcMemFlushCache(pVideoDecoder->memops,p->pData0, nBufLen);

    if(p->pData1 == NULL)
       p->pData1 = p->pData0 + p->nWidth*p->nHeight;

    if(p->pData2 == NULL)
       p->pData2 = p->pData0 + p->nWidth*p->nHeight*5/4;

    SHA1_Init(&shaCtx);
    unsigned char sha[20]= {0};
    uint8_t *pY, *pU, *pV;
    int width, height, i, j;
    int copyHeight, copyWidth;
    int srcWidth;
    unsigned char* srcPtr;
    pY = (unsigned char *) p->pData0;
    pU = (unsigned char *) p->pData1;
    pV = (unsigned char *) p->pData2;
    width = p->nWidth;
    height = p->nHeight;

    copyHeight = p->nBottomOffset - p->nTopOffset;
    copyWidth  = p->nRightOffset - p->nLeftOffset;

    if(copyHeight==height && copyWidth==width)
    {
        srcPtr = (unsigned char*)p->pData0 ;
        srcWidth = (p->nWidth + 15) & ~15;
        // copy y
        for(i=0; i<copyHeight; i++)
        {
            SHA1_Update(&shaCtx, srcPtr, copyWidth);
            srcPtr += srcWidth;
        }

        srcPtr =((unsigned char*)p->pData1);
        for(i=0; i<copyHeight/2 ; i++)
        {
            SHA1_Update(&shaCtx, srcPtr, copyWidth/2);
            srcPtr += srcWidth/2;
        }

        srcPtr =((unsigned char*)p->pData2) ;
        for(i=0; i<copyHeight/2 ; i++)
        {
            SHA1_Update(&shaCtx, srcPtr, copyWidth/2);
            srcPtr += srcWidth/2;
        }
    }
    else
    {
        // for h265
        int topOffset    = p->nTopOffset ;
        int leftOffset   = p->nLeftOffset ;
        int bottomOffset = p->nBottomOffset;
        int rightOffset  = p->nRightOffset ;
        copyHeight       = bottomOffset - topOffset;
        copyWidth        = rightOffset - leftOffset;
        srcPtr = (unsigned char*)p->pData0 + (p->nWidth * topOffset + leftOffset);
        srcWidth = width;

        // copy y
        for(i=0; i<copyHeight; i++)
        {
            SHA1_Update(&shaCtx, srcPtr, copyWidth);
            srcPtr += srcWidth;
        }

        srcPtr =((unsigned char*)p->pData1) + \
                p->nWidth/2 * topOffset / 2 + leftOffset / 2 ;
        for(i=0; i<copyHeight/2 ; i++)
        {
            SHA1_Update(&shaCtx, srcPtr, copyWidth/2);
            srcPtr += srcWidth/2;
        }

        srcPtr =((unsigned char*)p->pData2) + \
                p->nWidth/2 * topOffset / 2 + leftOffset / 2 ;
        for(i=0; i<copyHeight/2 ; i++)
        {
            SHA1_Update(&shaCtx, srcPtr, copyWidth/2);
            srcPtr += srcWidth/2;
        }
    }
    SHA1_Final(sha, &shaCtx);

    nShaInfoLen += sprintf(pShaInfo + nShaInfoLen,",sha:");
    for (j = 0; j < 20; j++)
    {
        nShaInfoLen += sprintf(pShaInfo + nShaInfoLen,"%02x", sha[j]);
    }
    shaInfo[nShaInfoLen] = '\n';

    if(bNeedCompSha == 1)
    {
        int cmpResult = strcmp(CmpSha, shaInfo);
        if(cmpResult != 0)
        {
            pVideoDecoder->nCompareShaErrorCount++;
            logd("***sha-cmp-error. ret = %d, %s, %s\n",cmpResult, CmpSha, shaInfo);
        }
    }

    logv("*** the sha : %s",shaInfo);
    fprintf(fp, "%s",shaInfo);
}


void *displayPictureThreadFunc(void* param)
{
    DecDemo  *pVideoDecoder;
    VideoDecoder *pVideoDec;
    VideoPicture* pPicture;
    display *DisHeader, *DisRear, *pDisList;
    paramStruct_t m_iPicture;
    paramStruct_t m_oPicture;
    long long nTime, DurationTime;
    int state, nDispFrameCount, nValidPicNum;

    pVideoDecoder = (DecDemo *)param;

    DisHeader = NULL;
    DisRear = NULL;
    nDispFrameCount = 0;
    nValidPicNum = 0;
    DurationTime = 0;
    pVideoDec = pVideoDecoder->pVideoDec;
    allocPicMem(&m_oPicture, pVideoDecoder->oPicW*pVideoDecoder->oPicH*4);
    allocPicMem(&m_iPicture, pVideoDecoder->iPicW*pVideoDecoder->iPicH*3/2);
    pVideoDecoder->IMG_ptr->header.cf = LV_IMG_CF_TRUE_COLOR;
    pVideoDecoder->IMG_ptr->header.always_zero = 0;
    pVideoDecoder->IMG_ptr->header.w = pVideoDecoder->oPicW;
    pVideoDecoder->IMG_ptr->header.h = pVideoDecoder->oPicH;
    pDisList = calloc(DISPLAY_PICTUER_LIST_NUM, sizeof(display));
    if(pDisList == NULL)
    {
        pthread_rwlock_wrlock(&pVideoDecoder->thread.rwrock);
        *pVideoDecoder->thread.state |= DEMO_DISPLAY_ERROR;
        pthread_rwlock_unlock(&pVideoDecoder->thread.rwrock);
        logd(" display thread calloc memory fial ");
        goto display_thread_exit;
    }
    logd(" display thread starting..... ");
    pVideoDecoder->nDispFrameCount = 0;
    nTime = GetNowUs();
    while(1)
    {
        usleep(100);
        pthread_rwlock_wrlock(&pVideoDecoder->thread.rwrock);
        state = *pVideoDecoder->thread.state;
        pthread_rwlock_unlock(&pVideoDecoder->thread.rwrock);
        if(state & DEMO_EXIT)
        {
            loge(" display thread recieve an error singnal,  exit..... ");
            break;
        }
        pPicture = RequestPicture(pVideoDec, 0/*the major stream*/);
        if(pPicture != NULL)
        {
            logv(" decoder get one picture, size: %dx%d ", pPicture->nWidth, pPicture->nHeight);
            nTime = GetNowUs() - nTime;
            DurationTime += nTime;
            nDispFrameCount += 1;

            if(nDispFrameCount >= pVideoDecoder->nSavePictureStartNumber &&
                    nDispFrameCount < (pVideoDecoder->nSavePictureStartNumber +
                                    pVideoDecoder->nSavePictureNumber))
            {
                savePicture(pPicture, pVideoDecoder);
                pVideoDecoder->nSavePicCount++;
            }

            if (pVideoDecoder->pSaveShaFp != NULL)
            {
                saveVideoPictureInfo(pPicture,pVideoDecoder);
            }

            if(nDispFrameCount >= pVideoDecoder->nFinishNum)
            {
                loge(" display thread get enungh frame, exit ...");
                pthread_rwlock_wrlock(&pVideoDecoder->thread.rwrock);
                *pVideoDecoder->thread.state |= DEMO_DECODE_FINISH;
                pthread_rwlock_unlock(&pVideoDecoder->thread.rwrock);
                break;
            }
#if DEBUG_TIME_INFO
            if(nDispFrameCount >= FRAME_COUNT)
            {
                float fps, avg;
                pVideoDecoder->nDispFrameCount += nDispFrameCount;
                pVideoDecoder->DurationTime += DurationTime;
                fps = (float)(DurationTime / 1000);
                fps = (nDispFrameCount * 1000) / fps;
                avg = (float)(pVideoDecoder->DurationTime / 1000);
                avg = (pVideoDecoder->nDispFrameCount * 1000) / avg;

                loge(" decoder speed info. current speed: %.2f, average speed: %.2f ", fps, avg);
                DurationTime = 0;
                nDispFrameCount = 0;
            }
#endif
            nTime = GetNowUs();
            addPictureToList(pVideoDec, &pVideoDecoder->img_lock,
                        pVideoDecoder->img_domain, pVideoDecoder->IMG_ptr, pDisList,
                        &DisHeader, &DisRear, pPicture, 
                        &m_iPicture, &m_oPicture,
                        pVideoDecoder->oPicW, pVideoDecoder->oPicH);
        }
        else
        {
            pthread_rwlock_wrlock(&pVideoDecoder->thread.rwrock);
            state = *pVideoDecoder->thread.state;
            pthread_rwlock_unlock(&pVideoDecoder->thread.rwrock);
            if(state & DEMO_DECODER_EXIT)
            {
                nValidPicNum = ValidPictureNum(pVideoDec, 0);
                logv(" display thread find that decode thread had exit ");
                if(nValidPicNum <= 0)
                    break;
            }
        }
    }

    pVideoDecoder->nDispFrameCount += nDispFrameCount;
    pthread_rwlock_wrlock(&pVideoDecoder->thread.rwrock);
    *pVideoDecoder->thread.state |= DEMO_DISPLAY_EXIT;
    pthread_rwlock_unlock(&pVideoDecoder->thread.rwrock);
    ReturnAllPicture(pVideoDec, DisHeader);
    logd(" display thread exit....disp frame num: %d ", pVideoDecoder->nDispFrameCount);
display_thread_exit:
    if(pDisList)
        free(pDisList);
    lv_img_set_src(pVideoDecoder->img_domain, &no_camera_200x200);
    lv_obj_set_size(pVideoDecoder->img_domain, 200, 200);
    lv_obj_align(pVideoDecoder->img_domain, LV_ALIGN_CENTER, 0, 0);

    freePicMem(&m_oPicture);
    freePicMem(&m_iPicture);
    sync();
    pthread_exit(NULL);
    return 0;
}

void *parserThreadFunc(void* param)
{
    DecDemo *pDec;
    CdxParserT *parser;
    VideoDecoder *pVideoDec;
    int nRet, nStreamNum, state;
    int nValidSize;
    int nRequestDataSize, trytime;
    unsigned char *buf;
    VideoStreamDataInfo  dataInfo;
    CdxPacketT packet;

    buf = malloc(1024*1024);
    if(buf == NULL)
    {
        loge(" parser thread malloc error ");
        goto parser_exit;
    }
    pDec = (DecDemo *)param;
    pVideoDec = pDec->pVideoDec;
    parser = pDec->parser;
    memset(&packet, 0, sizeof(packet));
    logv(" parserThreadFunc(), thread created ! ");
    state = 0;
    trytime = 0;
    while (1)
    {
        if(CdxParserPrefetch(parser, &packet) != 0){
            pthread_rwlock_wrlock(&pDec->thread.rwrock);
            state = *pDec->thread.state;
            pthread_rwlock_unlock(&pDec->thread.rwrock);
            if(state & DEMO_EXIT)
            {
                loge(" hevc parser receive other thread error. exit flag ");
                goto parser_exit;
            }
            continue;
        }
        usleep(50);
        nValidSize = VideoStreamBufferSize(pVideoDec, 0) - VideoStreamDataSize(pVideoDec, 0);
        nRequestDataSize = packet.length;

        pthread_rwlock_wrlock(&pDec->thread.rwrock);
        state = *pDec->thread.state;
        pthread_rwlock_unlock(&pDec->thread.rwrock);
        if(state & DEMO_EXIT)
        {
            loge(" hevc parser receive other thread error. exit flag ");
            goto parser_exit;
        }
        if(trytime >= 2000)
        {
            loge("  parser thread trytime >= 2000, maybe some error happen ");
            pthread_rwlock_wrlock(&pDec->thread.rwrock);
            *pDec->thread.state |= DEMO_PARSER_ERROR;
            pthread_rwlock_unlock(&pDec->thread.rwrock);
            goto parser_exit;
        }
        if (packet.type == CDX_MEDIA_VIDEO && ((packet.flags&MINOR_STREAM)==0))
        {
            if(nRequestDataSize > nValidSize)
            {
                usleep(50 * 1000);
                trytime++;
                continue;
            }

            nRet = RequestVideoStreamBuffer(pVideoDec,
                                            nRequestDataSize,
                                            (char**)&packet.buf,
                                            &packet.buflen,
                                            (char**)&packet.ringBuf,
                                            &packet.ringBufLen,
                                            0);
            if(nRet != 0)
            {
                logw(" RequestVideoStreamBuffer fail. request size: %d, valid size: %d ",
                        nRequestDataSize, nValidSize);
                usleep(50*1000);
                continue;
            }
            if(packet.buflen + packet.ringBufLen < nRequestDataSize)
            {
                loge(" RequestVideoStreamBuffer fail, require size is too small ");
                pthread_rwlock_wrlock(&pDec->thread.rwrock);
                *pDec->thread.state |= DEMO_PARSER_ERROR;
                pthread_rwlock_unlock(&pDec->thread.rwrock);
                goto parser_exit;
            }
        }
        else
        {
            packet.buf = buf;
            packet.buflen = packet.length;
            CdxParserRead(parser, &packet);
            continue;
        }
        trytime = 0;
        nStreamNum = VideoStreamFrameNum(pVideoDec, 0);
        if(nStreamNum > DEMO_PARSER_MAX_STREAM_NUM)
        {
            usleep(50*1000);
        }
        nRet = CdxParserRead(parser, &packet);
        if(nRet != 0)
        {
            loge(" parser thread read video data error ");
            pthread_rwlock_wrlock(&pDec->thread.rwrock);
            *pDec->thread.state |= DEMO_PARSER_ERROR;
            pthread_rwlock_unlock(&pDec->thread.rwrock);
            goto parser_exit;
        }
        memset(&dataInfo, 0, sizeof(VideoStreamDataInfo));
        dataInfo.pData          = packet.buf;
        dataInfo.nLength      = packet.length;
        dataInfo.nPts          = packet.pts;
        dataInfo.nPcr          = packet.pcr;
        dataInfo.bIsFirstPart = (!!(packet.flags & FIRST_PART));
        dataInfo.bIsLastPart = (!!(packet.flags & LAST_PART));
        dataInfo.bValid = 1;
        nRet = SubmitVideoStreamData(pVideoDec , &dataInfo, 0);
        if(nRet != 0)
        {
            loge(" parser thread  SubmitVideoStreamData() error ");
            pthread_rwlock_wrlock(&pDec->thread.rwrock);
            *pDec->thread.state |= DEMO_PARSER_ERROR;
            pthread_rwlock_unlock(&pDec->thread.rwrock);
            goto parser_exit;
        }
    }

    pthread_rwlock_wrlock(&pDec->thread.rwrock);
    pDec->thread.nEndofStream = 1;
    *pDec->thread.state |= DEMO_PARSER_EXIT;
    pthread_rwlock_unlock(&pDec->thread.rwrock);

parser_exit:
    if(buf)
        free(buf);
    logv(" parser exit..... ");
    pthread_exit(NULL);
    return 0;
}

/*
 * DRAMcostTHread() thread make cpu do some other thing, cost DRAM bandwidth
 * */
void *DRAMcostTHread(void *arg)
{
#define MEMORY_STRIDE 1920
#define MEMORY_NUM 8
#define MEMORY_BLOCK (1080*MEMORY_STRIDE)
#define MEMORY_SIZE (MEMORY_BLOCK*MEMORY_NUM)

    DecDemo *pDec;
    VideoDecoder *pVideoDec;
    //int bExitFlag = 0;
    int i, j;
    int times, state;
    char *pSrc, *pDst, *p;

    pSrc = NULL;
    pDst = NULL;
    pDec = (DecDemo *)arg;
    pVideoDec = pDec->pVideoDec;

    pSrc = (char *)malloc(MEMORY_SIZE);
    if(pSrc == NULL)
    {
        logd(" DRAMcostTHread malloc fail ....pSrc ");
        goto DRAM_cost_exit;
    }
    pDst = (char *)malloc(MEMORY_SIZE);
    if(pDst == NULL)
    {
        logd(" DRAMcostTHread malloc fail ....pDst ");
        goto DRAM_cost_exit;
    }
    logd(" DRAM memory copy thread created .... ");
    times = 0;
    while(1)
    {
        char *s, *d;
        usleep(100);
        pthread_rwlock_wrlock(&pDec->thread.rwrock);
        //bExitFlag = pDec->thread.nEndofStream ;
        state = *pDec->thread.state;
        pthread_rwlock_unlock(&pDec->thread.rwrock);
        if(state & DEMO_DECODER_EXIT)
        {
            logd(" DRAM COST THREAD EIXT..... ");
            break;
        }

        for(j = 0; j < MEMORY_NUM; j++)
        {
            s = pSrc + j*MEMORY_BLOCK;
            d = pDst + j*MEMORY_BLOCK;
            for(i = 0; i < 1080; i++)
            {
                int k;
                p = s;
                for(k = 0; k < MEMORY_NUM*10 && k*j < MEMORY_STRIDE; k++)
                    p[k*j] = rand()%128;
                memcpy(d, s, MEMORY_STRIDE);
                s += MEMORY_STRIDE;
                d += MEMORY_STRIDE;
            }
        }
    }
DRAM_cost_exit:
    if(pSrc)
        free(pSrc);
    if(pDst)
        free(pDst);
    pthread_exit(NULL);
    return 0;
}

void DemoHelpInfo(void)
{
    logd(" ==== CedarX linux decoder demo help start ===== ");
    logd(" -h or --help to show the demo usage");
    logd(" demo created by zouwenhuan, allwinnertech/AL3 ");
    logd(" email: zouwenhuan@allwinnertech.com ");
    logd(" ===== CedarX linux decoder demo help end ====== ");
}

void* ChannelThread(void* param)
{
    DecDemo *pDec = (DecDemo *)param;
    int nRet = 0;
    int i, nDramCostThreadNum;
    pthread_t tdecoder, tparser, tdisplay;
    pthread_t dram[DRAM_COST_MAX_THREAD_NUM];
    long long endTime;
    char *pInputFile;
    char *pSaveShaFile;
    char *pCompareShaFile;

    logd("ChannelThread 11, %d", pDec->nChannel);

    pDec->nFinishNum = 0x7fffffff;
    pDec->nDramCostThreadNum = 0;
    pDec->nSavePictureNumber = 0;
    pDec->nSavePictureStartNumber = 0x7fffffff;
    pDec->iPicW = 1920;
    pDec->iPicH = 1080;
    pDec->oPicW = 505;
    pDec->oPicH = 350;

    //pInputFile = "rtsp://admin:@192.168.1.11:554/h265/ch1/sub/av_stream";
    pSaveShaFile = calloc(DEMO_FILE_NAME_LEN, 1);
    if(pSaveShaFile == NULL)
    {
        loge(" output file. calloc memory fail. ");
        //free(pInputFile);
        return 0;
    }
    pCompareShaFile = calloc(DEMO_FILE_NAME_LEN, 1);
    if(pCompareShaFile == NULL)
    {
        loge(" output file. calloc memory fail. ");
        //free(pInputFile);
        free(pSaveShaFile);
        return 0;
    }
    
    ///pDec->pInputFile = pInputFile;
    pDec->pSaveShaFile = pSaveShaFile;
    pDec->pCompareShaFile = pCompareShaFile;
    pDec->pSavePicPath = NULL;

    if(pDec->pSaveShaFile!= NULL && strcmp(pDec->pSaveShaFile,"") != 0)
    {
        pDec->pSaveShaFp = fopen(pDec->pSaveShaFile, "wb");
        logd("open save sha file: %p",pDec->pSaveShaFp);
    }
    if(pDec->pCompareShaFile!= NULL && strcmp(pDec->pCompareShaFile,"") != 0)
    {
        pDec->pCompareShaFp= fopen(pDec->pCompareShaFile, "rb");
        logd("open compare sha file: %p",pDec->pCompareShaFp);
    }

    nRet = initDecoder(pDec);
    if(nRet != 0)
    {
        loge(" decoder demom initDecoder error ");
        return 0;
    }
    logd("decoder file: %s", pDec->pInputFile);

    pDec->totalTime = GetNowUs();

    nDramCostThreadNum = pDec->nDramCostThreadNum;

    int policy;
    struct sched_param dec_sched_param;
    dec_sched_param.sched_priority = 50;
    pthread_attr_getschedpolicy(&thread_attr, &policy);

    pthread_create(&tparser, NULL, parserThreadFunc, (void*)(pDec));
    pthread_create(&tdecoder, NULL, DecodeThread, (void*)(pDec));
    pthread_create(&tdisplay, NULL, displayPictureThreadFunc, (void*)(pDec));

    //pthread_setschedparam(tparser, policy, &dec_sched_param);
    //pthread_setschedparam(tdecoder, policy, &dec_sched_param);
    //pthread_setschedparam(tdisplay, policy, &dec_sched_param);
    for(i = 0; i < nDramCostThreadNum; i++)
    {
        pthread_create(&dram[i], NULL, DRAMcostTHread, (void*)(pDec));
        logd(" creat dram memory copy thread[%d] ", i);
    }

    pthread_join(tparser, (void**)&nRet);
    pthread_join(tdecoder, (void**)&nRet);
    pthread_join(tdisplay, (void**)&nRet);
    for(i = 0; i < nDramCostThreadNum; i++)
        pthread_join(dram[i], (void**)&nRet);

    endTime = GetNowUs();
    pDec->totalTime = endTime - pDec->totalTime;
    logd(" demoDecoder finish.decode frame: %d, display frame: %d, cost %lld s ",
            pDec->nDecodeFrameCount, pDec->nDispFrameCount, pDec->totalTime/(1000*1000));
    if(pDec->pCompareShaFp != NULL)
        logd("******** nCompare-Sha-Error-Count = %d", pDec->nCompareShaErrorCount);
    pthread_mutex_destroy(&pDec->parserMutex);
    CdxParserClose(pDec->parser);
    logv(" after CdxParserClose()");
    DestroyVideoDecoder(pDec->pVideoDec);

    //if(pInputFile != NULL)
        //free(pInputFile);
    if(pDec->pSavePicPath != NULL)
        free(pDec->pSavePicPath);
    if(pSaveShaFile != NULL)
        free(pSaveShaFile);
    if(pCompareShaFile != NULL)
        free(pCompareShaFile);
    if(pDec->pSaveShaFp != NULL)
        fclose(pDec->pSaveShaFp);
    if(pDec->pCompareShaFp != NULL)
        fclose(pDec->pCompareShaFp);

    logd(" demo decoder exit successful");
    CdcMemClose(pDec->memops);

    /*lv_img_set_src(pDec->img_domain, &no_camera_200x200);
    lv_obj_set_size(pDec->img_domain, 200, 200);
    lv_obj_align(pDec->img_domain, LV_ALIGN_CENTER, 0, 0);*/

    lv_img_set_src(pDec->img_domain, &no_camera_200x200);
    lv_obj_set_size(pDec->img_domain, 200, 200);
    lv_obj_align(pDec->img_domain, LV_ALIGN_CENTER, 0, 0);

    pthread_exit(NULL);
    return 0;
}

void decoder_start(void** ppDecoder, const char* inputpath, lv_obj_t* img_domain, lv_img_dsc_t* IMG_p, int* state){
    static pthread_t thread_id;
    DecDemo* pDecoder = calloc(sizeof(DecDemo), 1);
    pDecoder->pInputFile = inputpath;
    pDecoder->img_domain = img_domain;
    pDecoder->IMG_ptr = IMG_p;
    pDecoder->thread.state = state;
    *ppDecoder = (void*)pDecoder;
    pthread_create(&thread_id, NULL, ChannelThread, (void*)(pDecoder));
}

void decoder_stop(void* pDecoder, int* state){
    *state |= DEMO_EXIT;
    if(pDecoder){
        sleep(1);
        free(pDecoder);
    }
}

/*=======================mydecoder====================*/
#if 0

int initMyDecoder() {
	int result;
	//AddVDPlugin();
	//mScMemOpsS = MemAdapterGetOpsS();
	pVideoDec = CreateVideoDecoder();
	if(pVideoDec == NULL) {
		printf("Unable to create videoDec\n");
		return -1;
	}
	memset(&mVideoInfo, 0, sizeof(VideoStreamInfo));

	mVideoInfo.eCodecFormat          = VIDEO_CODEC_FORMAT_H265;
	mVideoInfo.nWidth                = 800;
	mVideoInfo.nHeight               = 448;
	mVideoInfo.nFrameRate            = 12 * 1000;
	mVideoInfo.nFrameDuration        = (1000 * 1000 * 1000) / mVideoInfo.nFrameRate;
	mVideoInfo.nAspectRatio          = 0;
	mVideoInfo.bIs3DStream           = 0;
	mVideoInfo.nCodecSpecificDataLen = 0;
	mVideoInfo.pCodecSpecificData    = NULL;

	memset(&VideoConf, 0, sizeof(VConfig));
	
	VideoConf.eOutputPixelFormat                = PIXEL_FORMAT_YUV_PLANER_420;
    VideoConf.nDeInterlaceHoldingFrameBufferNum = GetConfigParamterInt("pic_4di_num", 2);
    VideoConf.nDisplayHoldingFrameBufferNum = GetConfigParamterInt("pic_4list_num", 3);
    VideoConf.nRotateHoldingFrameBufferNum = GetConfigParamterInt("pic_4rotate_num", 0);
    VideoConf.nDecodeSmoothFrameBufferNum = GetConfigParamterInt("pic_4smooth_num", 3);
	/*VideoConf.nDeInterlaceHoldingFrameBufferNum = 0;
	VideoConf.nDisplayHoldingFrameBufferNum     = 3;
	VideoConf.nRotateHoldingFrameBufferNum      = 0;
	VideoConf.nDecodeSmoothFrameBufferNum       = 0;*/
    memset(&mSetparmMemops, 0, sizeof(paramStruct_t));
    allocOpen(MEM_TYPE_CDX_NEW, &mSetparmMemops, NULL);
	VideoConf.memops                            = (struct ScMemOpsS*)mSetparmMemops.ops;
	//CdcMemOpen(VideoConf.memops);

	result = InitializeVideoDecoder(pVideoDec, &mVideoInfo, &VideoConf);
	if(result != 0) {
		DestroyVideoDecoder(pVideoDec);
		pVideoDec = NULL;
		printf("Unable to initialize videodecoder.\n");
		return -1;
	}
	return 0;
}

void closeMyDecoder(){
    DestroyVideoDecoder(pVideoDec);
    allocClose(MEM_TYPE_CDX_NEW, &mSetparmMemops, NULL);
}


int allocPicMem(paramStruct_t*pops,int size)
{
    int iRet = 0;

    iRet = allocOpen(MEM_TYPE_CDX_NEW, pops, NULL);
    if (iRet < 0) {
        printf("ion_alloc_open failed\n");
        return iRet;
    }
    pops->size =size;
    iRet = allocAlloc(MEM_TYPE_CDX_NEW, pops, NULL);
    if(iRet < 0) {
        printf("allocAlloc failed\n");
        return iRet;
    }

    return 0;
}

int freePicMem(paramStruct_t*pops)
{
    int iRet = 0;

	allocFree(MEM_TYPE_CDX_NEW, pops, NULL);

    return 0;
}

void g2d_yuv2rgb(VideoPicture* iPicture,  paramStruct_t* oPicture){
    paramStruct_t m_iPicture;
    int src_width = iPicture->nWidth;
    int src_height = iPicture->nHeight;
    int src_format = (int)G2D_FORMAT_YUV420_PLANAR;
    int src_size = src_width * src_height * 3/2;

    /* output param */
    int dst_width = 480;
    int dst_height = 330;
    int dst_format = (int)G2D_FORMAT_ARGB8888;
    int dst_size = dst_width * dst_height * 4;
    /* commond flag */
    int flag = (int)G2D_ROT_0;

    allocPicMem(&m_iPicture, src_size);
    memset((void*)oPicture->vir, 0, oPicture->size);
    memcpy((void*)m_iPicture.vir, iPicture->pData0, src_size);
    flushCache(MEM_TYPE_CDX_NEW, &m_iPicture, NULL);
    int src_fd = m_iPicture.ion_buffer.fd_data.aw_fd;
    int dst_fd = oPicture->ion_buffer.fd_data.aw_fd;

    int g2d_fd = open("/dev/g2d", O_RDWR);
    printf("================== start RGBA scaler =============\n");
    g2d_blt_h blt;
    memset(&blt, 0, sizeof(blt));

    blt.flag_h = G2D_BLT_NONE_H;
    blt.src_image_h.format = (g2d_fmt_enh)src_format;
    blt.src_image_h.width = src_width;
    blt.src_image_h.height = src_height;
    blt.src_image_h.align[0] = 0;
    blt.src_image_h.align[1] = 0;
    blt.src_image_h.clip_rect.x = 0;
    blt.src_image_h.clip_rect.y = 0;
    blt.src_image_h.clip_rect.w = src_width;
    blt.src_image_h.clip_rect.h = src_height;
    blt.src_image_h.gamut = G2D_BT2020;
    blt.src_image_h.bpremul = 0;
    blt.src_image_h.mode = G2D_PIXEL_ALPHA;
    blt.src_image_h.use_phy_addr = 0;
    blt.src_image_h.fd = src_fd;

    blt.dst_image_h.format = (g2d_fmt_enh)dst_format;
    blt.dst_image_h.width = dst_width;
    blt.dst_image_h.height = dst_height;
    blt.dst_image_h.align[0] = 0;
    blt.dst_image_h.align[1] = 0;
    blt.dst_image_h.clip_rect.x = 0;
    blt.dst_image_h.clip_rect.y = 0;
    blt.dst_image_h.clip_rect.w = dst_width;
    blt.dst_image_h.clip_rect.h = dst_height;
    blt.dst_image_h.gamut = G2D_BT2020;
    blt.dst_image_h.bpremul = 0;
    blt.dst_image_h.mode = G2D_PIXEL_ALPHA;
    blt.dst_image_h.use_phy_addr = 0;
    blt.dst_image_h.fd = dst_fd;

    ioctl(g2d_fd, G2D_CMD_BITBLT_H, &blt);
    printf("================RGBA scaler finished!!========= \n");
    freePicMem(&m_iPicture);
}

using namespace awvideodecoder;
DecodeParam decodeParam;
AWVideoDecoder* pDecoder;

int initMyDecoder(){
    memset(&decodeParam, 0, sizeof(decodeParam));

    decodeParam.srcW = 800;
    decodeParam.srcH = 448;
    decodeParam.dstW = 380;
    decodeParam.dstH = 330;
    decodeParam.rotation = Angle_0;
    decodeParam.scaleRatio = ScaleNone;

    decodeParam.codecType = CODEC_H265;
    decodeParam.pixelFormat = PIXEL_NV21;

    AWVideoDecoder* pDecoder = AWVideoDecoder::create(&decodeParam);
}

void closeMyDecoder(){
    AWVideoDecoder::destroy(pDecoder);
}

#endif

#if 0
int find264NALFragment(char* data, size_t size, int* fragSize)
{
    static const char kStartCode[4] = { 0x00, 0x00, 0x00, 0x01 };
    if (size < 4)
    {
        printf("size %zu < 4\n", size);
        return -1;
    }

    if (memcmp(kStartCode, data, 4))
    {
        printf("StartCode not found in %.2x %.2x %.2x %.2x\n", data[0], data[1], data[2], data[3]);
        return -2;
    }

    size_t offset = 4;
    while (offset + 3 < size && memcmp(kStartCode, &data[offset], 4))
    {
        ++offset;
    }

    if (offset > (size - 4))
    {
        printf("offset>(size-4)\n");
        offset = size;
    }
    *fragSize = offset;

    return (int)(data[4] & 0x1f);
}


extern pthread_mutex_t stat_lock;

int decodeOneFrame(const uint8_t* inputData, size_t inputSize) {
    static uint16_t frameIndex = 0;
	int result, nVideoEOF=0;
	char* packet_buf = NULL, *packet_ringbuf=NULL;
	int packet_buflen = 0, packet_ringbuflen = 0;
	char *pSampleBuf = NULL;
	char *pTempBuf = NULL;
    int intputLen = DEFAULT_BUF_LEN;
    VideoPicture *pTempPicture;
	VideoStreamDataInfo  mDataInfo;

    char* decodePtr = NULL;
    int decodePos = 0;
    int readTmpLen = 0;
    int nalLen = 0;
    int count = 1;
    int data = 1;
	int nRequestDataSize = 0, nValidDataSize = 0;
	while(decodePos < inputSize){
        decodePtr = (char*)inputData+decodePos;
        result = find264NALFragment(decodePtr, inputSize, &nalLen);
	    if (nalLen < 4)
        {
            printf("find264NALFragment fail \n");
            break;
        }
		printf("decode pos: %d, nalu len: %d\n", decodePos, nalLen);
		nRequestDataSize = nalLen;
		nValidDataSize = VideoStreamBufferSize(pVideoDec, 0) - VideoStreamDataSize(pVideoDec, 0);
		if(nRequestDataSize > nValidDataSize){
			printf("nRequestDatasize > nValidDataSize.\n");
			usleep(5000);
            continue;
		}
		result = RequestVideoStreamBuffer(pVideoDec, nalLen, (char **)&packet_buf, &packet_buflen, (char **)&packet_ringbuf, &packet_ringbuflen, 0);
		if(result != 0) {
			printf("Request video stream buffer failed.\n");
			return -1;
		}
		if(nRequestDataSize <= packet_buflen){
			memcpy(packet_buf, decodePtr, nalLen);
		}
		else if(nRequestDataSize <= (packet_buflen + packet_ringbuflen)){
			memcpy(packet_buf, decodePtr, packet_buflen);
			memcpy(packet_ringbuf, decodePtr+packet_buflen-1, nalLen-packet_buflen);
		}
		memset(&mDataInfo, 0, sizeof(VideoStreamDataInfo));
		mDataInfo.pData        = (char *)packet_buf;
		mDataInfo.nLength      = nRequestDataSize;
		mDataInfo.nPts         = -1;
		mDataInfo.nPcr         = -1;
		mDataInfo.bIsFirstPart = 1;
		mDataInfo.bIsLastPart  = 1;
		mDataInfo.bValid = 1;
		result = SubmitVideoStreamData(pVideoDec, &mDataInfo,0);
		if(result != 0) {
			printf("submot video stream data faild.\n");
			return -1;
		}
		int ret = DecodeVideoStream(pVideoDec, 0, 0, 0, 0);
		printf("decode frame, return [%d], frameIndex: %d\n", ret, frameIndex);
        if (ret == VDECODE_RESULT_KEYFRAME_DECODED ||
                ret == VDECODE_RESULT_FRAME_DECODED){
        }
        else if(ret == VDECODE_RESULT_OK){
            //ReopenVideoEngine(pVideoDec, &VideoConf, &mVideoInfo);
            usleep(20000);
        }
        if(frameIndex++ > 400){
            pthread_mutex_lock(&stat_lock);
            //closeMyDecoder();
            //initMyDecoder();
            pthread_mutex_unlock(&stat_lock);
            frameIndex = 0;
        }
		decodePos += nalLen;
	}

	return 0;
}

lv_img_dsc_t IMG1;

static char copyPicture(VideoPicture* pPicture)
{
    CDX_UNUSE(pPicture);
    return 0;
}

static void addPictureToList(VideoDecoder *pVideoDec, display *pDisList,
        display **h, display **r, VideoPicture* pPicture, paramStruct_t* m_oPicture)
{
    int i;
    display *node = NULL;
    display *DisHeader = *h;
    display *DisRear = *r;
    if(pPicture == NULL || pDisList == NULL)
    {
        loge(" add picuter to list error");
        return;
    }
    for(i = 0; i < DISPLAY_PICTUER_LIST_NUM; i++)
    {
        if(pDisList[i].flag == 0)
        {
            node = &pDisList[i];
            node->flag = 1;
            node->picture = pPicture;
            break;
        }
    }
    if(DisHeader == NULL && DisRear == NULL)
    {
        DisHeader = DisRear = node;
    }
    else
    {
        DisRear->next = node;
        node->next = NULL;
        DisRear = node;
    }

    i = 1;
    node = DisHeader;
    while(node != NULL && node->next != NULL)
    {
        i += 1;
        node = node->next;
    }
    if(i >= DISPLAY_HOLDING_BUFFERS)
    {
        node = DisHeader;
        DisHeader = DisHeader->next;
        node->next = NULL;
        node->flag = 0;
        copyPicture(node->picture);
        printf("image size: %dx%d, stat: %d, size: %d, display one picture. pts: %lld\n",
                node->picture->nWidth, node->picture->nHeight, 
                node->picture->nBufStatus, node->picture->nBufSize, node->picture->nPts);
        g2d_yuv2rgb(node->picture, m_oPicture);
        flushCache(MEM_TYPE_CDX_NEW, m_oPicture, NULL);
        IMG1.data_size = m_oPicture->size;
        IMG1.data = (uint8_t*)m_oPicture->vir;
        printf("image size: %d, empty picture: %d\n", 
                m_oPicture->size, EmptyPictureBufferNum(pVideoDec, 0));
        lv_img_set_src(top_img_domain, &IMG1);
        lv_obj_invalidate(top_img_domain);
        usleep(10000);
        ReturnPicture(pVideoDec, node->picture);
    }
    *h = DisHeader;
    *r = DisRear;
}

void* player_thread(void* args){
    VideoPicture* pTempPicture;
    paramStruct_t m_oPicture;
    display *DisHeader, *DisRear, *pDisList;
    allocPicMem(&m_oPicture, 480*330*4);
    IMG1.header.cf = LV_IMG_CF_TRUE_COLOR;
    IMG1.header.always_zero = 0;
    IMG1.header.w = 480;
    IMG1.header.h = 330;
    pDisList = calloc(DISPLAY_PICTUER_LIST_NUM, sizeof(display));
    while(1){
        pthread_mutex_lock(&stat_lock);
        pTempPicture = RequestPicture(pVideoDec, 0);

        if(pTempPicture != NULL){
            addPictureToList(pVideoDec, pDisList, &DisHeader, &DisRear, pTempPicture, &m_oPicture);
        }
        pthread_mutex_unlock(&stat_lock);
        usleep(1000);
    }
    freePicMem(&m_oPicture);
    printf("player thread exited..\n");
}

pthread_t player_thread_handle;
int player_start(){
    pthread_create(&player_thread_handle, NULL, player_thread, NULL);
}

#endif
