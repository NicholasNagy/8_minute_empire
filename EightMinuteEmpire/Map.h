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
			int getSize();
			~Map();
	};

	class Country
	{
		int* country_ID;
		bool* mIsStartCountry;
		bool* mIsNavalCountry;
		string continent;

		public:
			Country();
			Country(const Country&);
			Country(int country_ID, bool startCountry, bool navalCountry, string* continent);
			int getID();
			bool isStartCountry();
			bool isNavalCountry();
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

