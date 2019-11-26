#pragma once
#include <iostream>
#include <unordered_map>
#include "Map.h"
#include "Cards.h"


class Game;
class PlayerStrategies;

using namespace std;


class Holdings
{
	friend class Player;

public:
	Holdings();
	~Holdings();

private:
	int mNumArmies;
	int mNumCities;

public:
	friend std::ostream& operator<<(std::ostream&, const Holdings&);
	int numArmies();
	int numCities();
	int addArmies(int n);
	int addCities(int n);
	int removeArmies(int n);

};

class Player
{
public:
	Player();
	Player(PlayerStrategies* newStrategy);
	Player(std::string* name, int age);
	Player(int money, int ownedCountries[], int ownedCities[]);
	Player(std::string* name, int age, int money, int ownedCountries[], int ownedCities[]);
	~Player();
	int getMoney();
	string getName();
	void setName(std::string s);
	int* getOwnedCountries();
	int* getOwnedCities();
	int getAge();
	void setAge(int age);
	void PayCoin(int amount);
	void setCoinPurse(int amount);
	Holdings* getHoldings(GraphWorld::Country*);
	std::unordered_map<int, Holdings*>& holdings();
	int computeScore();
	friend std::ostream& operator<<(std::ostream&, const Player&);
	void updateCountryPoints(int points);
	void updateGoodsPoints(int points);
	void updateContinentPoints(int points);
	void setCardToPlay(int cardsToPlay);
	string getStrategy();
	void setHand(Card*);
	Card* getHand();
	int getCardsToPlay();
	int getVictoryPoints();
	int getNumArmies();
	int getNumCities();
	int setArmies(int);
	int getArmies();
	int setCities(int);
	std::vector<GraphWorld::Country*>& countriesWithCities();

	//Function using strategy pattern -->

	void setStrategy(PlayerStrategies* newStrategy);
	void pickCard(Game* game, int position);
	void playCard(Game* game);

	void PlaceNewArmies(int numberOfArmies, GraphWorld::Country* country);
	void MoveArmies(GraphWorld::Country* srcCountry, GraphWorld::Country* destCountry);
	void BuildCity(GraphWorld::Country* country);
	void DestroyArmy(Player* player, GraphWorld::Country* country);
	void Ignore();
	void updateListOfOwnedCountries(Game* game);

	// Used in the game statistics.
	vector<GraphWorld::Country*> &getListOfOwnedCountries() { return listOfOwnedCountries; };
	int getNumOfOwnedCountries() { return listOfOwnedCountries.size(); };
	int getNumOfGoods() { return goodsVictoryPoints; };
	int getTotalVictoryPoints() { return totalVictoryPoints; };

private:
	
	std::unordered_map<int, Holdings*> mHoldings;  // How much the player holds on each country (key is the country id, could use country pointers as keys as well)
	std::vector<GraphWorld::Country*> mCountriesWithCities;
	std::string name;
	int* money; //Amount of money that the player holds
	int* age;
	int totalVictoryPoints;
	int goodsVictoryPoints;
	int countryVictoryPoints;
	int continentVictoryPoints;
	int cardsToPlay;
	int sumVictoryPoints();
	int mArmies;
	int mCities;
	Card* hand;

	PlayerStrategies* strategy;

	const static int totalNumberOfCountries = 30;
	int* ownedCountries; //Pointer to an array of Countries
	int* ownedCities; //Pointer to an array of built cities
	vector<GraphWorld::Country*> listOfOwnedCountries;
};

