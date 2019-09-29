#include <iostream>
#include "Player.h"
#include <array>
using namespace std;

void printArrayContents(int anArray[], int length);

int main() {
	int ownedCountries[5] = { 1,2,3,4,5 };
	int ownedCities[5] = { 5, 4, 3, 2, 1};

	Player example(500, ownedCountries, ownedCities);
	
	cout << "Player's money:\n" << example.getMoney();
	example.PayCoin(20);
	cout << "\nPlayer's money after payings 20 coins:\n" << example.getMoney();

	cout << "\nPrinting out owned Countries and Cities\n";
	printArrayContents(example.getOwnedCountries(), 5);
	cout << "\n";
	printArrayContents(example.getOwnedCities(), 5);

	example.PlaceNewArmies(5, 3);

	cout << "\nPlacing 5 armies at position 3\n";

	printArrayContents(example.getOwnedCountries(), 5);

	example.MoveArmies(3, 3, 4);

	cout << "\nMoving 3 armies from position 3 to 4\n";

	printArrayContents(example.getOwnedCountries(), 5);

	cout << "\nMoving over land, 2 armies from position 1, to 0\n";

	example.MoveOverLand(2, 1, 0);

	printArrayContents(example.getOwnedCountries(), 5);

	cout << "\nBuilding a city at position 2\n";

	example.BuildCity(2);

	printArrayContents(example.getOwnedCities(), 5);

	cout << "\nDestroying an army at position 4\n";

	example.DestroyArmy(4);

	printArrayContents(example.getOwnedCountries(), 5);

	cin.get();

	return 0;
};

void printArrayContents(int anArray[], int length) {
	for (int i = 0; i < length; i++) {
		cout << anArray[i] << " ";
	}
}