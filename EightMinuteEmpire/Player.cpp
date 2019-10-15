#include "Player.h"
#include <random>
#include <ctime>

Player::Player()
{
	name = "Default";
	money = new int(20);
	//totalNumberOfCountries = 30;
	ownedCities = nullptr;
	ownedCountries = nullptr;
	srand(time(NULL));
	age = new int(rand() % 100 + 1);
}

Player::Player(std::string* name, int age)
{
	money = new int(20);
	this->name = std::string(*name);
	this->age = new int(age);
}

Player::Player(int theMoney, int theOwnedCountries[], int theOwnedCities[]) {

	name = "Default";
	money = new int(theMoney);
	ownedCities = theOwnedCities;
	ownedCountries = theOwnedCountries;
	srand(time(NULL));
	age = new int(rand() % 100 + 1);
}

Player::Player(std::string* name, int age, int theMoney, int theOwnedCountries[], int theOwnedCities[])
{
	this->name = std::string(*name);
	*this->age = age;
	money = new int(theMoney);
	ownedCities = theOwnedCities;
	ownedCountries = theOwnedCountries;
}

Player::~Player()
{
	//delete this;
}

int Player::getMoney() {
	return *money;
}

string Player::getName()
{
	return name;
}

int* Player::getOwnedCities() {
	return ownedCities;
}

int* Player::getOwnedCountries() {
	return ownedCountries;
}

int Player::getAge()
{
	return *age;
}

void Player::PayCoin(int amount) {
	*money -= amount;
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
	if (isValidMovement(StartPosition, EndPosition)) {
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

std::ostream& operator<<(std::ostream& s, const Player& player)
{
	return  s << "--Player Info--\n" <<
		"Name: " << player.name << std::endl <<
		"Age: " << *player.age << std::endl <<
		"Money: " << *player.money << std::endl;

}


