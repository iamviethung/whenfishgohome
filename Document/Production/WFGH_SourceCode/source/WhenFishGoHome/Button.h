#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "Struct.h"
#include "Object.h"

class Button : public Object
{
public:
	Button();
	~Button();
	Type GetType();
	ButtonType GetButtonType();
	void SetButtonType(ButtonType type);

	void DoClick();

public:
	bool m_isHovered;
	GLuint m_clickedTexPos;
	GLuint m_UnClickedTexPos;
	LevelState m_levelState;

private:
	ButtonType m_bType;
};

#endif