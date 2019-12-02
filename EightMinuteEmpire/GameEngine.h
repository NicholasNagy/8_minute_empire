#pragma once
#include "GameplayState.h"

class GameEngine : public GameplayState
{
public:
	void init(Game* game);
	void pause();
	void resume();
	void clean(Game* game);
	void handleEvents(Game* game);
	void draw(Game* game);
	void update(Game* game);

	static GameEngine* Instance()
	{
		return &mGameEngine;
	}

protected:
	GameEngine() {}

private:
	static GameEngine mGameEngine;
	static SDL_Renderer* renderer;
	void ResultsTable(Game* game);

};



