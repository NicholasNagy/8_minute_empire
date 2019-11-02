#pragma once
#include "SDL.h"
#include "Map.h"
#include "Player.h"
#include "MapLoader.h"
#include <vector>

extern int WINDOW_X;
extern int WINDOW_X_SMALL;
extern int WINDOW_Y;
extern int GRID_CELL_SIZE;
extern int MAP_WIDTH; //number of columns 
extern int MAP_HEIGHT; //number of rows

class GameState;

class Game
{
public:
	Game();
	~Game();
	void init(const char* title, int initialX, int initialY, int width, int height, Uint32 flags);
	void handleEvents();
	void update();
	void draw();
	void clean();
	bool isRunning();
	void setRunning(bool);
	void changeState(GameState* state);
	void popState();
	void pushState(GameState* state);
	void setMap(GraphWorld::Map* map);
	std::vector<Player*>& players(); //get vector of players
	GraphWorld::Map* getMap();
	MapLoader* getMapLoader();
	void setMapLoader(MapLoader*);
	SDL_Window* getWindow();
	void setWindow(SDL_Window*);

private:
	bool mIsRunning;
	SDL_Window* window;
	std::vector<GameState*> states;
	MapLoader* mapLoader;
	GraphWorld::Map* map;
	std::vector<Player*> mPlayers;


};

