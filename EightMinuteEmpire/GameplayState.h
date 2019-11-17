#pragma once
#include "SDL.h"
#include "GameState.h"
#include "Cards.h"
#include "Observable.h"

class GameplayState : public GameState, public Observable
{
public:
	void init(Game* game);
	void pause();
	void resume();
	void clean(Game* game);
	void handleEvents(Game* game);
	void draw(Game* game);
	void update(Game* game);
	void nextMove(Game* game);



	static GameplayState* Instance()
	{
		return &mGameplayState;
	}

	static SDL_Renderer* renderer;
	static 	SDL_Event event;
protected:
	GameplayState() {}

private:
	static GameplayState mGameplayState;

	void initWindow(Game* game);
	void initMap(Game* game);
	void initUI(Game* game);
	void getHoveredCountry();
	void handleCardSelection(Game* game, int position);

	void placeStartingArmies(Game* game);
	void initPlayerHoldings(Game* game); // Initilizes each players'holdings to default values 0 cities and 0 armies)
	Player* computeFinalScore(Game* game); //returns winner if there is no tie
};