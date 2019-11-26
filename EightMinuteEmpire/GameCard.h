#pragma once

#include "Cards.h"

class PlaceNewArmiesCard : public Card
{
	public:
		PlaceNewArmiesCard(const int myGood, const Action& myAction);
		~PlaceNewArmiesCard();
};

class MoveArmiesLandCard : public Card
{
	public:
		MoveArmiesLandCard(const int myGood, const Action& myAction);
		~MoveArmiesLandCard();
};

class MoveArmiesLandWaterCard : public Card
{
	public:
		MoveArmiesLandWaterCard(const int myGood, const Action& myAction);
		~MoveArmiesLandWaterCard();
};

class BuildCityCard : public Card
{
	public:;
		  BuildCityCard(const int myGood, const Action& myAction);
		  ~BuildCityCard();
};

class DestroyArmyCard : public Card
{
	public:
		DestroyArmyCard(const int myGood, const Action& myAction);
		~DestroyArmyCard();
};

class AndActionCard : public Card
{
	public:
		AndActionCard(const int myGood, const Action& myAction);
		~AndActionCard();
};

class Ignore1Card : public Card
{
	public:
		Ignore1Card(const int myGood, const Action& myAction);
		~Ignore1Card();
};

class Ignore2Card : public Card
{
	public:
		Ignore2Card(const int myGood, const Action& myAction);
		~Ignore2Card();
};