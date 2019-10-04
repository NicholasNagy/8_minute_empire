#include <string>
#include <iostream>
#include "Map.h"

using std::string;
using std::cout;
using std::to_string;

namespace GraphWorld
{
	Map::Map()
	{
	}

	Map::~Map()
	{
	}

	Country::Country(int country_ID, bool startCountry, string* continent)
	{
		this->country_ID = new int();
		*this->country_ID = country_ID;
		this->isStartCountry = new bool();
		*this->isStartCountry = startCountry;
		this->continent = string(*continent);
	}

	string Country::displayCountry()
	{
		string s1 = "countryID: " + to_string(*country_ID) + "\n";
		string countryBool = "false";
		if (*isStartCountry == 1)
			countryBool = "true";
		string s2 = "StartingCountry: " + countryBool + "\n";
		string s3 = "Continent: " + continent + "\n";
		string s4 = "AdjacentCountries: ";
		return s1 + s2 + s3;
	}

	Country::~Country()
	{
		delete country_ID;
		delete isStartCountry;
		continent.clear();
		//delete adjacentCountries;
	}

	class LinkedList::Node
	{
		Country* country;
		Node* next;
		Node* previous;

		public:
			Node(Country* country)
			{
				if (country == NULL)
					this->country = NULL;
				else
					this->country = country;
				next = NULL;
				previous = NULL;
			}

			Node(Country* country, Node* next, Node* previous)
			{
				this->country = country;
				this->next = next;
				this->previous = previous;
			}

			Node* getNext()
			{
				return next;
			}

			Node* getPrev()
			{
				return previous;
			}

			void setNext(Node* addMe)
			{
				next = addMe;
			}

			void setPrev(Node* addMe)
			{
				previous = addMe;
			}

			Country* getCountry()
			{
				return country;
			}

			void displayNode()
			{
				cout << "Country:\n" + country->displayCountry() + "\n";
			}

			~Node()
			{
				delete country;
			}
	};

	LinkedList::LinkedList(Country* country)
	{
		if (country == NULL)
		{
			head = NULL;
			tail = NULL;
			size = 0;
		}
		else
		{
			head = new Node(country);
			tail = head;
			size++;
		}
	}

	LinkedList::Node* LinkedList::getNext(Node* current)
	{
		return current->getNext();
	}

	LinkedList::Node* LinkedList::getPrev(Node* current)
	{
		return current->getPrev();
	}

	int LinkedList::sizeOf()
	{
		return size;
	}

	bool LinkedList::addToHead(Country* country)
	{
		bool returnMe = false;
		if (country != NULL)
		{
			if (head == NULL)
			{
				head = new Node(country);
				tail = head;
			}
			else
			{
				Node* n1 = new Node(country, head, NULL);
				head->setPrev(n1);
				head = n1;
			}
			size++;
			returnMe = true;
		}
		else
		{
			cout << "You tried to add nothing";
		}
		return returnMe;
	}

	bool LinkedList::addToTail(Country* country)
	{
		bool returnMe = false;
		if (country != NULL)
		{
			if (tail == NULL)
			{
				tail = new Node(country);
				head = tail;
			}
			else
			{
				Node* n1 = new Node(country, NULL, tail);
				tail->setNext(n1);
				tail = n1;
			}
			size++;
			returnMe = true;
		}
		else
		{
			cout << "You tried to add nothing";
		}
		return returnMe;
	}

	bool LinkedList::add(int where, Country* country)
	{
		bool returnMe = false;
		if (country != NULL)
		{
			if (where == 0)
				returnMe = addToHead(country);
			else if (where >= size)
				returnMe = addToTail(country);
			else
			{
				Node* current = head;
				for (int i = 0; i < where; i++)
				{
					current = current->getNext();
				}
				Node* prev = current->getPrev();
				Node* n1 = new Node(country, current, prev);
				prev->setNext(n1);
				current->setPrev(n1);
				size++;
				returnMe = true;
			}
		}
		else
		{
			cout << "You tried to add nothing";
		}
		return returnMe;
	}

	Country* LinkedList::removeHead()
	{
		if (size == 0)
			return NULL;
		else
		{
			Node* returnMe = head;
			head = head->getNext();
			head->setPrev(NULL);
			size--;
			return returnMe->getCountry();
		}
	}

	Country* LinkedList::removeTail()
	{
		if (size == 0)
			return NULL;
		else
		{
			Node* returnMe = tail;
			tail = tail->getPrev();
			tail->setNext(NULL);
			size--;
			return returnMe->getCountry();
		}
	}

	Country* LinkedList::remove(int where)
	{
		if (where == 0)
			return removeHead();
		else if (where >= size)
			return removeTail();
		else
		{
			Node* current = head;
			for (int i = 0; i < where; i++)
			{
				current = current->getNext();
			}
			Node* prev = current->getPrev();
			Node* next = current->getNext();
			prev->setNext(next);
			next->setPrev(prev);
			size--;
			return current->getCountry();
		}
	}

	void LinkedList::displayLinkedList()
	{
		if (size == 0)
			cout << "Nothing in the Linked List";
		else
		{
			cout << "size: " + to_string(size) + "\n";
			Node* toDisplay = this->head;
			for (int i = 0; i < size; i++)
			{
				toDisplay->displayNode();
				toDisplay = this->getNext(toDisplay);
			}
		}
	}

	LinkedList::~LinkedList()
	{
		Node* current = this->head;
		Node* next;
		for (int i = 0; i < size; i++)
		{
			next = current->getNext();
			delete current;
			current = next;
		}
		size = 0;
	}
}

