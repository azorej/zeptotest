LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)


LOCAL_MODULE    := libpng
LOCAL_C_INCLUDES += $(LOCAL_PATH)
MY_LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/*.c)
LOCAL_SRC_FILES := $(subst jni/libs/libpng/, , $(MY_LOCAL_SRC_FILES))

#$(filter-out example.c pngtest.c,$(call LS_C,$(LOCAL_PATH)))

LOCAL_EXPORT_LDLIBS := -lz
				

include $(BUILD_STATIC_LIBRARY)

