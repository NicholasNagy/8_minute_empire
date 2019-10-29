#pragma once
#include "SDL.h"
#include "GameState.h"


class GameplayState : public GameState
{
public:
	void init(Game* game);
	void pause();
	void resume();
	void clean(Game* game);
	void handleEvents(Game* game);
	void draw(Game* game);

	void update(Game* game);

	static GameplayState* Instance()
	{
		return &mGameplayState;
	}

protected:
	GameplayState() {}

private:
	static GameplayState mGameplayState;
	static SDL_Renderer* renderer;

};