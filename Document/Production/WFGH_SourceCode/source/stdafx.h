// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef _STDAFX_H_
#define _STDAFX_H_

#pragma warning( disable : 4996 )

#include <stdio.h>
#include <string.h>
#include <string>

#ifdef _WIN32
// ADDITIONAL LIB
#pragma comment(lib, "libEGL.lib")
#pragma comment(lib, "libGLESv2.lib")
#pragma comment(lib, "SOIL.lib")

#include <windows.h>
#include "GLES2/gl2.h"
#include "EGL/egl.h"

// Check memory leak with VLD
//#define USE_VLD
#ifdef USE_VLD
#include "vld.h"
#pragma comment(lib, "vld.lib")
#endif

#else
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#endif

#include "game_config.h"
#include "macros.h"

#include "Utils/Math3D.h"
#include "Video/Program.h"
#include "Video/Texture.h"

#endif