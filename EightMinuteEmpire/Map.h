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
			string mapName;
			int* numberOfCountries;
			int* numberOfContinents;
	};

	class Country
	{
		public:
			Country(int country_ID, bool startCountry, string* continent);// list<Country*> adjacentCountries
			~Country();
			string displayCountry();

		private:
			int* country_ID;
			bool* isStartCountry;
			string continent;
			list<Country*> adjacentCountries;
	};

	class Node
	{
		public:
			Node(Country* country);
			Node(Country* country, Node* next, Node* previous);
			Node* getNext();
			Node* getPrev();
			void setNext(Node* addMe);
			void setPrev(Node* addMe);
			Country* getCountry();
			~Node();
			void displayNode();

		private:
			Country* country;
			Node* next;
			Node* previous;
	};

	class LinkedList
	{
		public:
			LinkedList(Country* country);
			int sizeOf();
			bool add(int where, Country* country);
			Country* remove(int where);
			void displayLinkedList();
			~LinkedList();

		private:
			int size;
			Node* head;
			Node* tail;
			Node* getNext(Node* current);
			Node* getPrev(Node* current);
			Country* removeHead();
			Country* removeTail();
			bool addToHead(Country* country);
			bool addToTail(Country* country);
	};
}

#endif

