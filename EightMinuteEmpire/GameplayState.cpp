#include "GameplayState.h"
#include "TextureLoader.h"
#include "SDL_image.h"
#include <iostream>
#include "ImGui/imgui.h"
#include "ImGui/imgui_sdl.h"
#include "Map.h"

GameplayState GameplayState::mGameplayState;
SDL_Renderer* GameplayState::renderer = nullptr;

SDL_Texture* texture = nullptr;
SDL_Rect cursor;
SDL_Rect cursorShadow = {cursor.x, cursor.y, 32,32 };
SDL_Color cursorShadowColor = { 107, 107, 107, 107 };
GraphWorld::Map* map;
GraphWorld::Country* country = nullptr;
int numCountries;
bool mouseActive = false;
bool mouseHover = false;

void GameplayState::init(Game* game)
{
	std::cout << "Game Started\n\n-------------------------------------------------\n\n";
	SDL_Window* gameWindow = SDL_CreateWindow("Eight Minute Empire", 0, SDL_WINDOWPOS_CENTERED, 1600, 640, SDL_WINDOW_SHOWN );
	game->setWindow(gameWindow);
	renderer = SDL_CreateRenderer(game->getWindow(), -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderSetLogicalSize(renderer, 1600, 640);
	texture = TextureLoader::loadTexutre("assets/TileMaps/Ancient_KingdomSet.png", renderer);
	map = game->getMap();
	numCountries = map->getNumCountries();

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
		case SDL_MOUSEBUTTONDOWN:
			cursor.x = (event.motion.x / 32) * 32;
			cursor.y = (event.motion.y / 32) * 32;
			
			//Find out type of tile clicked
			static int typeCol;
			static int typeRow;
			static int type;

			typeCol = cursor.x / 32;
			typeRow = cursor.y / 32;
			type = map->getTileMap()->tiles[typeRow][typeCol];
			
			if (type < numCountries && numCountries > 0  )
				country = map->getCountry(type);

			if (country)
			std::cout << "Selected: " << country->displayCountry() << std::endl;
			break;
		case SDL_MOUSEMOTION:
				cursorShadow.x = (event.motion.x / 32) * 32;
				cursorShadow.y = (event.motion.y / 32) * 32;;
			//	std::cout << "x: " << cursor.x << " y: " << cursor.y << std::endl;
			if (!mouseActive)
				mouseActive = true;
			break;
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_ENTER && !mouseHover)
				mouseHover = true;
			else if (event.window.event == SDL_WINDOWEVENT_LEAVE && mouseHover)
				mouseHover = false;
					break;
		default:
			break;
		}
	

}

void GameplayState::draw(Game* game)
{
	SDL_RenderClear(renderer);
	game->getMap()->getTileMap()->drawTileMap(renderer, texture);
	if (mouseActive && mouseHover)
	{
		SDL_SetRenderDrawColor(renderer, cursorShadowColor.r,cursorShadowColor.g,cursorShadowColor.b,cursorShadowColor.a);
		SDL_RenderFillRect(renderer, &cursorShadow);
	}
	SDL_RenderPresent(renderer);

}

void GameplayState::update(Game* game)
{


}
