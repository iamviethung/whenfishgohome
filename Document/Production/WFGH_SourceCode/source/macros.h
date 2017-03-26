#ifndef _MACROS_H_
#define _MACROS_H_

// COMMON DEFINED
#define NULL 0

#define _GAME_DEBUG		//enable debug mode

#ifdef _GAME_DEBUG
#define LOG_TAG "GAME"

#ifdef _ANDROID
#include <android/log.h>
#include <assert.h>
#  define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#  define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#  define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#include <stdio.h>
#  define QUOTEME_(x) #x
#  define QUOTEME(x) QUOTEME_(x)
#  define LOGI(...) printf("I/" LOG_TAG " (" __FILE__ ":" QUOTEME(__LINE__) "): " __VA_ARGS__)
#  define LOGE(...) printf("E/" LOG_TAG "(" ")" __VA_ARGS__)
#  define LOGV(...) printf(__VA_ARGS__)
#endif

#else

#define LOGI(...)
#define LOGE(...)

#endif

//define
#define GL_CHECK(x) \
        x; \
        { \
          GLenum glError = glGetError(); \
          if(glError != GL_NO_ERROR) { \
            LOGI("glGetError() = %i (0x%.8x) at line %i\n", glError, glError, __LINE__); \
            exit(1); \
          } \
        }

#define EGL_CHECK(x) \
    x; \
    { \
        EGLint eglError = eglGetError(); \
        if(eglError != EGL_SUCCESS) { \
            LOGI("eglGetError() = %i (0x%.8x) at line %i\n", eglError, eglError, __LINE__); \
            exit(1); \
        } \
    }		

#ifdef _WIN32
	#define ASSERT_MSG(condition, message) \
		{ \
			if (!condition) \
			{ \
				char buff[BUFFER_BIG_SIZE]; \
				sprintf(buff, "%s\n%s: %i", message, __FILE__, __LINE__); \
				MessageBox(NULL, buff, NULL, MB_OK | MB_ICONEXCLAMATION); \
				exit(1); \
			} \
		}

	#define ASSERT(condition) ASSERT_MSG(condition, "")
#else
	#define ASSERT_MSG(condition, message) \
		do { \
		if (!condition) \
			{ \
			char buff[BUFFER_BIG_SIZE]; \
			sprintf(buff, "%s\n%s: %i", message, __FILE__, __LINE__); \
			LOGI("%s", buff); \
			assert(0); \
			} \
		} while (0)

	#define ASSERT(condition) ASSERT_MSG(condition, "")
#endif

// Safe delete
#define SAFE_DEL(x)					\
{									\
if (x)							\
	{								\
	delete x;					\
	x = NULL;						\
	}								\
}

#define SAFE_DEL_ARR(x) \
{									\
if (x)							\
	{								\
	delete[] x;					\
	x = NULL;						\
	}								\
}
#endif