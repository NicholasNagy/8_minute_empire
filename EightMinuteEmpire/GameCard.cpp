#pragma once

#include "GameCard.h"

PlaceNewArmiesCard::PlaceNewArmiesCard(const int myGood, const Action& myAction) : Card(myGood, myAction) {}
PlaceNewArmiesCard::~PlaceNewArmiesCard() {}

MoveArmiesLandCard::MoveArmiesLandCard(const int myGood, const Action& myAction) : Card(myGood, myAction) {}
MoveArmiesLandCard::~MoveArmiesLandCard() {}

MoveArmiesLandWaterCard::MoveArmiesLandWaterCard(const int myGood, const Action& myAction) : Card(myGood, myAction) {}
MoveArmiesLandWaterCard::~MoveArmiesLandWaterCard() {}

BuildCityCard::BuildCityCard(const int myGood, const Action& myAction) : Card(myGood, myAction) {}
BuildCityCard::~BuildCityCard() {}

DestroyArmyCard::DestroyArmyCard(const int myGood, const Action& myAction) : Card(myGood, myAction) {}
DestroyArmyCard::~DestroyArmyCard() {}

AndActionCard::AndActionCard(const int myGood, const Action& myAction) : Card(myGood, myAction) {}
AndActionCard::~AndActionCard() {}

Ignore1Card::Ignore1Card(const int myGood, const Action& myAction) : Card(myGood, myAction) {}
Ignore1Card::~Ignore1Card() {}

Ignore2Card::Ignore2Card(const int myGood, const Action& myAction) : Card(myGood, myAction) {}
Ignore2Card::~Ignore2Card() {}