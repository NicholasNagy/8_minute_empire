#include "Player.h"
#include <random>
#include <ctime>
#include <string>
#include <typeinfo>
#include "PlayerStrategies.h"

using GraphWorld::Map;
using GraphWorld::Country;

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

int Holdings::addArmies(int n)
{
	return mNumArmies += n;
}

int Holdings::addCities(int n)
{
	return mNumCities += n;
}

int Holdings::removeArmies(int n)
{
	mNumArmies -= n;
	if (mNumArmies < 0)
		mNumArmies = 0;

	return mNumArmies;
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
	mArmies = 14;
	mCities = 3;
	hand = nullptr;
}

Player::Player(PlayerStrategies* newStrategy)
{
	strategy = newStrategy;
	name = "Default";
	money = new int(20);
	//totalNumberOfCountries = 30;
	ownedCities = nullptr;
	ownedCountries = nullptr;
	srand(time(NULL));
	age = new int(rand() % 100 + 1);
	mArmies = 14;
	mCities = 3;
	hand = nullptr;

}

Player::Player(std::string* name, int age)
{
	money = new int(20);
	this->name = std::string(*name);
	this->age = new int(age);
	mArmies = 14;
	mCities = 3;
	hand = nullptr;
}

Player::Player(int theMoney, int theOwnedCountries[], int theOwnedCities[]) {

	name = "Default";
	money = new int(theMoney);
	ownedCities = theOwnedCities;
	ownedCountries = theOwnedCountries;
	srand(time(NULL));
	age = new int(rand() % 100 + 1);
	mArmies = 14;
	mCities = 3;
	hand = nullptr;
}

Player::Player(std::string* name, int age, int theMoney, int theOwnedCountries[], int theOwnedCities[])
{
	this->name = std::string(*name);
	*this->age = age;
	money = new int(theMoney);
	ownedCities = theOwnedCities;
	ownedCountries = theOwnedCountries;
	mArmies = 14;
	mCities = 3;
	hand = nullptr;
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

void Player::PlaceNewArmies(int numberOfArmies,GraphWorld::Country* country)
{

	strategy->PlaceNewArmies(numberOfArmies, country);

}

void Player::MoveArmies(GraphWorld::Country* start, GraphWorld::Country* destination)
{
	strategy->MoveArmies(start, destination);
	
}

bool isValidMovement(int startPosition, int endPosition) {
	return true;
}


void Player::BuildCity(GraphWorld::Country* country) 
{
	strategy->BuildCity(country);

}


void Player::DestroyArmy(Player* playerToDestroy, GraphWorld::Country* country)
{

	strategy->DestroyArmy(playerToDestroy, country);
	
}

void Player::Ignore()
{
	strategy->Ignore();
}

void Player::updateListOfOwnedCountries(Game* game)
{
	int numCountriesInMap = Map::instance()->getNumCountries();
	listOfOwnedCountries.clear();
	for (int i = 0; i < numCountriesInMap; i++) {
		
		Country* country = Map::instance()->getCountry(i); // get the country at postion i

		// if the owner of the country is this player, add it to the list of owned countries
		if (country->getCountryOwner() == this) {
			listOfOwnedCountries.push_back(country);
		}
	}
}

Holdings* Player::getHoldings(GraphWorld::Country* country)
{
	return mHoldings.at(country->getID());
}

std::unordered_map<int, Holdings*>& Player::holdings()
{
	return mHoldings;
}

int Player::computeScore()
{

	GraphWorld::Country* country;
	Player* countryOwner;
	for (int i = 0; i < Map::instance()->getNumCountries(); i++)
	{
		country = Map::instance()->getCountry(i);
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

void Player::setCardToPlay(int n)
{
	cardsToPlay = n;
}

string Player::getStrategy()
{
	string s = typeid(*strategy).name();
	return s.substr(6);
}


void Player::setHand(Card* h)
{
	hand = h;
}

Card* Player::getHand()
{
	return hand;
}

int Player::getCardsToPlay()
{
	return cardsToPlay;
}

int Player::getVictoryPoints()
{
	return totalVictoryPoints;
}

int Player::getNumArmies()
{
	return mArmies;
}

int Player::getNumCities()
{
	return mCities;
}

int Player::setArmies(int n )
{
	if (n < 0)
		mArmies = 0;

	return mArmies = n;
}

int Player::getArmies()
{
	return mArmies;
}

int Player::setCities(int n)
{
	if (n < 0)
		mCities = 0;

	return mCities = n;
}

std::vector<GraphWorld::Country*>& Player::countriesWithCities()
{
	return mCountriesWithCities;
}

void Player::setStrategy(PlayerStrategies* newStrategy)
{
	strategy = newStrategy;
}

void Player::pickCard(Game* game, int position)
{
	strategy->pickCard(game, position);
}

void Player::playCard(Game* game)
{
	strategy->playCard(game);	
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

	string strategy = typeid(*player.strategy).name();

		return  s << "--" << player.name << " (" << strategy.substr(6) << ")--\n" <<
		"Coins: " << *player.money << std::endl <<
		"Cards Left to play: " << player.cardsToPlay << std::endl <<
		"Armies Left: " << player.mArmies << endl <<
		"Cities Left: " << player.mCities << endl <<
		"Victory Points: " << player.totalVictoryPoints << endl <<
		"Goods: " << player.goodsVictoryPoints << endl <<
		"Countries: " << player.countryVictoryPoints << endl <<
		"Countinents: " << player.continentVictoryPoints << endl << endl;

}

