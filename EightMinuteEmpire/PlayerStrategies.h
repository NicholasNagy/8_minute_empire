#pragma once
#include "Game.h"
#include "ActionState.h"
class PlayerStrategies
{

public:
	virtual void pickCard(Game* game, int position) = 0;
	virtual void playCard(Game* game) = 0;
	virtual void PlaceNewArmies(int numberOfArmies, GraphWorld::Country* country) = 0;
	virtual void MoveArmies(GraphWorld::Country* srcCountry, GraphWorld::Country* destCountry) = 0;
	virtual void BuildCity(GraphWorld::Country* country) = 0;
	virtual void DestroyArmy(Player* player, GraphWorld::Country* country) = 0;
	virtual void Ignore() = 0;

};


class GreedyCPU : public PlayerStrategies
{
	void pickCard(Game* game, int position);
	void playCard(Game* game);
	void PlaceNewArmies(int numberOfArmies, GraphWorld::Country* country);
	void MoveArmies(GraphWorld::Country* srcCountry, GraphWorld::Country* destCountry);
	void BuildCity(GraphWorld::Country* country);
	void DestroyArmy(Player* player, GraphWorld::Country* country);
	void Ignore();
	
	//helper functions
	static GraphWorld::Country* determineCountryForArmyPlacement(Game* game);
	static GraphWorld::Country* determineCountryForCityBuild(Game* game);
	static GraphWorld::Country* determineArmiesToDestroy(Game* game);

};


class ModerateCPU : public PlayerStrategies
{
	void pickCard(Game* game, int position);
	void playCard(Game* game);
	void PlaceNewArmies(int numberOfArmies, GraphWorld::Country* country);
	void MoveArmies(GraphWorld::Country* srcCountry, GraphWorld::Country* destCountry);
	void BuildCity(GraphWorld::Country* country);
	void DestroyArmy(Player* player, GraphWorld::Country* country);
	void Ignore();

	//helper functions
	static GraphWorld::Country* determineCountryForArmyPlacement(Game* game);
	static GraphWorld::Country* determineCountryForCityBuild(Game* game);
	static GraphWorld::Country* determineArmiesToDestroy(Game* game);
};




class Human : public PlayerStrategies
{
	void pickCard(Game* game, int position);
	void playCard(Game* game);
	void PlaceNewArmies(int numberOfArmies, GraphWorld::Country* country);
	void MoveArmies(GraphWorld::Country* srcCountry, GraphWorld::Country* destCountry);
	void BuildCity(GraphWorld::Country* country);
	void DestroyArmy(Player* player, GraphWorld::Country* country);
	void Ignore();
};

