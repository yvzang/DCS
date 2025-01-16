
#只生成 .d,.o即可 
MC_LIB = $(LIBDIR)/libmc.a #这个可以保证生成nginx可执行文件
INCLUDE_PATH = $(shell pwd)
LIBARARY_LINK ?= ar -rc

default: $(MC_LIB)

include $(BUILD_ROOT)/common.mk

	
$(MC_LIB): $(OBJS)
	$(LIBARARY_LINK) $@ $^

