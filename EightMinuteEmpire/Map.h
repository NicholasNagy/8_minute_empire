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
		// match click to country specified by ID
		// print adjacent countries / list
		// match/go to one of the countries in adjacent list
		class Node;
		string mapName;
		int* size;
		int* numCountries;
		int* numContinents;
		typedef Node* NodePtr;
		NodePtr* arrayOfCountries;

		public:
			Map(string* mapName, int numCountries, int numContinents);
			void addNode(Country* country);
			void addEdge(Country* currentCountry, Country* adjacentCountry);
			Country* getCountry(int id);
			LinkedList* getAdjacentList(Country* country);
			void printMap();
			~Map();
	};

	class Country
	{
		int* country_ID;
		bool* isStartCountry;
		string continent;

		public:
			Country();
			Country(int country_ID, bool startCountry, string* continent);
			int getID();
			bool getStartCountry();
			string getContinent();
			void setID(int id);
			void setStartCountry(bool maybe);
			void setContinent(string* continent);
			string displayCountry();
			~Country();
	};

	class LinkedList
	{
		class Node;
		int* size;
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
			Country* get(int where);
			Country* remove(int where);
			void displayLinkedList();
			~LinkedList();
	};
}

#endif

