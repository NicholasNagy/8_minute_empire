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
		string countryBool = "false";
		if (*isStartCountry == 1)
			countryBool = "true";
		string s2 = "StartingCountry: " + countryBool + "\n";
		string s3 = "Continent: " + continent;
		return s1 + s2 + s3;
	}

	Node::Node(Country* country)
	{
		if (country == NULL)
			this->country = NULL;
		else
			this->country = country;
		next = NULL;
		previous = NULL;
	}

	Node::Node(Country* country, Node* next, Node* previous)
	{
		this->country = country;
		this->next = next;
		this->previous = previous;
	}

	Node* Node::getNext()
	{
		return next;
	}

	Node* Node::getPrev()
	{
		return previous;
	}

	void Node::setNext(Node* addMe)
	{
		next = addMe;
	}

	void Node::setPrev(Node* addMe)
	{
		previous = addMe;
	}

	Node::~Node()
	{
		delete country;
	}

	void Node::displayNode()
	{
		cout << "Country:\n" + country->displayCountry() + "\n";
	}

	LinkedList::LinkedList(Country* country)
	{
		if (country == NULL)
		{
			head = NULL;
			tail = NULL;
		}
		else
		{
			head = new Node(country);
			tail = head;
			size++;
		}
	}

	Node* LinkedList::getNext(Node* current)
	{
		return current->getNext();
	}

	Node* LinkedList::getPrev(Node* current)
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

	// Continue working Here
	bool LinkedList::add(int where, Country* country)
	{
		bool returnMe = false;
		if (country != NULL)
		{
			if (where == 0)
				addToHead(country);
			else if (where >= size)
				addToTail(country);
			else
			{
				
				for (int i = 0; i < where; i++)
				{

				}
			}
		}
		else
		{
			cout << "You tried to add nothing";
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

