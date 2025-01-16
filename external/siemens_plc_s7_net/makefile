
#只生成 .d,.o即可 
S7_LIB = $(LIBDIR)/libs7.a #这个可以保证生成nginx可执行文件
INCLUDE_PATH = $(shell pwd)
LIBARARY_LINK ?= ar -rc

default: $(S7_LIB)

include $(BUILD_ROOT)/common.mk

	
$(S7_LIB): $(OBJS)
	$(LIBARARY_LINK) $@ $^

