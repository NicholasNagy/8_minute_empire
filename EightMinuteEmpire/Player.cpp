#include "Player.h"
#include <random>
#include <ctime>



Holdings::Holdings() : mNumArmies(0), mNumCities(0){}

Holdings::~Holdings()
{
}

int Holdings::numArmies()
{
	return mNumArmies;
}

int Holdings::numCities()
{
	return mNumCities;
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


	Holdings* playerHoldings = mHoldings.at(country->getID());

	//Check if the player has a city on the country or it is a starting country

	if ( playerHoldings->mNumCities == 0 && !country->isStartCountry() )
	{
		cout << messageFail << endl;
		return;
	}
	playerHoldings->mNumArmies += numberOfArmies;
	cout << "Added " << numberOfArmies << " Armies to Country: " << country->getID() << " For player: " << this->getName() << endl;
	country->updateOccupyingPlayerScore(playerHoldings->mNumArmies + playerHoldings->mNumCities, this);

}

void Player::MoveArmies(GraphWorld::Map* map, GraphWorld::Country* start, GraphWorld::Country* destination)
{

	Holdings* holdingsOnStartCountry = mHoldings.at(start->getID());

	//Check if the player has armies on the starting country
	if (holdingsOnStartCountry->mNumArmies == 0)
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
	holdingsOnStartCountry->mNumArmies -= 1;
	holdingsOnDestCountry->mNumArmies += 1;
	cout << "Moved " << 1  << " Army from Country: " << start->getID() << " to Country: " << destination->getID() << endl;
	start->updateOccupyingPlayerScore(holdingsOnStartCountry->mNumArmies + holdingsOnStartCountry->mNumCities, this);
	destination->updateOccupyingPlayerScore(holdingsOnDestCountry->mNumArmies + holdingsOnDestCountry->mNumCities, this);
}

bool isValidMovement(int startPosition, int endPosition) {
	return true;
}


void Player::BuildCity(GraphWorld::Country* country) 
{
	//TODO: check if the player still has any armies left to place (if numCountries > 0 )
	Holdings* countryHoldings = getHoldings(country);
	countryHoldings->mNumCities++;
	cout << "Built " << 1 << " City on Country: " << country->getID() << endl;
	country->updateOccupyingPlayerScore(countryHoldings->mNumArmies + countryHoldings->mNumCities, this);
}


void Player::DestroyArmy(GraphWorld::Country* country)
{
	Holdings* countryHoldings = getHoldings(country);
	int armies = countryHoldings->mNumArmies;

	if (armies != 0)
	{
		getHoldings(country)->mNumArmies--;
	cout << "Destroyed " << 1 << " Army on Country: " << country->getID() << endl;
	country->updateOccupyingPlayerScore(countryHoldings->mNumArmies + countryHoldings->mNumCities, this);
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

int Player::computeScore(GraphWorld::Map* map)
{

	GraphWorld::Country* country;
	Player* countryOwner;
	for (int i = 0; i < map->getNumCountries(); i++)
	{
		country = map->getCountry(i);
		countryOwner = country->getCountryOwner();
	auto it = country->occupyingPlayers().begin(); // Top element is the highest points, if tied with second there is no country owner
	auto it2 = it;
	std::advance(it2, 1);

	//for (int i = 0; i < map->getCountry(i)->occupyingPlayers().size(); ++i)
	//{
	//	std::cout << it->first << "--" << it->second->getName() << std::endl;
	//	++it;
	//}

	if (it->first == it2->first)
	{
		if (countryOwner)
		{
			countryOwner->updateCountryPoints(-1);
			country->setCountryOwner(nullptr);
		}

	}

		if (it->first > it2->first)
		{
			country->setCountryOwner(it->second);
		if(!countryOwner)
			it->second->updateCountryPoints(1);
		}
	
	}

	return totalVictoryPoints;
}

void Player::updateCountryPoints(int points)
{
	countryVictoryPoints += points;
	sumVictoryPoints();

}

void Player::updateGoodsPoints(int points)
{
	goodsVictoryPoints += points;
	sumVictoryPoints();
}

void Player::updateContinentPoints(int points)
{
	continentVictoryPoints += points;
	sumVictoryPoints();
}

int Player::sumVictoryPoints()
{
	return totalVictoryPoints = countryVictoryPoints + continentVictoryPoints + goodsVictoryPoints;;
}

std::ostream& operator<<(std::ostream& s, const Holdings& holdings)
{
	return  s << 
		"Number of Cites: " << holdings.mNumCities << std::endl <<
		"Number of Armies: " << holdings.mNumArmies << std::endl;
}

std::ostream& operator<<(std::ostream& s, const Player& player)
{
	return  s << "--" << player.name << "--\n" <<
		"Coins: " << *player.money << std::endl <<
		"Victory Points: " << player.totalVictoryPoints << endl <<
		"Goods: " << player.goodsVictoryPoints << endl <<
		"Countries: " << player.countryVictoryPoints << endl <<
		"Countinents: " << player.continentVictoryPoints << endl << endl;
		;

}

