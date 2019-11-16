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
	static bool inActionState;

protected:
	ActionState() {}

	void getSelectedCountry(Game* game);

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
	void getSelectedCountry(Game* game);
};

class MoveArmiesState : public ActionState
{

public:
	void init(Game* game);
	void pause();
	void resume();
	void clean(Game* game);
	void handleEvents(Game* game);
	void draw(Game* game);
	void update(Game* game);

	static MoveArmiesState* Instance()
	{
		return &mMoveArmiesState;
	}

protected:
	MoveArmiesState() {}

private:
	static MoveArmiesState mMoveArmiesState;
	void executeAction(Game* game);
	void getSelectedCountry(Game* game);

};
