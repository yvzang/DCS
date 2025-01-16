export BUILD_ROOT = $(shell pwd)
export LIBDIR = $(BUILD_ROOT)/libs
export LINK_OBJ_DIR = $(BUILD_ROOT)/objs

$(shell mkdir -p $(LIBDIR))
$(shell mkdir -p $(LINK_OBJ_DIR))

SUB_DIR = melsec_mc_net siemens_plc_s7_net


all:
	@for dir in $(SUB_DIR); \
	do \
		$(MAKE) -C $$dir; \
	done

clean:
	rm -r $(BUILD_ROOT)/objs $(BUILD_ROOT)/libs