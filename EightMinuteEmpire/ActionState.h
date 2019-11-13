#pragma once
#include "SDL.h"
#include "GameplayState.h"

class ActionState :
	public GameplayState
{	

public:
	void init(Game* game);
	void resume();
	void pause();
	void clean(Game* game);
	void handleEvents(Game* game);
	void draw(Game* game);
	void update(Game* game);
	static Player* toPlay;

protected:
	ActionState() {}

	void getClickedCountry(Game* game);

};

class PlaceNewArmiesState : public ActionState
{

public:
	void init(Game* game);
	void pause();
	void resume();
	void clean(Game* game);
	void handleEvents(Game* game);
	void draw(Game* game);
	void update(Game* game);

	static PlaceNewArmiesState* Instance()
	{
		return &mPlaceNewArmiesState;
	}

protected:
	PlaceNewArmiesState() {}

private:
	static PlaceNewArmiesState mPlaceNewArmiesState;
	void executeAction();

};

