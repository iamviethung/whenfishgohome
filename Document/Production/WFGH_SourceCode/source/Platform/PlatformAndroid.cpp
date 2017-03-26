#include "stdafx.h"
#include "PlatformAndroid.h"

#ifdef _ANDROID
#include <GLES2/gl2.h>

PlatformAndroid::PlatformAndroid() : Platform()
{

}

PlatformAndroid::~PlatformAndroid()
{
}

void PlatformAndroid::Init(int width, int height)
{
	LOGI("Platform Init: %d - %d", width, height);
	m_iWidth = width;
	m_iHeight = height;

	//set viewport
	glViewport(0, 0, width, height);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	m_isInit = true;
}

void PlatformAndroid::Update()
{

}

void PlatformAndroid::SwapBuffers()
{

}
void PlatformAndroid::Resize(int width, int height)
{
	if (m_isInit)
	{
		LOGI("Platform Resize: %d - %d", width, height);

		m_iWidth = width;
		m_iHeight = height;

		//set viewport
		glViewport(0, 0, width, height);
	}	
}

void PlatformAndroid::Exit()
{

}
	 
void PlatformAndroid::printSystemSpecs()
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

std::string PlatformAndroid::getFullPath(const char* path)
{
	std::string str_path = ANDROID_PREFIX;
	str_path += path;
	return str_path;
}
#endif