#pragma once
class Player
{
public:
	Player();
	~Player();
	void PayCoin(int amount);
	void PlaceNewArmies(int numberOfArmies, int Country);
	void MoveArmies(int numberOfArmies, int StartPosition, int EndPosition);
	void MoveOverLand(int numberOfArmies, int StartPosition, int EndPosition);
	void BuildCity(int Position);
	void DestroyArmy(int Position);

private:
	int *ownedCountries; //Pointer to an array of Countries
	int *ownedCities; //Pointer to an array of built cities
	int money; //Amount of money that the player holds

};

