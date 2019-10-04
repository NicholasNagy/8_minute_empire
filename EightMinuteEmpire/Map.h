#ifndef MAP_H
#define MAP_H

#include <string>

using std::string;

namespace GraphWorld
{
	class Map;
	class LinkedList;
	class Country;

	class Map
	{
		string mapName;
		int* numberOfCountries;
		int* numberOfContinents;

		public:
			Map();
			~Map();
	};

	class Country
	{
		int* country_ID;
		bool* isStartCountry;
		string continent;
		//LinkedList* adjacentCountries;

		public:
			Country(int country_ID, bool startCountry, string* continent);
			string displayCountry();
			~Country();
	};

	class LinkedList
	{
		class Node;
		int size;
		Node* head;
		Node* tail;
		Node* getNext(Node* current);
		Node* getPrev(Node* current);
		Country* removeHead();
		Country* removeTail();
		bool addToHead(Country* country);
		bool addToTail(Country* country);

		public:
			LinkedList(Country* country);
			int sizeOf();
			bool add(int where, Country* country);
			Country* remove(int where);
			void displayLinkedList();
			~LinkedList();
	};
}

#endif

