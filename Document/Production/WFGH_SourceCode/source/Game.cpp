#include "stdafx.h"
#include <string>
using namespace std;

#include "Game.h"
#include <time.h>

#include "Platform/Platform.h"
#include "Platform/InputManager.h"

#include "Utils/FPSLimiter.h"

#include "WhenFishGoHome/GameManager.h"
#include "WhenFishGoHome/MenuManager.h"
#include "WhenFishGoHome/SceneManager.h"
#include "WhenFishGoHome/Globals.h"

bool Globals::isEndGame = false;
bool Globals::isWinGame = false;

int Globals::screenWidth = 0;
int Globals::screenHeight = 0;

Vector2 Globals::goalPos = Vector2(0.0, 0.0);

int num_fps = 0;
int stepSleep = 0;

Game::Game() :
m_isInit(false),
m_isRunning(false)
{
}

Game::~Game()
{	
	//clean up
}

//init function
void Game::Init(int width, int height)
{
	new FPSLimiter();	
	new InputManager();

	FPS_LMT->setFPS(60);
	FPS_LMT->setEnable(true);

	//call platform to init environment
	PLATFORM->Init(width, height);
	PLATFORM->printSystemSpecs();

	Globals::screenWidth = width;
	Globals::screenHeight = height;

	GameManager::GetInstance()->Init();
	GameManager::GetInstance()->SwitchState(MenuManager::GetInstance());
}

void Input()
{
	PointerEvent touch = INPUT_MGR->getCurPointerEvent();
	switch (touch.state)
	{
	case E_POINTER_PRESSSED:
		GameManager::GetInstance()->LMouseDown(touch.posX, touch.posY);
		break;
	case E_POINTER_MOVED:
		GameManager::GetInstance()->MouseMove(touch.posX, touch.posY);
		break;
	case E_POINTER_RELEASED:
		GameManager::GetInstance()->LMouseUp(touch.posX, touch.posY);
		break;
	case E_POINTER_NONE: break;
	}
}

//update function
void Game::Update(float dt)
{
	clock_t start = clock();
	//update platform - call
	PLATFORM->Update();

	// Press ESC to exit
#ifdef _WIN32
	if (INPUT_MGR->isKeyPressed(VK_ESCAPE))
		setRunning(false);
#endif
	//Limit FPS
	FPS_LMT->tick();
	// Update goes here
	Input();
}

//render function
void Game::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// Render goes here
	GameManager::GetInstance()->Update(true);
	//swap buffer after render
	PLATFORM->SwapBuffers();
}

void Game::Exit()
{
	m_isRunning = false;

	//clean up
	InputManager::freeInstance();	
	FPSLimiter::freeInstance();	

	GameManager::GetInstance()->Exit();
	delete GameManager::GetInstance();

	PLATFORM->Exit();
}

void Game::setRunning(bool running)
{
	m_isRunning = running;
}