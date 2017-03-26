APP_STL := gnustl_static 
APP_ABI := armeabi-v7a
APP_CPPFLAGS := -frtti
APP_CPPFLAGS += -fexceptions \
				-DOS_ANDROID \
				-D_ANDROID
NDK_TOOLCHAIN_VERSION := 4.8