#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "../../../source/stdafx.h"
#include "../../../source/Game.h"
#include "../../../source/Platform/PlatformAndroid.h"
#include "../../../source/Platform/InputManager.h"
#include "../../../source/Utils/FPSLimiter.h"


#define JNI_FUNCTION(function) 		 Java_com_android_trainingfw_ ## function

Game* game = NULL;

void GameInit(int width, int height)
{
	LOGI("INIT GAME # %d - %d", width, height);
	
	//init singleton
	game = new Game();
	new PlatformAndroid();
	
	game->Init(width, height);
	game->setRunning();
	
	LOGI("INIT GAME - DONE");
}

void GameRender()
{	
	game->Update(FPS_LMT->getDeltaTime());
	game->Render();
}

extern "C" {
    JNIEXPORT void JNICALL JNI_FUNCTION(GameRenderer_nativeInit) (JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL JNI_FUNCTION(GameRenderer_nativeRender) (JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL JNI_FUNCTION(GameRenderer_nativeResize) (JNIEnv * env, jobject obj,  jint width, jint height);
	JNIEXPORT void JNICALL JNI_FUNCTION(GameSurfaceView_nativeOnTouch) (JNIEnv * env, jobject obj,  jfloat x, jfloat y, jint action);
};

JNIEXPORT void JNICALL JNI_FUNCTION(GameRenderer_nativeInit) (JNIEnv * env, jobject obj,  jint width, jint height)
{
    GameInit(width, height);
}

JNIEXPORT void JNICALL JNI_FUNCTION(GameRenderer_nativeRender) (JNIEnv * env, jobject obj)
{
    GameRender();
}

JNIEXPORT void JNICALL JNI_FUNCTION(GameRenderer_nativeResize) (JNIEnv * env, jobject obj,  jint width, jint height)
{
	LOGI("Resize # %d - %d", width, height);
	PLATFORM->Resize(width, height);
}

JNIEXPORT void JNICALL JNI_FUNCTION(GameSurfaceView_nativeOnTouch) (JNIEnv * env, jobject obj,  jfloat x, jfloat y, jint action)
{	
	INPUT_MGR->updatePointerEvent((PointerState) action, (short) x, (short) y);
}
