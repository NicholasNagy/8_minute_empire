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
	//string getStatus() { return status; }
	//string startNewStatus(string newStatus) { status = newStatus; notify(); }
	//string updateStatus(string addStatus) { status = status + "\n" + addStatus; notify();}

protected:
	ActionState() {}

private: 
	//string status;
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


class BuildCityState : public ActionState
{

public:
	void init(Game* game);
	void pause();
	void resume();
	void clean(Game* game);
	void handleEvents(Game* game);
	void draw(Game* game);
	void update(Game* game);

	static BuildCityState* Instance()
	{
		return &mBuildCityState;
	}

protected:
	BuildCityState() {}

private:
	static BuildCityState mBuildCityState;
	void executeAction(Game* game);
	void getSelectedCountry(Game* game);

};

class DestroyArmyState : public ActionState
{

public:
	void init(Game* game);
	void pause();
	void resume();
	void clean(Game* game);
	void handleEvents(Game* game);
	void draw(Game* game);
	void update(Game* game);

	static DestroyArmyState* Instance()
	{
		return &mDestroyArmyState;
	}

protected:
	DestroyArmyState() {}

private:
	static DestroyArmyState mDestroyArmyState;
	void executeAction(Game* game);
	void getSelectedCountry(Game* game);
	void getSelectedPlayer(Game* game, int index);

};


class IgnoreState : public ActionState
{

public:
	void init(Game* game);
	void pause();
	void resume();
	void clean(Game* game);
	void handleEvents(Game* game);
	void draw(Game* game);
	void update(Game* game);

	static IgnoreState* Instance()
	{
		return &mIgnoreState;
	}

protected:
	IgnoreState() {}

private:
	static IgnoreState mIgnoreState;
	void executeAction(Game* game);
};
