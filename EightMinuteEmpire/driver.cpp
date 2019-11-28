#include "SDL.h"
#undef main
#include <iostream>
#include "Game.h"
#include "MainMenuState.h"

void limitFPS(Uint32 frameStart, const int frameDelay);

int main(int argc, char* argv[])
{
	
	int const FPS = 60;
	int const frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	Game game;

	game.init("Eight Minute Empire", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_X_SMALL, WINDOW_Y, SDL_WINDOW_SHOWN );
	game.changeState(MainMenuState::Instance());
		while (game.isRunning())
		{

		frameStart = SDL_GetTicks();

				game.handleEvents();
				game.update();
				game.draw();

		// limitFPS(frameStart, frameDelay);
		
		}

		game.clean();

	return 0;
}

void limitFPS(Uint32 frameStart, const int frameDelay)
{
	int fTime = SDL_GetTicks() - frameStart;

			if (frameDelay > fTime)
				SDL_Delay(frameDelay - fTime);
}
