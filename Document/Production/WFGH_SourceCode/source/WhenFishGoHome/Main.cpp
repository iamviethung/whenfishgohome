// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

#include "Shaders.h"
#include "Globals.h"
#include <conio.h>

clock_t fpsTimer;
int fps;

int Init(ESContext *esContext)
{
	GameManager::GetInstance()->Init();
	GameManager::GetInstance()->SwitchState(MenuManager::GetInstance());

	fpsTimer = clock();
	fps = 0;
	return 0;
}

void Draw(ESContext *esContext)
{
	GameManager::GetInstance()->Update(true);
	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

void Update ( ESContext *esContext, float deltaTime )
{}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
}

void CleanUp()
{
	delete GameManager::GetInstance();
	delete ResourceManager::GetInstance();
	delete SceneManager::GetInstance();
	delete MenuManager::GetInstance();
}

void LMouseDown(ESContext *esContext, int mouse_x, int mouse_y)
{
	GameManager::GetInstance()->LMouseDown(mouse_x, mouse_y);
}

void LMouseUp(ESContext *esContext, int mouse_x, int mouse_y)
{
	GameManager::GetInstance()->LMouseUp(mouse_x, mouse_y);
}

void MouseMove(ESContext *esContext, int mouse_x, int mouse_y)
{
	GameManager::GetInstance()->MouseMove(mouse_x, mouse_y);
}

int _tmain(int argc, _TCHAR* argv[])
{
	ESContext esContext;
	
	esInitContext ( &esContext );

	esCreateWindow ( &esContext, "OpenGL ES", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;
	
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterDrawFunc(&esContext, Draw);
	esRegisterKeyFunc ( &esContext, Key);
	esRegisterLeftMouseDownFunc(&esContext, LMouseDown);
	esRegisterLeftMouseUpFunc(&esContext, LMouseUp);
	esRegisterMouseMoveFunc(&esContext, MouseMove);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();

	//identifying memory leaks
	MemoryDump();
	cout << fps << endl;
	printf("Press any key...\n");
	_getch();
	
	return 0;
}
