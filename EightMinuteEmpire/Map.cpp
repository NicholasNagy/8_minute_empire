#include "Map.h"
#include <string>

using std::string;
using std::to_string;

namespace GraphWorld
{
	Map::Map()
	{
	}

	Map::~Map()
	{
	}

	Country::Country(int country_ID, bool startCountry, string* continent) // list<Country*> adjacentCountries
	{
		this->country_ID = new int();
		*this->country_ID = country_ID;
		this->isStartCountry = new bool();
		*this->isStartCountry = startCountry;
		this->continent = string(*continent);
		//this->adjacentCountries = adjacentCountries;
	}

	Country::~Country()
	{
		delete country_ID;
		delete isStartCountry;
		continent.clear();
		//adjacentCountries.clear();
	}

	string Country::displayCountry()
	{
		string s1 = "countryID: " + to_string(*country_ID) + "\n";
		string s2 = "StartingCountry: " + to_string(*isStartCountry) + "\n";
		string s3 = "Continent: " + continent;
		return s1 + s2 + s3;
	}

	Node::Node(Country* country)
	{
		this->country = country;
	}

	Node::Node(Country* country, Node* next, Node* previous)
	{
		this->country = country;
		this->next = next;
		this->previous = previous;
	}

	Node::~Node()
	{
		delete country;
	}

	string Node::displayNode()
	{
		return "Country: " + country->displayCountry();
	}

	/*
	LinkedList::LinkedList()
	{

	}

	LinkedList::~LinkedList()
	{

	}
	*/
}

