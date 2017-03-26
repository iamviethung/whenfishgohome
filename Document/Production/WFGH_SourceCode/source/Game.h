#ifndef _GAME_H_
#define _GAME_H_

#include <vector>

class Camera;
class BSPLevel;
class Frustum;
class GUI;
class NFGObject;

class Game
{	
public:
	Game();		//constructor
	~Game();	//detructor	

	void		Init(int width, int height);
	//update and render function
	void		Update(float dt);
	void		Render();
	void		Exit();	

	void			setRunning(bool running = true);

	inline bool		isInit()	{	return m_isInit;	};
	inline bool		isRunning()	{	return m_isRunning;	};
private:
	bool			m_isInit;
	bool			m_isRunning;	
};

#endif