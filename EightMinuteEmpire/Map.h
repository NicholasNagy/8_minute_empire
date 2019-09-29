#pragma once
#include <string>
#include <list>
class Map
{
public:
	Map();
	~Map();


private:
	std::string* mapName;
	int* numberOfCountries;
	int* numberOfContinents;


};

class Country
{
public:
	Country();
	~Country();


private:
	int* country_ID;
	bool* isStartingCountry;
	std::string* continent; 
	std::list<Country*> adjacentCountries; 

	
};

