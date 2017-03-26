#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__

#include "State.h"

using namespace std;

class GameManager
{
public:
	static GameManager* GetInstance();
	~GameManager();
	void Init();
	void Update(bool isRun);

	void LMouseDown(int mouse_x, int mouse_y);
	void LMouseUp(int mouse_x, int mouse_y);
	void MouseMove(int mouse_x, int mouse_y);

	void SwitchState(State* nextState);
	void Exit();

private:
	GameManager();

private:
	static GameManager* m_instance;
	State* m_pCurrentState;
	State* m_pNextState;
};

#endif