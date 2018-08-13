LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE	:= aaudioio
LOCAL_LDLIBS	:= -llog -laaudio
#LOCAL_CFLAGS	+= -D__platform_android__=1
LOCAL_CFLAGS	+= -D__platform_android__=1 -DDEBUG
LOCAL_SRC_FILES	:= mutex.cpp audioio_jni.cpp audioio.cpp audioin.cpp audioout.cpp

include $(BUILD_SHARED_LIBRARY)

