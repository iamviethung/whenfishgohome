#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

#include <queue>
#include "Utils/Singleton.h"

enum KeyState
{
	E_KEY_RELEASED = 0,
	E_KEY_PRESSED,
	E_KEY_HOLD
};

enum PointerState
{
	E_POINTER_PRESSSED = 0,
	E_POINTER_MOVED,	
	E_POINTER_RELEASED,
	E_POINTER_NONE
};

struct PointerEvent
{
	PointerState	state;
	short			posX;
	short			posY;

	PointerEvent() :state(E_POINTER_NONE), posX(0), posY(0){}
	PointerEvent(PointerState state, short posX, short posY) :state(state), posX(posX), posY(posY) { };
};

class InputManager : public Singleton<InputManager>
{
	unsigned char*				m_keyMap;
	unsigned char*				m_nullKeyMap;

	PointerEvent				m_prevEvent;
	PointerEvent				m_curEvent;

public:
	InputManager();
	~InputManager();
	
	bool			hasKeyEvent();
	void			setKey(KeyState state, unsigned char keyCode);
	bool			isKeyPressed(unsigned char keyCode);
		
	void			updatePointerEvent(PointerState state, short posX, short posY);
	PointerEvent	getCurPointerEvent();
	PointerEvent	getPrevPointerEvent();
};

#define INPUT_MGR InputManager::getInstance()

#endif