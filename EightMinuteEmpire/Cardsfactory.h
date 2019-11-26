#pragma once

#include "Cards.h";

using std::string;

class CardsFactory
{
	public:
		CardsFactory();
		~CardsFactory();
		Card* makeCard(string type);
};