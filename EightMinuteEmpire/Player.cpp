#include "Player.h"

Player::Player()
{
	money = 1000;
	//totalNumberOfCountries = 30;
	ownedCities = nullptr;
	ownedCountries = nullptr;
}

Player::Player(int theMoney, int theOwnedCountries[], int theOwnedCities[]) {
	money = theMoney;
	ownedCities = theOwnedCities;
	ownedCountries = theOwnedCountries;
}


Player::~Player()
{
	//delete[]s;
}

int Player::getMoney() {
	return int(money);
}

int* Player::getOwnedCities() {
	return ownedCities;
}

int* Player::getOwnedCountries() {
	return ownedCountries;
}

void Player::PayCoin(int amount) {
	money -= amount;
}

void Player::PlaceNewArmies(int numberOfArmies, int Country) {
	//int *theCountries[totalNumberOfCountries];
//	int* theCountries;
//	theCountries= ownedCountries;
	int numberOfArmiesHeld = ownedCountries[Country];
	numberOfArmiesHeld += numberOfArmies;
	ownedCountries[Country] = numberOfArmiesHeld;
}

void Player::MoveArmies(int numberOfArmies, int StartPosition, int EndPosition) {
	int* theCountries;
	theCountries = ownedCountries;
	int armiesAtStart = ownedCountries[StartPosition];
	//if the number of armies to move from the start is more than the amount of armies already 
	//at the starting position
	if (armiesAtStart - numberOfArmies < 0) { 
		numberOfArmies = armiesAtStart;
		armiesAtStart = 0;
	}
	else {
		armiesAtStart -= numberOfArmies;
	}

	int armiesAtEnd = ownedCountries[EndPosition];
	armiesAtEnd += numberOfArmies;
	
	ownedCountries[StartPosition] = armiesAtStart;
	ownedCountries[EndPosition] = armiesAtEnd;
}

bool isValidMovement(int startPosition, int endPosition) {
	return true;
}

void Player::MoveOverLand(int numberOfArmies, int StartPosition, int EndPosition) {
	if (isValidMovement( StartPosition, EndPosition )){
		MoveArmies(numberOfArmies, StartPosition, EndPosition);
	}
	else {
		// handle cannot move armies
	}
}

void Player::BuildCity(int position) {
	int cities = ownedCities[position];
	cities += 1;
	ownedCities[position] = cities;
}

void Player::DestroyArmy(int position) {
	int armiesHeld = ownedCountries[position];
	if (armiesHeld > 0) {
		armiesHeld -= 1;
	}
	else {
		//handle when armies is less than or equal to 0
	}
	ownedCountries[position] = armiesHeld;
	
}


