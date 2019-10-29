#include "GameplayState.h"
#include "TextureLoader.h"
#include "SDL_image.h"
#include <iostream>
#include "ImGui/imgui.h"
#include "ImGui/imgui_sdl.h"

GameplayState GameplayState::mGameplayState;
SDL_Renderer* GameplayState::renderer = nullptr;

SDL_Texture* texture = nullptr;
SDL_Rect srcR, destR;
int f = 0;

void GameplayState::init(Game* game)
{
	std::cout << "Game Started\n";
	renderer = SDL_CreateRenderer(game->getWindow(), -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	texture = TextureLoader::loadTexutre("assets/p1.png", renderer);
}

void GameplayState::pause()
{

	printf("Game paused");
}

void GameplayState::resume()
{
	printf("Game resumed");
}

void GameplayState::clean(Game* game)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(game->getWindow());
	SDL_Quit();
	std::cout << "Game State Cleaned";
}

void GameplayState::handleEvents(Game* game)
{

	SDL_Event event;
	SDL_PollEvent(&event);	
	switch (event.type)
		{
		case SDL_QUIT:
			game->setRunning(false);
			break;
		default:
			break;
		}
	

}

void GameplayState::draw(Game* game)
{
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, &destR);
	SDL_RenderPresent(renderer);

}

void GameplayState::update(Game* game)
{
	std::cout << f++ << endl;
	destR.h = 64;
	destR.w = 64;
	destR.x = f;
}
