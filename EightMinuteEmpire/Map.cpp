#include "Map.h"
#include <string>

using std::string;

namespace GraphWorld
{
	Map::Map()
	{
	}

	Map::~Map()
	{
	}

	Country::Country(int country_ID, bool startCountry, char* continent, list<Country*> adjacentCountries)
	{
		this->country_ID = country_ID;
		this->isStartCountry = startCountry;
		//this->continent = malloc(1 + strlen(continent));
		//strcpy(this->continent, continent);
		this->adjacentCountries = adjacentCountries;
	}

	Country::~Country()
	{
		//delete continent;
		adjacentCountries.clear();
	}

	string Country::displayCountry()
	{

	}

	LinkedList::LinkedList()
	{

	}

	LinkedList::~LinkedList()
	{

	}

	Node::Node(Country *country)
	{
		this->country = country;
	}

	Node::~Node()
	{

	}
}

