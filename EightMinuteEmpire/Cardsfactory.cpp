#pragma once

#include "Cardsfactory.h"
#include "GameCard.h"
#include <random>

using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;

CardsFactory::CardsFactory() {}

CardsFactory::~CardsFactory() {}

Card* CardsFactory::makeCard(string type)
{
	random_device dev;
	mt19937 rng(dev());
	uniform_int_distribution<std::mt19937::result_type> dist3(1, 3);

	int good = dist3(rng); // ranges betweeb (1,3)
	int multiplicity = dist3(rng); // ranges between (1,3)

	if (type == "PlaceNewArmiesCard")
		return new PlaceNewArmiesCard(good, Action(0, multiplicity, false));
	else if (type == "MoveArmiesLandCard")
		return new MoveArmiesLandCard(good, Action(1, multiplicity, false));
	else if (type == "MoveArmiesLandWaterCard")
		return new MoveArmiesLandWaterCard(good, Action(1, multiplicity, true));
	else if (type == "BuildCityCard")
		return new BuildCityCard(good, Action(2));
	else if (type == "DestroyArmyCard")
		return new DestroyArmyCard(good, Action(3, multiplicity));
	else if (type == "AndActionCard")
		return new AndActionCard(good, Action(4, multiplicity, false, 0, 2, 1, false));
	else if (type == "Ignore1Card")
		return new Ignore1Card(good, Action(5, multiplicity, false, 0, 1, 1, true));
	else if (type == "Ignore2Card")
		return new Ignore2Card(good, Action(6, multiplicity));
}