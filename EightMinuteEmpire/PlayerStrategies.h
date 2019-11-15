#pragma once
#include "Game.h"
#include "ActionState.h"
class PlayerStrategies
{

public:
	virtual void pickCard(Game* game, int position) = 0;

};


class GreedyCPU : public PlayerStrategies
{
	void pickCard(Game* game, int position);


};

class ModerateCPU : public PlayerStrategies
{
	void pickCard(Game* game, int position);
};


class Human : public PlayerStrategies
{
	void pickCard(Game* game, int position);

};
