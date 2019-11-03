#pragma once
#include <iostream>
#include <unordered_map>
#include "Map.h"
using namespace std;

class Holdings
{
	friend class Player;

public:
	Holdings();
	~Holdings();

private:
	int numArmies;
	int numCities;

public:
	friend std::ostream& operator<<(std::ostream&, const Holdings&);

};

class Player
{
public:
	Player();
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
	void PlaceNewArmies(int numberOfArmies, GraphWorld::Country* country);
	void MoveArmies(GraphWorld::Map*, GraphWorld::Country* srcCountry, GraphWorld::Country* destCountry);
	void BuildCity(GraphWorld::Country* country);
	void DestroyArmy(GraphWorld::Country* country);
	Holdings* getHoldings(GraphWorld::Country*);
	std::unordered_map<int, Holdings*>& holdings(); // How much the player holds on each country (key is the country id, could use country pointers ask keys as well)
	friend std::ostream& operator<<(std::ostream&, const Player&);

private:
	std::unordered_map<int, Holdings*> mHoldings;
	std::string name;
	int* ownedCountries; //Pointer to an array of Countries
	int* ownedCities; //Pointer to an array of built cities
	int* money; //Amount of money that the player holds
	int* age;
	const static int totalNumberOfCountries = 30;
};

