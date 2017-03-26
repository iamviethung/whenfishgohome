#include "stdafx.h"

#include "InputManager.h"

#define VK_SIZE 256

InputManager::InputManager():Singleton()
{
	m_keyMap = new unsigned char[VK_SIZE];
	memset(m_keyMap, 0, VK_SIZE);

	m_nullKeyMap = new unsigned char[VK_SIZE];
	memset(m_nullKeyMap, 0, VK_SIZE);
}

InputManager::~InputManager()
{
	delete[] m_keyMap;
	delete[] m_nullKeyMap;
}

bool InputManager::hasKeyEvent()
{
	return memcmp(m_keyMap, m_nullKeyMap, VK_SIZE) == 0 ? false : true;
}

void InputManager::setKey(KeyState state, unsigned char keyCode)
{
	m_keyMap[keyCode] = state;
}
	 
bool InputManager::isKeyPressed(unsigned char keyCode)
{
	return m_keyMap[keyCode] == E_KEY_PRESSED ? true : false;
}

void InputManager::updatePointerEvent(PointerState state, short posX, short posY)
{
	m_prevEvent = m_curEvent;
	m_curEvent = PointerEvent(state, posX, posY);
}

PointerEvent InputManager::getCurPointerEvent()
{
	return m_curEvent;
}

PointerEvent InputManager::getPrevPointerEvent()
{
	return m_prevEvent;
}