#pragma once
#include "SDL.h"
#include "GameState.h"

class MainMenuState : public GameState
{
public:
	void init(Game* game);
	void pause();
	void resume();
	void clean(Game* game);
	void handleEvents(Game* game);
	void draw(Game* game);
	void update(Game* game);

	static MainMenuState* Instance()
	{
		return &mMenuState;
	}

protected:
	MainMenuState(){}

private:
	static MainMenuState mMenuState;
	static SDL_Renderer* renderer;
	static SDL_Texture* logoTexture;
	static void handleButtons(Game* game);
};