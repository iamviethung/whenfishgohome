#pragma once

#include "Platform.h"

class PlatformAndroid : public Platform
{
protected:
	int			m_iWidth;
	int			m_iHeight;
	bool		m_isInit;
	
public:
	PlatformAndroid();
	~PlatformAndroid();

	void		Init(int width, int height);
	void		Update();				//use for key map if has
	void		SwapBuffers();			//NOT USED ON ANDROID
	void		Resize(int width, int height);
	void		Exit();

	void		printSystemSpecs();
	virtual		std::string getFullPath(const char* path);
};