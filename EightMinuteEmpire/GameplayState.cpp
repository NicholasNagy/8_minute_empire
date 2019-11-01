#include "GameplayState.h"
#include "TextureLoader.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <iostream>
#include <sstream>
#include "ImGui/imgui.h"
#include "ImGui/imgui_sdl.h"
#include "Map.h"
#include "Bid.h"
#include "GameOverlay.h"

GameplayState GameplayState::mGameplayState;
SDL_Renderer* GameplayState::renderer = nullptr;

SDL_Texture* texture = nullptr;
SDL_Rect cursor;
SDL_Rect cursorShadow = { cursor.x, cursor.y, GRID_CELL_SIZE, GRID_CELL_SIZE };
SDL_Color cursorShadowColor = { 107, 107, 107, 107 };
bool mouseActive = false;
bool mouseHover = false;
bool selectedCountry = false;

GraphWorld::Map* gameMap;
GraphWorld::Country* country = nullptr;
int numCountries;

GameOverlay ui;
Label* label;


void GameplayState::init(Game* game)
{
	std::cout << "\nGame Started\n\n-------------------------------------------------\n\n";

	//Creating Game Window
	SDL_Window* gameWindow = SDL_CreateWindow("Eight Minute Empire", 0, SDL_WINDOWPOS_CENTERED, WINDOW_X, WINDOW_Y, SDL_WINDOW_SHOWN);
	game->setWindow(gameWindow);

	//Creating Renderer to draw on window
	renderer = SDL_CreateRenderer(game->getWindow(), -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderSetLogicalSize(renderer, WINDOW_X, WINDOW_Y);


	//Drawing the map tiles once
	gameMap = game->getMap();
	string path = game->getMapLoader()->getTileSetPath();
	texture = TextureLoader::loadTexutre(path.c_str(), renderer);
	numCountries = gameMap->getNumCountries();
	gameMap->getTileMap()->drawTileMap(renderer, texture);


	//Drawing the UI Labels
	if (TTF_Init() == -1)
	{
		std::cout << "TTF_Init: %s\n" << TTF_GetError();
		exit(2);
	}
	string bidding = "Biding initiated in console!";
	ui.addFonts("assets/Fonts/unispace bd.ttf", "unispace bd", 18);
	SDL_Color black = { 0,0,0,0 };
	label = new Label(bidding, "unispace bd", 0, 0, black);
	label->setLabelText(renderer,bidding, ui.getFont("unispace bd"));
	label->drawLabel(renderer);
	SDL_RenderPresent(renderer);

	//Bidding
	Bid::initiateBidding(game);
	for (auto p : game->players())
		cout << p << endl;

	label->destroyLabelTexture();
	
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
		cursor.x = (event.motion.x / GRID_CELL_SIZE) * GRID_CELL_SIZE;
		cursor.y = (event.motion.y / GRID_CELL_SIZE) * GRID_CELL_SIZE;
		//std::cout << "x: " << cursor.x << " y: " << cursor.y << std::endl;

		//Find out type of tile clicked
		static int typeCol;
		static int typeRow;
		static int type;

		typeCol = cursor.x / GRID_CELL_SIZE;
		typeRow = cursor.y / GRID_CELL_SIZE;

		if (cursor.x < MAP_WIDTH * GRID_CELL_SIZE)
		{
			type = gameMap->getTileMap()->tiles[typeRow][typeCol];
			if (type < numCountries && type >= 0)
				country = gameMap->getCountry(type);
			if (country)
			{
				selectedCountry = true;
				std::cout << "Selected: " << country->displayCountry() << std::endl;
			}
		}
		break;
	case SDL_MOUSEMOTION:
		cursorShadow.x = (event.motion.x / GRID_CELL_SIZE) * GRID_CELL_SIZE;
		cursorShadow.y = (event.motion.y / GRID_CELL_SIZE) * GRID_CELL_SIZE;
		//std::cout << "x: " << cursor.x << " y: " << cursor.y << std::endl;
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
	gameMap->getTileMap()->drawTileMap(renderer, texture);
	if (mouseActive && mouseHover)
	{
		SDL_SetRenderDrawColor(renderer, cursorShadowColor.r, cursorShadowColor.g, cursorShadowColor.b, cursorShadowColor.a);
		SDL_RenderFillRect(renderer, &cursorShadow);
	}

	label->drawLabel(renderer);
	SDL_RenderPresent(renderer);

}

void GameplayState::update(Game* game)
{
	std::stringstream ss;
	if (country && selectedCountry)
	{
		label->destroyLabelTexture();
		selectedCountry = false;
		ss << "Selected: " << country->displayCountry();
		label->setLabelText(renderer, ss.str(), ui.getFont("unispace bd"));
	}

}
