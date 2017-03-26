#include "stdafx.h"
#include "PlatformWin32.h"
#include "InputManager.h"

#include <windowsx.h>

#include "Game.h"

#ifdef USE_POINTER_INPUT
//an anchor to screen
static int g_anchorX = 0, g_anchorY = 0;

#define WIN_CURSOR_OFFSET 200
#define WIN_BORDER_W 8
#define WIN_BORDER_H 30
#endif

/*!*****************************************************************************************************************************************
@Function		TestEGLError
@Input			nativeWindow                Handle to the window
@Input			functionLastCalled          Function which triggered the error
@Return		True if no EGL error was detected
@Description	Tests for an EGL error and prints it in a message box.
*******************************************************************************************************************************************/
bool TestEGLError(HWND nativeWindow, const char* functionLastCalled)
{
	/*	eglGetError returns the last error that occurred using EGL, not necessarily the status of the last called function. The user has to
	check after every single EGL call or at least once every frame. Usually this would be for debugging only, but for this example
	it is enabled always.
	*/
	EGLint lastError = eglGetError();
	if (lastError != EGL_SUCCESS)
	{
		TCHAR stringBuffer[256];
		sprintf(stringBuffer, ("%s failed (%x).\n"), functionLastCalled, lastError);
		MessageBox(nativeWindow, stringBuffer, "Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	return true;
}

/*!*****************************************************************************************************************************************
@Function		TestGLError
@Input			nativeWindow                Handle to the window
@Input			functionLastCalled          Function which triggered the error
@Return		True if no EGL error was detected
@Description	Tests for an EGL error and prints it in a message box.
*******************************************************************************************************************************************/
bool TestGLError(HWND nativeWindow, const char* functionLastCalled)
{

	/*	glGetError returns the last error that occurred using OpenGL ES, not necessarily the status of the last called function. The user
	has to check after every single OpenGL ES call or at least once every frame. Usually this would be for debugging only, but for this
	example it is enabled always
	*/
	GLenum lastError = glGetError();
	if (lastError != GL_NO_ERROR)
	{
		TCHAR stringBuffer[256];
		sprintf(stringBuffer, ("%s failed (%x).\n"), functionLastCalled, lastError);
		MessageBox(nativeWindow, stringBuffer, "Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	return true;
}

HWND g_hWnd = 0;

PlatformWin32::PlatformWin32(): Platform(),// Windows variables
								nativeWindow(NULL),
								deviceContext(NULL),
								eglDisplay(NULL),
								eglConfig(NULL),
								eglSurface(NULL),
								eglContext(NULL)
{
}

PlatformWin32::~PlatformWin32()
{
	if (!m_isInit)
		return;
}

void PlatformWin32::Exit()
{
	if (!m_isInit)
		return;

	eglTerminate(eglDisplay);

	// Release the device context.
	if (deviceContext)
	{
		ReleaseDC(nativeWindow, deviceContext);
	}

	// Destroy the window
	if (nativeWindow)
	{
		DestroyWindow(nativeWindow);
	}
}

void PlatformWin32::Init(int width, int height)
{
	m_iWidth = width;
	m_iHeight = height;

	// Setup the windowing system, getting a window and a display
	if (!CreateWindowAndDisplay(width, height, nativeWindow, deviceContext))
	{
		ASSERT(false);
	}

	// Create and Initialise an EGLDisplay from the native display
	if (!CreateEGLDisplay(deviceContext, eglDisplay))
	{
		ASSERT(false);
	}

	// Choose an EGLConfig for the application, used when setting up the rendering surface and EGLContext
	if (!ChooseEGLConfig(eglDisplay, eglConfig))
	{
		ASSERT(false);
	}

	// Create an EGLSurface for rendering from the native window
	if (!CreateEGLSurface(nativeWindow, eglDisplay, eglConfig, eglSurface))
	{
		ASSERT(false);
	}

	// Setup the EGL Context from the other EGL constructs created so far, so that the application is ready to submit OpenGL ES commands
	if (!SetupEGLContext(eglDisplay, eglConfig, eglSurface, eglContext, nativeWindow))
	{
		ASSERT(false);
	}

	//set viewport
	glViewport(0, 0, width, height);

	glClearColor ( 1.0f, 1.0f, 1.0f, 1.0f );

	ShowCursor(true);
	m_isInit = true;
}

void PlatformWin32::Update()
{	
	if(PeekMessage(&sMessage, NULL, 0, 0, PM_REMOVE)) {
            if(sMessage.message == WM_QUIT) {						
            } else {
                TranslateMessage(&sMessage);
                DispatchMessage(&sMessage);
            }
        }
}

void PlatformWin32::SwapBuffers()
{
	eglSwapBuffers(eglDisplay, eglSurface);
}

void PlatformWin32::Resize(int width, int height)
{
	if (m_isInit)
	{
		m_iWidth = width;
		m_iHeight = height;

		//set viewport
		glViewport(0, 0, width, height);
	}
}

void PlatformWin32::printSystemSpecs()
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* gl_version = glGetString(GL_VERSION);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* ext = glGetString(GL_EXTENSIONS);

	LOGV("Vendor: %s\n", vendor);
	LOGV("Renderer: %s\n", renderer);
    LOGV("OpenGL version supported %s\n", gl_version);
	LOGV("Extensions: \n%s\n", ext);
}

/*
 * process_window(): This function handles Windows callbacks.
 */
LRESULT CALLBACK PlatformWin32::ProcessWindow(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT  lRet = 1;

	switch(uiMsg) {
		case WM_CLOSE:
				PostMessage(hWnd, WM_QUIT, 0, 0);
				return 0;

		case WM_ACTIVATE:
			{
			}
			break;
		case WM_KEYDOWN:
			{
				INPUT_MGR->setKey(E_KEY_PRESSED, wParam);
			}
			break;
		case WM_KEYUP:
			{
				INPUT_MGR->setKey(E_KEY_RELEASED, wParam);
			}
			break;
		case WM_SIZE: 
			{				
				RECT rect;
				if(GetWindowRect(hWnd, &rect))
				{
					int width = rect.right - rect.left;
					int height = rect.bottom - rect.top;

					PLATFORM->Resize(width, height);
				}
			}
			break;
		case WM_LBUTTONDOWN:
			{
				int xPos = GET_X_LPARAM(lParam);
				int yPos = GET_Y_LPARAM(lParam);

				INPUT_MGR->updatePointerEvent(E_POINTER_PRESSSED, xPos, yPos);
			}
			break;
		case WM_LBUTTONUP:
			{
				int xPos = GET_X_LPARAM(lParam);
				int yPos = GET_Y_LPARAM(lParam);

				INPUT_MGR->updatePointerEvent(E_POINTER_RELEASED, xPos, yPos);
			}
			break;
		case WM_MOUSEMOVE:
			{
				int xPos = GET_X_LPARAM(lParam);
				int yPos = GET_Y_LPARAM(lParam);

				INPUT_MGR->updatePointerEvent(E_POINTER_MOVED, xPos, yPos);
			}
			break;
		default:
			lRet = DefWindowProc(hWnd, uiMsg, wParam, lParam);
			break;
	}

	return lRet;
}

/*!*****************************************************************************************************************************************
@Function		CreateWindowAndDisplay
@Input			applicationInstance		    Specific instance of the application
@Output		nativeWindow			    Native window type to create
@Output		deviceContext			    Native device context to create
@Return		Whether the function succeeded or not.
@Description	Creates a native window and display for the application to render into.
*******************************************************************************************************************************************/
bool PlatformWin32::CreateWindowAndDisplay(int width, int height, HWND &nativeWindow, HDC &deviceContext)
{
	// Describe the native window in a window class structure
	WNDCLASS nativeWindowDescription;
	nativeWindowDescription.style = CS_HREDRAW | CS_VREDRAW;
	nativeWindowDescription.lpfnWndProc = (WNDPROC) ProcessWindow;
	nativeWindowDescription.cbClsExtra = 0;
	nativeWindowDescription.cbWndExtra = 0;
	nativeWindowDescription.hInstance = NULL;
	nativeWindowDescription.hIcon = 0;
	nativeWindowDescription.hCursor = 0;
	nativeWindowDescription.lpszMenuName = 0;
	nativeWindowDescription.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	nativeWindowDescription.lpszClassName = "OGLES";

	// Register the windows class with the OS.
	ATOM registerClass = RegisterClass(&nativeWindowDescription);
	if (!registerClass)
	{
		ASSERT_MSG(false, "Failed to register the window class");
	}

	// Create a rectangle describing the area of the window
	RECT windowRectangle;
	SetRect(&windowRectangle, 0, 0, width, height);
	AdjustWindowRectEx(&windowRectangle, WS_CAPTION | WS_SYSMENU, false, 0);

	// Create the window from the available information
	nativeWindow = CreateWindow("OGLES", "Game", WS_VISIBLE | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT,
		windowRectangle.right - windowRectangle.left, windowRectangle.bottom - windowRectangle.top,
		NULL, NULL, NULL, NULL);
	if (!nativeWindow)
	{
		LOGI("Failed to create the window");
		return false;
	}

	// Get the associated device context from the window
	deviceContext = GetDC(nativeWindow);
	if (!deviceContext)
	{
		LOGI("Failed to create the device context");
		return false;
	}

	g_hWnd = nativeWindow;

	return true;
}

/*!*****************************************************************************************************************************************
@Function		CreateEGLDisplay
@Input			deviceContext               The device context used by the application
@Output		eglDisplay				    EGLDisplay created from deviceContext
@Return		Whether the function succeeded or not.
@Description	Creates an EGLDisplay from a native device context, and initialises it.
*******************************************************************************************************************************************/
bool PlatformWin32::CreateEGLDisplay(HDC deviceContext, EGLDisplay &eglDisplay)
{
	/*	Get an EGL display.
	EGL uses the concept of a "display" which in most environments corresponds to a single physical screen. After creating a native
	display for a given windowing system, EGL can use this handle to get a corresponding EGLDisplay handle to it for use in rendering.
	Should this fail, EGL is usually able to provide access to a default display.
	*/
	eglDisplay = eglGetDisplay(deviceContext);
	if (eglDisplay == EGL_NO_DISPLAY)
	{
		eglDisplay = eglGetDisplay((EGLNativeDisplayType)EGL_DEFAULT_DISPLAY);
	}

	// If a display still couldn't be obtained, return an error.
	if (eglDisplay == EGL_NO_DISPLAY)
	{
		LOGI("Failed to get an EGLDisplay");
		return false;
	}

	/*	Initialize EGL.
	EGL has to be initialized with the display obtained in the previous step. All EGL functions other than eglGetDisplay
	and eglGetError need an initialised EGLDisplay.
	If an application is not interested in the EGL version number it can just pass NULL for the second and third parameters, but they
	are queried here for illustration purposes.
	*/
	EGLint eglMajorVersion, eglMinorVersion;
	if (!eglInitialize(eglDisplay, &eglMajorVersion, &eglMinorVersion))
	{
		LOGI("Failed to initialise the EGLDisplay");
		return false;
	}

	return true;
}

/*!*****************************************************************************************************************************************
@Function		ChooseEGLConfig
@Input			eglDisplay                  The EGLDisplay used by the application
@Output		eglConfig                   The EGLConfig chosen by the function
@Return		Whether the function succeeded or not.
@Description	Chooses an appropriate EGLConfig and return it.
*******************************************************************************************************************************************/
bool PlatformWin32::ChooseEGLConfig(EGLDisplay eglDisplay, EGLConfig& eglConfig)
{
	/*	Specify the required configuration attributes.
	An EGL "configuration" describes the capabilities an application requires and the type of surfaces that can be used for drawing.
	Each implementation exposes a number of different configurations, and an application needs to describe to EGL what capabilities it
	requires so that an appropriate one can be chosen. The first step in doing this is to create an attribute list, which is an array
	of key/value pairs which describe particular capabilities requested. In this application nothing special is required so we can query
	the minimum of needing it to render to a window, and being OpenGL ES 2.0 capable.
	*/
	const EGLint configurationAttributes[] =
	{
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_DEPTH_SIZE, 8,
		EGL_STENCIL_SIZE, 8,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_NONE
	};

	/*	Find a suitable EGLConfig
	eglChooseConfig is provided by EGL to provide an easy way to select an appropriate configuration. It takes in the capabilities
	specified in the attribute list, and returns a list of available configurations that match or exceed the capabilities requested.
	Details of all the possible attributes and how they are selected for by this function are available in the EGL reference pages here:
	http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglChooseConfig.html
	It is also possible to simply get the entire list of configurations and use a custom algorithm to choose a suitable one, as many
	advanced applications choose to do. For this application however, taking the first EGLConfig that the function returns suits
	its needs perfectly, so we limit it to returning a single EGLConfig.
	*/
	EGLint configsReturned;
	if (!eglChooseConfig(eglDisplay, configurationAttributes, &eglConfig, 1, &configsReturned) || (configsReturned != 1))
	{
		LOGI("eglChooseConfig() failed.");
		return false;
	}

	return true;
}


/*!*****************************************************************************************************************************************
@Function		CreateEGLSurface
@Input			nativeWindow                A native window that's been created
@Input			eglDisplay                  The EGLDisplay used by the application
@Input			eglConfig                   An EGLConfig chosen by the application
@Output		eglSurface					The EGLSurface created from the native window.
@Return		Whether the function succeeds or not.
@Description	Creates an EGLSurface from a native window
*******************************************************************************************************************************************/
bool PlatformWin32::CreateEGLSurface(HWND nativeWindow, EGLDisplay eglDisplay, EGLConfig eglConfig, EGLSurface& eglSurface)
{
	/*	Create an EGLSurface for rendering.
	Using a native window created earlier and a suitable eglConfig, a surface is created that can be used to render OpenGL ES calls to.
	There are three main surface types in EGL, which can all be used in the same way once created but work slightly differently:
	- Window Surfaces  - These are created from a native window and are drawn to the screen.
	- Pixmap Surfaces  - These are created from a native windowing system as well, but are offscreen and are not displayed to the user.
	- PBuffer Surfaces - These are created directly within EGL, and like Pixmap Surfaces are offscreen and thus not displayed.
	The offscreen surfaces are useful for non-rendering contexts and in certain other scenarios, but for most applications the main
	surface used will be a window surface as performed below.
	*/
	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, nativeWindow, NULL);
	if (eglSurface == EGL_NO_SURFACE)
	{
		eglGetError(); // Clear error
		eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, NULL, NULL);
	}

	// Check for any EGL Errors
	if (!TestEGLError(nativeWindow, "eglCreateWindowSurface"))
	{
		return false;
	}

	return true;
}

/*!*****************************************************************************************************************************************
@Function		SetupEGLContext
@Input			eglDisplay                  The EGLDisplay used by the application
@Input			eglConfig                   An EGLConfig chosen by the application
@Input			eglSurface					The EGLSurface created from the native window.
@Output		eglContext                  The EGLContext created by this function
@Input			nativeWindow                A native window, used to display error messages
@Return		Whether the function succeeds or not.
@Description	Sets up the EGLContext, creating it and then installing it to the current thread.
*******************************************************************************************************************************************/
bool PlatformWin32::SetupEGLContext(EGLDisplay eglDisplay, EGLConfig eglConfig, EGLSurface eglSurface, EGLContext& eglContext, HWND nativeWindow)
{
	/*	Create a context.
	EGL has to create what is known as a context for OpenGL ES. The concept of a context is OpenGL ES's way of encapsulating any
	resources and state. What appear to be "global" functions in OpenGL actually only operate on the current context. A context
	is required for any operations in OpenGL ES.
	Similar to an EGLConfig, a context takes in a list of attributes specifying some of its capabilities. However in most cases this
	is limited to just requiring the version of the OpenGL ES context required - In this case, OpenGL ES 2.0.
	*/
	EGLint contextAttributes[] =
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	// Create the context with the context attributes supplied
	eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, contextAttributes);
	if (!TestEGLError(nativeWindow, "eglCreateContext"))
	{
		return false;
	}

	/*	Make OpenGL ES the current API.
	After creating the context, EGL needs a way to know that any subsequent EGL calls are going to be affecting OpenGL ES,
	rather than any other API (such as OpenVG).
	*/
	eglBindAPI(EGL_OPENGL_ES_API);
	if (!TestEGLError(nativeWindow, "eglBindAPI"))
	{
		return false;
	}

	/*	Bind the context to the current thread.
	Due to the way OpenGL uses global functions, contexts need to be made current so that any function call can operate on the correct
	context. Specifically, make current will bind the context to the thread it's called from, and unbind it from any others. To use
	multiple contexts at the same time, users should use multiple threads and synchronise between them.
	*/
	eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
	if (!TestEGLError(nativeWindow, "eglMakeCurrent"))
	{
		return false;
	}

	return true;
}

RECT PlatformWin32::GetLocalCoordinates(HWND hWnd)
{
	RECT Rect;
	GetWindowRect(hWnd, &Rect);
	MapWindowPoints(HWND_DESKTOP, GetParent(hWnd), (LPPOINT)&Rect, 2);
	return Rect;
}

std::string PlatformWin32::getFullPath(const char* path)
{
	return path;
}