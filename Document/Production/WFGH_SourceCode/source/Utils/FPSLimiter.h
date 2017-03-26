#pragma once

#include "Singleton.h"

class FPSLimiter : public Singleton<FPSLimiter>
{
	bool				m_bEnable;
	float				m_fFrameTime;
	int					m_iFPS;
	long				m_lLastTime;
	float				m_fDeltaTime;

public:
	FPSLimiter();
	~FPSLimiter();

	void	setEnable(bool enable);
	void	setFPS(int fps);
	int		getFPS();
	float	getDeltaTime();

	void	tick();
};

#define FPS_LMT FPSLimiter::getInstance()