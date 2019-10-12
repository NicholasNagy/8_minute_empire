#pragma once
#include <iostream>
using namespace std;

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
	int* getOwnedCountries();
	int* getOwnedCities();
	int getAge();
	void PayCoin(int amount);
	void PlaceNewArmies(int numberOfArmies, int Country);
	void MoveArmies(int numberOfArmies, int StartPosition, int EndPosition);
	void MoveOverLand(int numberOfArmies, int StartPosition, int EndPosition);
	void BuildCity(int Position);
	void DestroyArmy(int Position);
	friend std::ostream& operator<<(std::ostream&, const Player&);

private:
	std::string name;
	int* ownedCountries; //Pointer to an array of Countries
	int* ownedCities; //Pointer to an array of built cities
	int* money; //Amount of money that the player holds
	int* age;
	const static int totalNumberOfCountries = 30;
};
