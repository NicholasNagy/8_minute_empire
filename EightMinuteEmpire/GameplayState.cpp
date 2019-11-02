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
SDL_Surface* screen;
SDL_Rect cursor;
SDL_Rect cursorShadow = { cursor.x, cursor.y, GRID_CELL_SIZE, GRID_CELL_SIZE };
SDL_Color cursorShadowColor = { 107, 107, 107, 107 };
bool mouseActive = false;
bool mouseHover = false;
bool mouseClick = false;
bool bid = false;
bool nextAction = false;

GraphWorld::Map* gameMap;
GraphWorld::Country* country = nullptr;
GraphWorld::Country* startingCountry = nullptr;
int numCountries;
int numPlayers;
Player* toPlay;

GameOverlay ui;
Label* countryHoverLabel;
Label* playerInfoLabel;


int playerMove;  //The current player's turn

void GameplayState::init(Game* game)
{
	std::cout << "\nGame Started\n\n-------------------------------------------------\n\n";
	numPlayers = game->players().size();

	GameplayState::initWindow(game);

	GameplayState::initMap(game);

	GameplayState::initUI(game);

	int playerMove = Bid::initiateBidding(game);
	bid = true;
	placeStartingArmies(game);
	handlePlayerAction(game);

}

void GameplayState::initWindow(Game* game)
{

	//Creating Game Window
	SDL_Window* gameWindow = SDL_CreateWindow("Eight Minute Empire", 0, SDL_WINDOWPOS_CENTERED, WINDOW_X, WINDOW_Y, SDL_WINDOW_SHOWN);
	game->setWindow(gameWindow);
	screen = SDL_GetWindowSurface(gameWindow);
	//Creating Renderer to draw on window
	renderer = SDL_CreateRenderer(game->getWindow(), -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderSetLogicalSize(renderer, WINDOW_X, WINDOW_Y);

}

void GameplayState::initMap(Game* game)
{
	gameMap = game->getMap();
	string path = game->getMapLoader()->getTileSetPath();
	texture = TextureLoader::loadTexutre(path.c_str(), renderer);
	numCountries = gameMap->getNumCountries();
	gameMap->getTileMap()->drawTileMap(renderer, texture);
	startingCountry = gameMap->getStartingCountry();
	initPlayerHoldings(game);
}

void GameplayState::initUI(Game* game)
{
	if (TTF_Init() == -1)
	{
		std::cout << "TTF_Init: %s\n" << TTF_GetError();
		exit(2);
	}
	string bidding = "Biding initiated in console!";
	ui.addFonts("assets/Fonts/unispace bd.ttf", "unispace bd", 18);
	ui.addFonts("assets/Fonts/arial.ttf", "arial", 22);
	SDL_Color black = { 0,0,0,0 };
	countryHoverLabel = new Label(bidding, "arial", 0, 0, black);
	countryHoverLabel->setLabelText(renderer,screen, bidding, ui.getFont("arial"));
	countryHoverLabel->drawLabel(renderer);
	playerInfoLabel = new Label("", "unispace bd", 1040, 0, black);
	playerInfoLabel->setLabelText(renderer,screen, "", ui.getFont("unispace bd"));
	playerInfoLabel->drawLabel(renderer);
	SDL_RenderPresent(renderer);
	countryHoverLabel->destroyLabelTexture();
	playerInfoLabel->destroyLabelTexture();



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
		if (bid)
			//getCursorCountry();
			mouseClick = true;
		break;
	case SDL_MOUSEMOTION:
		cursor.x = cursorShadow.x = (event.motion.x / GRID_CELL_SIZE) * GRID_CELL_SIZE;
		cursor.y = cursorShadow.y = (event.motion.y / GRID_CELL_SIZE) * GRID_CELL_SIZE;
		//std::cout << "x: " << cursor.x << " y: " << cursor.y << std::endl;
		if (!mouseActive)
			mouseActive = true;
		if (bid)
			getCursorCountry();
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

void GameplayState::getCursorCountry()
{

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
		if (country && mouseClick)
		{
			std::cout << "Selected: " << country->displayCountry() << std::endl;
			mouseClick = false;
		}

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

	countryHoverLabel->drawLabel(renderer);
	playerInfoLabel->drawLabel(renderer);
	SDL_RenderPresent(renderer);

}

void GameplayState::update(Game* game)

{
	std::stringstream ss;
	countryHoverLabel->destroyLabelTexture();
	playerInfoLabel->destroyLabelTexture();

	for (Player* p : game->players())
		ss << *p;
	
	playerInfoLabel->setLabelText(renderer,screen, ss.str(), ui.getFont("unispace bd"));

	ss.clear();
	if (country)
	{
		std::stringstream ss;
		ss << country->displayCountry() << *toPlay->getHoldings(country, numCountries);
		countryHoverLabel->setLabelText(renderer,screen, ss.str(), ui.getFont("arial"));

	}

}



void GameplayState::handlePlayerAction(Game* game)
{
	toPlay = game->players().at(playerMove);
	
	cout << toPlay->getName() << " turn to play\n";

	switch (playerMove+1)
	{

	case 1:

		toPlay->PlaceNewArmies(3, gameMap->getCountry(5) );

		break;


	case 2:


		break;

	case 3:


		break;

	case 4:


		break;

	case 5:


		break;

	default:
		break;
	}

	nextMove();
}

inline void GameplayState::nextMove()
{
	playerMove++;

	if (playerMove > numPlayers);
	playerMove = 1;
}

void GameplayState::placeStartingArmies(Game* game)
{
	for ( Player* p: game->players() )
	{
		startingCountry->occupyingPlayers().emplace(p);
		p->holdings().emplace(startingCountry->getID(), new Holdings());	
		p->PlaceNewArmies(3, startingCountry);
	}

	cout << "\n------------------------------------------------------------\n";

}

void GameplayState::initPlayerHoldings(Game* game)
{
	for (int i = 0; i < numCountries; i++)
	{
		for (Player* p : game->players())
		{
			p->holdings().emplace(i, new Holdings());
		}



	}

}


void choseMove()
{
	// PlaceNewArmies():Place new armies on the board.
	//MoveOverLand() and MoveOverWater():Move over land and/or water
	//BuildCity() :Build a city.
	//DestroyArmy():Destroy army.
	//AndOrAction(): �And/Or� actions. 
	// Ignore() : Player may take the card and ignore the action
}