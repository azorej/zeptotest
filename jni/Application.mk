APP_PLATFORM := android-19
APP_ABI := armeabi

# use this to select gcc instead of clang
#NDK_TOOLCHAIN_VERSION := 4.8
# OR use this to select the latest clang version:
NDK_TOOLCHAIN_VERSION := clang

#APP_STL := gnustl_static
APP_STL := c++_static
#LIBCXX_FORCE_REBUILD := true

# then enable c++11 extentions in source code
#LOCAL_CPPFLAGS := -std=gnu++11
LOCAL_CPPFLAGS += -std=c++11

APP_CPPFLAGS += -fexceptions
APP_CPPFLAGS += -frtti

APP_CPPFLAGS += -DANDROID
APP_CPPFLAGS += -D__ANDROID__