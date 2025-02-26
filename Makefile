#
# Makefile
#
export CC = arm-linux-gnueabi-gcc
export GCC = arm-linux-gnueabi-g++
LVGL_DIR_NAME ?= lvgl
LVGL_DIR ?= ${shell pwd}
LIBCEDARX_DIR = /home/yuzang/linux/linux/multimedia/libcedarx
LIBCEDARC_DIR = /home/yuzang/linux/linux/multimedia/libcedarc
EXTERNAL_LIB_DIR = $(LVGL_DIR)/external/libs

CFLAGS_CDXG = -g -Wall -D_GNU_SOURCE -DCONF_H265_4K \
                        -DCONF_KERNEL_VERSION_4_9 \
                        -DCONF_GPU_MALI \
                        -DCONF_KERN_BITWIDE=64 \
                        -DCONF_NEW_BDMV_STREAM \
                        -DCONF_NEW_DISPLAY \
                        -DCONF_ION_HANDLE_INT \
                        -DCONF_VE_PHY_OFFSET \
                        -DCONF_PRODUCT_STB \
                        -DCONF_3D_ENABLE \
                        -DCONF_USE_IOMMU \
                        -DCONF_AFBC_ENABLE \
                        -DCONF_HIGH_DYNAMIC_RANGE_ENABLE \
                        -DCONF_SCALE_DOWN \
			  -Wno-unused-variable \
			  -Wno-unused-result \
			  -Wno-unused-function \
			  -Wno-unused-value \
			  -Wno-pointer-to-int-cast \
			  -Wno-int-to-pointer-cast \
			  -Wno-unused-label \
			  -Wno-strict-aliasing \
			  -Wno-unused-but-set-variable 

CFLAGS ?= $(CFLAGS_CDXG) -I$(LVGL_DIR)/ -Wshadow -Wundef -Wmissing-prototypes -Wno-discarded-qualifiers -Wextra -Wno-unused-function -Wno-error=strict-prototypes -Wpointer-arith -fno-strict-aliasing -Wno-error=cpp -Wuninitialized -Wmaybe-uninitialized -Wno-unused-parameter -Wno-missing-field-initializers -Wtype-limits -Wsizeof-pointer-memaccess -Wno-format-nonliteral -Wno-cast-qual -Wunreachable-code -Wno-switch-default -Wreturn-type -Wmultichar -Wformat-security -Wno-ignored-qualifiers -Wno-error=pedantic -Wno-sign-compare -Wno-error=missing-prototypes -Wdouble-promotion -Wclobbered -Wdeprecated -Wempty-body -Wtype-limits -Wstack-usage=2048 -Wno-unused-value -Wno-unused-parameter -Wno-missing-field-initializers -Wuninitialized -Wmaybe-uninitialized -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -Wtype-limits -Wsizeof-pointer-memaccess -Wno-format-nonliteral -Wpointer-arith -Wno-cast-qual -Wmissing-prototypes -Wunreachable-code -Wno-switch-default -Wreturn-type -Wmultichar -Wno-discarded-qualifiers -Wformat-security -Wno-ignored-qualifiers -Wno-sign-compare \
			-I$(LIBCEDARX_DIR) \
			-I$(LIBCEDARX_DIR)/libcore/base/include/ \
			-I$(LIBCEDARX_DIR)/libcore/parser/include/ \
			-I$(LIBCEDARX_DIR)/libcore/stream/include/ \
			-I$(LIBCEDARC_DIR)/include/ \
			-I$(LIBCEDARX_DIR)/external/include/adecoder \
			-I$(LIBCEDARX_DIR)/external/include/ \
			-I$(LIBCEDARX_DIR)/libcore/common/iniparser/ \
			-I/home/yuzang/linux/linux/auto-t113x-linux/platform/framework/auto/sdk_lib/include
CPPFLAGS = $(CFLAGS) -std=c++17 -I/home/yuzang/linux/tools/ffmpeg-7.0.2/arm-myir-install/include/ \
			-I/home/yuzang/linux/linux/auto-t113x-linux/platform/framework/auto/sdk_lib/include/  \
			-I/home/yuzang/linux/tools/sqlite-version-3.33.0/build-arm-abi/include

EXTERNLIBS = $(LIBCEDARX_DIR)/libcore/parser/base/libcdx_parser.la \
			$(LIBCEDARX_DIR)/libcore/base/libcdx_base.la \
			$(LIBCEDARX_DIR)/libcore/stream/base/libcdx_stream.la \
			$(LIBCEDARX_DIR)/libcore/common/libcdx_common.la

LDFLAGS ?= -Wl,--no-undefined \
			-L/home/yuzang/linux/tools/ffmpeg-7.0.2/arm-myir-install/lib \
			-L/home/yuzang/linux/tools/sqlite-version-3.33.0/build-arm-abi/lib \
			-L$(LIBCEDARX_DIR)/external/lib32/arm-linux-gnueabi \
			-L$(LIBCEDARC_DIR)/library/arm-linux-gnueabi/ \
			-L$(LIBCEDARC_DIR)/install/lib \
			-L$(LIBCEDARX_DIR)/install/lib \
			-L/home/yuzang/linux/linux/auto-t113x-linux/platform/framework/auto/sdk_lib/libs \
			-lMemAdapter -lVE -lcdc_base -lvdecoder  -lcdx_base \
			-lcdx_parser -lcdx_stream -lcdx_common -lsdk_g2d \
			-lvideoengine -lfbm -lsbm -laftertreatment -lscaledown \
			-lcrypto -lz -lavcodec -lavformat -lavutil -lswresample \
			-lswscale -lm -lpthread -lcdx_ion -lsdk_memory \
			-llive555 -lsqlite3

EXTERNAL_A = $(EXTERNAL_LIB_DIR)/libmc.a \
				$(EXTERNAL_LIB_DIR)/libs7.a

BIN = main_test


#Collect the files to compile
MAINSRC = ./main.cpp

include $(LVGL_DIR)/lvgl/lvgl.mk
include $(LVGL_DIR)/lv_drivers/lv_drivers.mk
include $(LVGL_DIR)/lv_demos/lv_demo.mk

#CSRCS +=$(LVGL_DIR)/mouse_cursor_icon.c 
CSRCS += $(LVGL_DIR)/demoVdecoder.c \
			$(LVGL_DIR)/green_point.c \
			$(LVGL_DIR)/red_point.c \
			$(LVGL_DIR)/column_30x30.c \
			$(LVGL_DIR)/battery_4.c \
			$(LVGL_DIR)/lv_font_cn_songti_bold.c \
			$(LVGL_DIR)/lv_font_cn_songti_bold_25.c \
			${LVGL_DIR}/lv_font_cn_songti_medium_21.c \
			${LVGL_DIR}/lv_font_cn_songti_medium_21_media_cfg.c \
			$(LVGL_DIR)/loading_200x200.c \
			$(LVGL_DIR)/no_camera_200x200.c \
			$(LVGL_DIR)/down_20x20.c \
			$(LVGL_DIR)/off_20x20.c \
			$(LVGL_DIR)/lv_font_cn_songti_medium_21_msg.c
CPPSRCS += $(LVGL_DIR)/lv_page0.cpp \
			$(LVGL_DIR)/lv_page1.cpp \
			$(LVGL_DIR)/lv_page2.cpp \
			$(LVGL_DIR)/lv_page3.cpp \
			$(LVGL_DIR)/lv_page4.cpp \
			$(LVGL_DIR)/lv_page5.cpp \
			$(LVGL_DIR)/lv_UITopPage.cpp \
			$(LVGL_DIR)/lv_message_window.cpp \
			$(LVGL_DIR)/plccore.cpp \
			$(LVGL_DIR)/task_manager.cpp \
			$(LVGL_DIR)/camera_helper.cpp \
			$(LVGL_DIR)/cJSON.cpp \
			$(LVGL_DIR)/network.cpp \
			$(LVGL_DIR)/utils.cpp \
			$(LVGL_DIR)/myregex.cpp \
			$(LVGL_DIR)/serial.cpp \
			$(LVGL_DIR)/keyboard.cpp \
			$(LVGL_DIR)/plc_protocol.cpp \
			$(LVGL_DIR)/plc.cpp \
			$(LVGL_DIR)/database.cpp \
			$(LVGL_DIR)/access.cpp

OBJEXT ?= .o

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))
CPPOBJS = $(CPPSRCS:.cpp=$(OBJEXT))

MAINOBJ = $(MAINSRC:.cpp=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS) $(MAINSRC) $(CPPSRCS)
OBJS = $(AOBJS) $(COBJS) $(CPPOBJS)

## MAINOBJ -> OBJFILES

all: default

%.o: %.c
	@$(CC)  $(CFLAGS) -c $< -o $@
	@echo "CC $<"

%.o: %.cpp
	@$(GCC) $(CPPFLAGS) -c $< -o $@
	@echo "GCC $<"

$(EXTERNAL_A):
	$(MAKE) -C ./external
    
default: $(AOBJS) $(COBJS) ${CPPOBJS} $(MAINOBJ) $(EXTERNAL_A)
	$(GCC) -o $(BIN) $(MAINOBJ) $(AOBJS) $(COBJS) $(CPPOBJS) $(EXTERNAL_A) $(LDFLAGS)

clean: 
	rm -f $(BIN) $(AOBJS) $(COBJS) $(CPPOBJS) $(MAINOBJ) $(EXTERNAL_A)
	$(MAKE) clean -C ./external
