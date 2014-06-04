LOCAL_PATH := $(call my-dir)

#main application
include $(CLEAR_VARS)

LOCAL_MODULE    := native-zeptotest
LOCAL_C_INCLUDES += "$(LOCAL_PATH)/../libs/libpng"
MY_LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/*.cpp) \
					  $(wildcard $(LOCAL_PATH)/elements/*.cpp) \
					  $(wildcard $(LOCAL_PATH)/graphics/*.cpp) \
					  $(wildcard $(LOCAL_PATH)/physics/*.cpp) \
					  $(wildcard $(LOCAL_PATH)/subsystems/*.cpp) \
					  $(wildcard $(LOCAL_PATH)/utils/*.cpp)

LOCAL_SRC_FILES := $(subst jni/application/, , $(MY_LOCAL_SRC_FILES))
				
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2
LOCAL_STATIC_LIBRARIES := android_native_app_glue libpng

#LOCAL_ALLOW_UNDEFINED_SYMBOLS := true

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)