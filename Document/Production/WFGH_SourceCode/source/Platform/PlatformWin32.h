#pragma once

#include "Platform.h"

class PlatformWin32 : public Platform
{
	//config for win32
	MSG sMessage;

	// Windows variables
	HWND				nativeWindow;
	HDC					deviceContext;

	// EGL variables
	EGLDisplay			eglDisplay;
	EGLConfig			eglConfig;
	EGLSurface			eglSurface;
	EGLContext			eglContext;

public:
	PlatformWin32();
	~PlatformWin32();

	virtual void		Init(int width, int height);
	virtual void		Update();				//use for key map if has
	virtual void		SwapBuffers();			//this function depend on which platform using
	virtual void		Resize(int width, int height);
	virtual void		Exit();

	virtual void		printSystemSpecs();
	virtual std::string getFullPath(const char* path);

	//win32 function
	static LRESULT CALLBACK ProcessWindow(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
	static RECT GetLocalCoordinates(HWND hWnd);

	static bool CreateWindowAndDisplay(int width, int height, HWND &nativeWindow, HDC &deviceContext);
	static bool CreateEGLDisplay(HDC deviceContext, EGLDisplay &eglDisplay);
	static bool ChooseEGLConfig(EGLDisplay eglDisplay, EGLConfig& eglConfig);
	static bool CreateEGLSurface(HWND nativeWindow, EGLDisplay eglDisplay, EGLConfig eglConfig, EGLSurface& eglSurface);
	static bool SetupEGLContext(EGLDisplay eglDisplay, EGLConfig eglConfig, EGLSurface eglSurface, EGLContext& eglContext, HWND nativeWindow);
};