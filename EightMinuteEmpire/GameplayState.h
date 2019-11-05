#pragma once
#include "SDL.h"
#include "GameState.h"
#include "Cards.h"

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

	static SDL_Renderer* renderer;
protected:
	GameplayState() {}

private:
	static GameplayState mGameplayState;

	void initWindow(Game* game);
	void initMap(Game* game);
	void initUI(Game* game);
	void getHoveredCountry();
	void getClickedCountry(bool isArmyBeingMoved);
	void handlePlayerAction(Game* game);
	void handlePlaceNewArmies(Game* game);
	void handleMoveArmies(Game* game);
	void handleBuildCity(Game* game);
	void handleDestroyArmy(Game* game);
	void handleAndOrAction(Game* game);
	void handleIgnore(Game* game);

	void nextMove(Game* game);
	void placeStartingArmies(Game* game);
	void initPlayerHoldings(Game* game); // Initilizes each players'holdings to default values 0 cities and 0 armies)
	Player* computeFinalScore(Game* game); //returns winner if there is no tie
};