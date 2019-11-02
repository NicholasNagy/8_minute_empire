#include "Player.h"
#include <random>
#include <ctime>



Holdings::Holdings() : numArmies(0), numCities(0){}

Holdings::~Holdings()
{
}



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

void Player::setName(std::string s)
{
	name = s;
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

void Player::setAge(int age)
{
	*this->age = age;
}

void Player::PayCoin(int amount) {
	*money -= amount;
}

void Player::setCoinPurse(int amount)
{
	*money = amount;
}

void Player::PlaceNewArmies(int numberOfArmies,GraphWorld::Country* country) {

	string messageFail = "Cannot place a new army on country " + to_string(country->getID());
	
	if ( (country->occupyingPlayers().find(this) == country->occupyingPlayers().end()) )
	{
		cout << messageFail << endl;
		return;
	}

	Holdings* playerHoldings = mHoldings.at(country->getID());

	//Check if the player has a city on the country or it is a starting country

	if ( playerHoldings->numCities == 0 && !country->isStartCountry() )
	{
		cout << messageFail << endl;
		return;
	}
	cout << "Adding " << numberOfArmies << " Armies to Country: " << country->getID() << " For player: " << this->getName() << endl;
	playerHoldings->numArmies += numberOfArmies;

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

Holdings* Player::getHoldings(GraphWorld::Country* country, int maxNumCountries)
{
	int countryID = country->getID();
	if (countryID < 0 || countryID > maxNumCountries)
		return nullptr;

	return mHoldings.at(country->getID());
}

std::unordered_map<int, Holdings*>& Player::holdings()
{
	return mHoldings;
}

std::ostream& operator<<(std::ostream& s, const Holdings& holdings)
{
	return  s << 
		"Number of Cites: " << holdings.numCities << std::endl <<
		"Number of Armies: " << holdings.numArmies << std::endl;
}

std::ostream& operator<<(std::ostream& s, const Player& player)
{
	return  s << "--Player Info--\n" <<
		"Name: " << player.name << std::endl <<
		"Age: " << *player.age << std::endl <<
		"Money: " << *player.money << std::endl;

}

