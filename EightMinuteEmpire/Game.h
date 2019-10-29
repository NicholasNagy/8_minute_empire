#pragma once
#include "SDL.h"
#include "Map.h"
#include "Player.h"
#include <vector>

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
		std::vector<Player*> players(); //get vector of players
		GraphWorld::Map* getMap();
		SDL_Window* getWindow();

private:
	bool mIsRunning;
	SDL_Window* window;
	std::vector<GameState*> states;
	GraphWorld::Map* map;
	std::vector<Player*> mPlayers;

};

