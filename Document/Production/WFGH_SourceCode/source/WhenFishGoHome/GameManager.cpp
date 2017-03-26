#include "stdafx.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include "MenuManager.h"
#include "SceneManager.h"

GameManager* GameManager::m_instance = NULL;

GameManager::GameManager() :m_pCurrentState(NULL), m_pNextState(NULL)
{}

void GameManager::Init()
{
#ifdef WIN32
	ResourceManager::GetInstance()->readSourceFile("data");
	return;
#endif // WIN

	//ANDROID
	//ResourceManager::GetInstance()->readSourceFile("Removable/MicroSD/thuyen/data");
	ResourceManager::GetInstance()->readSourceFile("sdcard/Thuyen/Data");
}

GameManager* GameManager::GetInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new GameManager();
	}
	return m_instance;
}

void GameManager::LMouseDown(int mouse_x, int mouse_y)
{
	if (this->m_pCurrentState) this->m_pCurrentState->LMouseDown(mouse_x, mouse_y);
}

void GameManager::LMouseUp(int mouse_x, int mouse_y)
{
	if (this->m_pCurrentState) this->m_pCurrentState->LMouseUp(mouse_x, mouse_y);
}

void GameManager::MouseMove(int mouse_x, int mouse_y)
{
	if (this->m_pCurrentState) this->m_pCurrentState->MouseMove(mouse_x, mouse_y);
}

void GameManager::Update(bool isRun)
{
	//Check if need switch state
	if (m_pCurrentState != m_pNextState)
	{
		if (m_pCurrentState)
		{
			m_pCurrentState->Pause();
		}

		if (m_pNextState)
		{
			m_pNextState->Resume();
		}

		m_pCurrentState = m_pNextState;
	}

	// Update state
	if (m_pCurrentState)
	{
		if (isRun)
		{
			m_pCurrentState->Update();
		}

		m_pCurrentState->Draw();
	}
}

void GameManager::Exit()
{
	//GameManager::~GameManager();
	//exit(0);
}

void GameManager::SwitchState(State* nextState)
{
	m_pNextState = nextState;
	m_pNextState->ResetData();
	m_pNextState->Init();
}

GameManager::~GameManager()
{
	delete ResourceManager::GetInstance();
	delete SceneManager::GetInstance();
	delete MenuManager::GetInstance();
	//delete m_pCurrentState;

	m_instance = NULL;
}