#include "SDL.h"
#include "Game.h"
#include "GameState.h"

/*To learn the GUI SDL2 and inspiration for the game state stack architecture I used these as references

https://gamedevelopment.tutsplus.com/articles/how-to-build-a-jrpg-a-primer-for-game-developers--gamedev-6676#state
https://www.dreamincode.net/forums/topic/120775-beginning-sdl-part-4-state-manager/
https://gamedev.stackexchange.com/questions/1783/game-state-stack
https://gamedev.stackexchange.com/questions/72878/how-can-i-implement-a-main-menu

*/

int WINDOW_X = 1280;
int WINDOW_X_SMALL = 800;
int WINDOW_Y = 640;
int GRID_CELL_SIZE = 32;
int MAP_WIDTH = 32;
int MAP_HEIGHT = 20;

Game::Game() : mIsRunning(false), window(nullptr), map(nullptr) {}

Game::~Game()
{
}

void Game::init(const char* title, int initialX, int initialY, int width, int height, Uint32 flags)
{

	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow(title, initialX, initialY, width, height, flags);
	if (window)
		mIsRunning = true;
	else
		mIsRunning = false;

}

void Game::update()
{
	states.back()->update(this);
}


void Game::clean()
{
	while (!states.empty())
	{
		states.back()->clean(this);
		states.pop_back();

	}
	SDL_DestroyWindow(getWindow());
	SDL_Quit();

}

void Game::draw()
{
	states.back()->draw(this);
}

void Game::handleEvents()
{
	states.back()->handleEvents(this);

}

bool Game::isRunning()
{
	return mIsRunning;
}

void Game::setRunning(bool b)
{
	mIsRunning = b;
}

// Changes the current game state to a new one. 
void Game::changeState(GameState* state)
{
	//Pop current game state
	if (!states.empty())
	{
		states.back()->clean(this);
		states.pop_back();
	}

	// Push a new game state
	states.push_back(state);
	states.back()->init(this);
}

// Pops a game state from the stack and then resumes the top state. 
void Game::popState()
{
	// Pop current game state
	if (!states.empty()) {
		states.back()->clean(this);
		states.pop_back();
	}

	// Resume state
	if (!states.empty()) {
		states.back()->resume();
	}
}

// Pauses the current game state then pushes a new game state to the stack. 
void Game::pushState(GameState* state)
{
	// Pause game state
	if (!states.empty()) {
		states.back()->pause();
	}

	// Push state
	states.push_back(state);
	states.back()->init(this);

}

void Game::setMap(GraphWorld::Map* map)
{
	this->map = map;
}

std::vector<Player*>& Game::players()
{
	return mPlayers;
}

GraphWorld::Map* Game::getMap()
{
	return map;
}

MapLoader* Game::getMapLoader()
{
	return mapLoader;
}

void Game::setMapLoader(MapLoader* m)
{
	mapLoader = m;
}


SDL_Window* Game::getWindow()
{
	return window;
}

void Game::setWindow(SDL_Window* win)
{
	window = win;
}
