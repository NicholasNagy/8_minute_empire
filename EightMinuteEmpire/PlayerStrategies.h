#pragma once
class PlayerStrategies
{

public:
	virtual int pickCard() = 0;

};


class GreedyCPU : public PlayerStrategies
{
	int pickCard();


};

class ModerateCPU : public PlayerStrategies
{
	int pickCard();
};


class Human : public PlayerStrategies
{
	int pickCard();

};
