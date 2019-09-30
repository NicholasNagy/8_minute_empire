#ifndef MAP_H
#define MAP_H

#include <string>
#include <list>

using std::string;
using std::list;

namespace GraphWorld
{
	class Map
	{
		public:
			Map();
			~Map();

		private:
			string* mapName;
			int numberOfCountries;
			int numberOfContinents;
	};

	class Country
	{
		public:
			Country(int country_ID, bool startCountry, char* continent, list<Country*> adjacentCountries);
			~Country();
			string displayCountry();

		private:
			int country_ID;
			bool isStartCountry;
			char continent;
			list<Country*> adjacentCountries;
	};

	class LinkedList
	{
		public:
			LinkedList();
			~LinkedList();

		private:
			int size;
			Node* head;
	};

	class Node
	{
		public:
			Node(Country *country);
			~Node();

		private:
			Country* country;
			Node* next;
			Node* previous;
	};
}

#endif

