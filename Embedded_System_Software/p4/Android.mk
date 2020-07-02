LOCAL_PATH:=$(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE:=puzzle-game
LOCAL_SRC_FILES:=fpga.c
LOCAL_LDLIBS := -llog
#LOCAL_LDLIB := -L$(SYSROOT)/usr/lib -llog

include $(BUILD_SHARED_LIBRARY)

