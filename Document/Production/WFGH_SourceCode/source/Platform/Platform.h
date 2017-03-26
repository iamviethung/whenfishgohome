#ifndef _PLATFORM_H
#define _PLATFORM_H

#include "Utils/Singleton.h"

#include <stdio.h>

class Platform : public Singleton< Platform >
{
protected:
	int			m_iWidth;
	int			m_iHeight;
	bool		m_isInit;
	
public:
	Platform():Singleton(), m_isInit(false)		{ };
	virtual ~Platform() { };

	virtual void		Init(int width, int height) = 0;
	virtual void		Update() = 0;				//use for key map if has
	virtual void		SwapBuffers() = 0;			//this function depend on which platform using
	virtual void		Resize(int width, int height) = 0;
	virtual void		Exit() = 0;

	virtual void		printSystemSpecs() = 0;

	int			GetWidth()		{	return m_iWidth;	};
	int			GetHeight()		{	return m_iHeight;	};

	bool			isInit()		{	return m_isInit;	};
	virtual std::string		getFullPath(const char* path) = 0;
	
};

#define		PLATFORM				Platform::getInstance()
#define		PLATFORM_GET_PATH(x)	PLATFORM->getFullPath(x).c_str()

#endif