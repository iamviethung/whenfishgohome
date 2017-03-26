LOCAL_PATH := $(call my-dir)

# Build support lib
LOCAL_CFLAGS := -DOS_ANDROID \
				-D_ANDROID
           
LOCAL_MODULE := libsoil
LOCAL_MODULE_FILENAME := libsoil

LOCAL_CFLAGS	:= -mandroid \
					-ffunction-sections \
					-funwind-tables \
					-fstack-protector \
					-fpic \
					-Wno-psabi \
					-mfpu=vfpv3-d16 \
					-mfloat-abi=softfp \
					-Wno-write-strings \
					-fsigned-char \
					-fomit-frame-pointer \
					-fno-strict-aliasing \
					-finline-limit=300 \
					-ffast-math \
					-pipe \
					-g \
					-w -O3 \
					-fno-rtti \
                    -fexceptions

LOCAL_SRC_FILES :=	..\..\..\lib\soil\src\image_DXT.c \
					..\..\..\lib\soil\src\image_helper.c \
					..\..\..\lib\soil\src\stb_image_aug.c \
					..\..\..\lib\soil\src\original\stb_image-1.09.c \
					..\..\..\lib\soil\src\original\stb_image-1.16.c			

include $(BUILD_STATIC_LIBRARY)

# Build main lib
include $(CLEAR_VARS)

LOCAL_CFLAGS := -DOS_ANDROID \
				-D_ANDROID \
				-UNDEBUG 
           
LOCAL_MODULE := libtrainingfw
LOCAL_MODULE_FILENAME := libtrainingfw

LOCAL_LDLIBS	:= -llog -lGLESv2 -lEGL -landroid
LOCAL_CFLAGS	:= 	-mandroid \
					-ffunction-sections \
					-funwind-tables \
					-fstack-protector \
					-fpic \
					-Wno-psabi \
					-mfpu=vfpv3-d16 \
					-mfloat-abi=softfp \
					-Wno-write-strings \
					-fsigned-char \
					-fomit-frame-pointer \
					-fno-strict-aliasing \
					-finline-limit=300 \
					-ffast-math \
					-pipe \
					-g \
					-w -O3 \
					-fno-rtti \
                    -fexceptions

LOCAL_LDFLAGS	:= -Wl,--allow-multiple-definition

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../source \
					$(LOCAL_PATH)/../../../lib/soil/src

LOCAL_SRC_FILES :=	main.cpp \
					../../../source/Game.cpp \
					../../../source/Platform/InputManager.cpp \
					../../../source/Platform/PlatformAndroid.cpp \
					../../../source/Utils/FPSLimiter.cpp \
					../../../source/Utils/Math3D.cpp \
					../../../source/Video/Program.cpp \
					../../../source/WhenFishGoHome/Button.cpp \
					../../../source/WhenFishGoHome/Camera.cpp \
					../../../source/WhenFishGoHome/Fish.cpp \
					../../../source/WhenFishGoHome/GameManager.cpp \
					../../../source/WhenFishGoHome/JumpPointSearch.cpp \
					../../../source/WhenFishGoHome/Map.cpp \
					../../../source/WhenFishGoHome/MenuManager.cpp \
					../../../source/WhenFishGoHome/MiniMap.cpp \
					../../../source/WhenFishGoHome/Object.cpp \
					../../../source/WhenFishGoHome/Item.cpp \
					../../../source/WhenFishGoHome/Player.cpp \
					../../../source/WhenFishGoHome/Processor.cpp \
					../../../source/WhenFishGoHome/ResourceManager.cpp \
					../../../source/WhenFishGoHome/SceneManager.cpp \
					../../../source/WhenFishGoHome/Shark.cpp \
					../../../source/WhenFishGoHome/WaterCaustic.cpp \
					../../../source/WhenFishGoHome/TGA.cpp

LOCAL_WHOLE_STATIC_LIBRARIES := soil

include $(BUILD_SHARED_LIBRARY)
