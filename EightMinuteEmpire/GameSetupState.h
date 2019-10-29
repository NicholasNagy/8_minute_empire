#pragma once
#include "SDL.h"
#include "GameState.h"

class GameSetupState : public GameState
{
public:
	void init(Game* game);
	void pause();
	void resume();
	void clean(Game* game);
	void handleEvents(Game* game);
	void draw(Game* game);
	void update(Game* game);

	static GameSetupState* Instance()
	{
		return &mGameSetupState;
	}

protected:
	GameSetupState() {}

private:
	static GameSetupState mGameSetupState;
	static SDL_Renderer* renderer;
	static std::vector<bool> players; //holds player before the game starts
	static std::vector<int> ages; //holds player ages
	static bool mapLoaded;
	static void handleMapPicker(Game* game);
	static void handlePlayerPicker(Game* game);
	static void handleGameStart(Game* game);
	static bool initMapLoader(Game* game);
};