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
	playerHoldings->numArmies += numberOfArmies;
	cout << "Added " << numberOfArmies << " Armies to Country: " << country->getID() << " For player: " << this->getName() << endl;

}

void Player::MoveArmies(GraphWorld::Map* map, GraphWorld::Country* start, GraphWorld::Country* destination)
{

	Holdings* holdingsOnStartCountry = mHoldings.at(start->getID());

	//Check if the player has armies on the starting country
	if (holdingsOnStartCountry->numArmies == 0)
	{
		cout << "\nNo armies to move." << endl;
		return;
	}
	
	//Check if the start and destination countries are adjacent to each other
	if (!map->getAdjacentList(start)->isAdjacent(destination))
	{
		cout << "Cannot move armies there." << endl;
		return;
	}
	
	//Check if the move requires naval
	if (start->isNavalCountry() && destination->isNavalCountry())
	{
		cout << "Cannot move armies there. Requires Naval movement." << endl;
		return;
	}

	//Move armies
	Holdings* holdingsOnDestCountry = mHoldings.at(destination->getID());
	holdingsOnStartCountry->numArmies -= 1;
	holdingsOnDestCountry->numArmies += 1;
	cout << "Moved " << 1  << " Army from Country: " << start->getID() << " to Country: " << destination->getID() << endl;


}

bool isValidMovement(int startPosition, int endPosition) {
	return true;
}


void Player::BuildCity(GraphWorld::Country* country) 
{
	//TODO: check if the player still has any armies left to place (if numCountries > 0 )
	getHoldings(country)->numCities++;
	cout << "Built " << 1 << " City on Country: " << country->getID() << endl;
}


void Player::DestroyArmy(GraphWorld::Country* country)
{
	int armies = getHoldings(country)->numArmies;

	if (armies != 0)
	{
		getHoldings(country)->numArmies--;
	cout << "Destroyed " << 1 << " Army on Country: " << country->getID() << endl;
	}
	else
		cout << "No armies to Destroy!\n" << endl;
}

Holdings* Player::getHoldings(GraphWorld::Country* country)
{
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

