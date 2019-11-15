#pragma once
#include "Game.h"
#include "ActionState.h"
class PlayerStrategies
{

public:
	virtual int pickCard(Game* game) = 0;

};


class GreedyCPU : public PlayerStrategies
{
	int pickCard(Game* game);


};

class ModerateCPU : public PlayerStrategies
{
	int pickCard(Game* game);
};


class Human : public PlayerStrategies
{
	int pickCard(Game* game);

};
