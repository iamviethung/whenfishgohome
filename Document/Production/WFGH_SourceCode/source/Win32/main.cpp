#include "stdafx.h"

#include "Game.h"
#include "Platform/PlatformWin32.h"
#include "Utils/FPSLimiter.h"

#define SCREEN_W 1024
#define SCREEN_H 576

void main(int argc, char **argv)
{	
	//init singleton
	Game* game = new Game();
	new PlatformWin32();

	game->Init(SCREEN_W, SCREEN_H);
	game->setRunning();

	///* Enter event loop */
	while (game->isRunning()) {
		game->Update(FPS_LMT->getDeltaTime());
		game->Render();
    }

	game->Exit();

	Platform::freeInstance();
	delete game;
}
