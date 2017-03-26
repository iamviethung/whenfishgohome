#include "stdafx.h"
#include "FPSLimiter.h"

#include <time.h>

#ifdef _WIN32
extern HWND g_hWnd;
#endif

FPSLimiter::FPSLimiter() :m_bEnable(false), m_fFrameTime(1000.0f/60), m_lLastTime(-1)
{
}

FPSLimiter::~FPSLimiter()
{
}

void FPSLimiter::setEnable(bool enable)
{
	m_bEnable = enable;

	if (enable)
	{
		m_lLastTime = clock();
	}
}

void FPSLimiter::setFPS(int fps)
{
	m_iFPS = fps;
	m_fFrameTime = 1000.0f / m_iFPS;
}

int FPSLimiter::getFPS()
{
	return m_iFPS;
}

void FPSLimiter::tick()
{
	long cur_time = clock();	
	m_fDeltaTime = (cur_time - m_lLastTime) / 1000.0f;

	// Show frame-per-second
	static int framesPerSecond = 0;
	static float fpsTime = 0.0f;
	char strFrameRate[50] = { 0 };

	fpsTime += m_fDeltaTime;	
	if (fpsTime > 1.0f)
	{
		fpsTime = 0.0f;
		sprintf(strFrameRate, "Current Frames Per Second: %d", framesPerSecond);

#ifdef _WIN32
		SetWindowText(g_hWnd, strFrameRate);
#endif
		framesPerSecond = 0;
	}
	else
	{
		// Increase the frame counter
		++framesPerSecond;
	}

	// Limit FPS
	if (m_bEnable)
	{
		long sleepTime = m_fFrameTime - (cur_time - m_lLastTime);
		if (sleepTime > 0)
		{
#ifdef _WIN32
			Sleep(sleepTime);
#endif
			cur_time += sleepTime;
		}
	}

	m_lLastTime = cur_time;	
}

float FPSLimiter::getDeltaTime()
{
	return m_fDeltaTime;
}