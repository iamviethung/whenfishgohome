#include "stdafx.h"
#include "Button.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "MenuManager.h"

Button::Button() : m_isHovered(false){}
Button::~Button(){}
Type Button::GetType()
{
	return BUTTON;
}
ButtonType Button::GetButtonType()
{
	return m_bType;
}

void Button::SetButtonType(ButtonType type)
{
	m_bType = type;
}

void Button::DoClick()
{
	switch (m_bType)
	{
	case START:
	{
		MenuManager::GetInstance()->DrawLevelMenu();
	}
	break;

	case LEVEL:
	{
		if (m_levelState == UNLOCKED)
		{
			SceneManager::GetInstance()->SetStage(m_id);
			GameManager::GetInstance()->SwitchState(SceneManager::GetInstance());
		}
	}

	case PAUSE:
	{
		SceneManager::GetInstance()->Pause();
	}
	break;

	case RESUME:
	{
		SceneManager::GetInstance()->Resume();
	}
	break;

	case REPLAY:
	{
		SceneManager::GetInstance()->SetStage(SceneManager::GetInstance()->m_stageID);
		SceneManager::GetInstance()->ResetData();
		SceneManager::GetInstance()->Init();
		SceneManager::GetInstance()->Resume();
	}
	break;

	case DROP_ITEM:
	{
		SceneManager::GetInstance()->m_player->DropItem();
	}
	break;

	case TUTORIAL:
	{
		MenuManager::GetInstance()->ShowTutorial();
	}
	break;

	case OPTIONS:
	{

	}
	break;

	case CREDITS:
	{
		MenuManager::GetInstance()->ShowCredit();
	}
	break;

	case RETURN:
	{
		MenuManager::GetInstance()->DrawMainMenu();
	}

	case QUIT:
	{
		MenuManager::GetInstance()->DrawMainMenu();
		GameManager::GetInstance()->SwitchState(MenuManager::GetInstance());
	}
	break;

	case EXIT:
	{
		GameManager::GetInstance()->Exit();
	}
	break;
	}
}