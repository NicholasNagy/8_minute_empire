#include <string>
#include <iostream>
#include "Map.h"
#include "TextureLoader.h"


using std::string;
using std::cout;
using std::to_string;

namespace GraphWorld
{
	class Map::Node
	{
		Country* country;
		LinkedList* adjacencyList;

	public:
		Node()
		{
			country = NULL;
			adjacencyList = new LinkedList(NULL);
		}

		void setCountry(Country* country)
		{
			this->country = country;
		}

		void addAdjacentCountry(Country* country, bool naval)
		{
			adjacencyList->add(0, country, naval);
		}

		Country* getCountry()
		{
			return country;
		}

		LinkedList* getAdjacencyList()
		{
			return adjacencyList;
		}

		void displayNode()
		{
			cout << "COUNTRY IN NODE:\n";
			if (country != NULL)
			{
				cout << country->displayCountry() + "";
				cout << "--Adjacent List--\n";
				adjacencyList->displayLinkedList();
			}
			else
				cout << "Nothing in the Node!\n";
		}

		~Node()
		{
			delete country;
			delete adjacencyList;
		}
	};

	Map::Map(string* mapName, int numCountries, int numContinents)
	{

		this->mapName = string(*mapName);
		this->numCountries = new int(numCountries);
		this->numContinents = new int(numContinents);
		size = new int(0);

		arrayOfCountries = new NodePtr[numCountries];
		for (int i = 0; i < numCountries; ++i)
		{
			arrayOfCountries[i] = new Node();
		}
	}

	void Map::addNode(Country* country)
	{
		if (*size < *numCountries)
		{
			Node* nodeToModify = arrayOfCountries[country->getID()];
			nodeToModify->setCountry(country);
			*size = *size + 1;
		}
		else
		{
			cout << "Can't add any more nodes to graph!";
		}
	}

	void Map::addEdge(Country* currentCountry, Country* adjacentCountry, bool naval)
	{
		Node* toModify = arrayOfCountries[currentCountry->getID()];
		toModify->addAdjacentCountry(adjacentCountry, naval);
	}

	Country* Map::getCountry(int id)
	{
		if (id < 0)
			return arrayOfCountries[0]->getCountry();
		else if (id >= *numCountries)
			return arrayOfCountries[*numCountries - 1]->getCountry();
		else
			return arrayOfCountries[id]->getCountry();
	}

	string Map::getMapName()
	{
		return mapName;
	}

	int Map::getNumCountries()
	{
		return *numCountries;
	}

	int Map::getNumContinents()
	{
		return *numContinents;
	}

	void Map::setStartingCountry(Country* c)
	{
		startingCountry = c;
	}

	Country* Map::getStartingCountry()
	{
		return startingCountry;
	}

	LinkedList* Map::getAdjacentList(Country* country)
	{
		int nodeNum = country->getID();
		Node* getMe = arrayOfCountries[nodeNum];
		return getMe->getAdjacencyList();
	}

	void Map::printMap()
	{
		cout << "MapName: " + mapName + "\n";
		cout << "Number of Continents: " + to_string(*numContinents) + "\n";
		cout << "Number of Countries: " + to_string(*numCountries) + "\n\n";
		if (*size == 0)
		{
			cout << "MAP is empty!" << std::endl;
		}
		else
		{
			for (int i = 0; i < *size; i++)
			{
				arrayOfCountries[i]->displayNode();
			}
		}
	}

	int Map::getSize()
	{
		return *size;
	}

	TileMap* GraphWorld::Map::getTileMap()
	{
		return tileMap;
	}


	void GraphWorld::Map::setTileMap(TileMap* tm)
	{
		tileMap = tm;
	}


	Map::~Map()
	{
		mapName.clear();
		delete numContinents;
		delete numCountries;
		delete arrayOfCountries;
	}

	Country::Country()
	{
		country_ID = new int();
		*country_ID = 0;
		mIsStartCountry = new bool();
		*mIsStartCountry = false;
		mIsNavalCountry = new bool();
		*mIsNavalCountry = false;
		continent = string("");
	}

	Country::Country(const Country& country2)
	{
		this->country_ID = country2.country_ID;
		this->mIsStartCountry = country2.mIsStartCountry;
		this->mIsNavalCountry = country2.mIsNavalCountry;
		this->continent = country2.continent;

	}

	Country::Country(int country_ID, bool startCountry, bool navalCountry, string* continent)
	{
		this->country_ID = new int();
		*this->country_ID = country_ID;
		this->mIsStartCountry = new bool();
		*this->mIsStartCountry = startCountry;
		this->mIsNavalCountry = new bool();
		*this->mIsNavalCountry = navalCountry;
		this->continent = string(*continent);
	}

	int Country::getID()
	{
		return *country_ID;
	}

	bool Country::isStartCountry()
	{
		return *mIsStartCountry;
	}

	bool Country::isNavalCountry()
	{
		return *mIsNavalCountry;
	}

	string Country::getContinent()
	{
		return continent;
	}

	std::unordered_set<Player*>& Country::occupyingPlayers()
	{
		return mOccupyingPlayers;
	}

	void Country::setID(int id)
	{
		*country_ID = id;
	}

	void Country::setStartCountry(bool maybe)
	{
		*mIsStartCountry = maybe;
	}

	void Country::setContinent(string* continent)
	{
		this->continent.clear();
		this->continent = *continent;
	}

	string Country::displayCountry()
	{
		string s1 = "countryID: " + to_string(*country_ID) + "\n";
		string s2 = (*mIsStartCountry) ? "true\n" : "false\n";
		string s3 = (*mIsNavalCountry) ? "true\n" : "false\n";
		string s4 = "Continent: " + continent + "\n";
		return s1 + "StartingCountry: " + s2 + "NavalCountry: " + s3 + s4;
	}

	Country::~Country()
	{
		delete country_ID;
		delete mIsStartCountry;
		delete mIsNavalCountry;
		continent.clear();
	}

	class LinkedList::Node
	{
		Country* country;
		Node* next;
		Node* previous;
		bool* mRequiresNaval;

	public:
		Node(Country* country, bool naval)
		{
			this->mRequiresNaval = new bool(naval);
			if (country == NULL)
				this->country = NULL;
			else
				this->country = country;
			next = NULL;
			previous = NULL;
		}

		Node(Country* country, Node* next, Node* previous, bool naval)
		{
			this->mRequiresNaval = new bool(naval);
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

		bool requiresNavalPassage()
		{
			return *mRequiresNaval;
		}

		void displayNode()
		{
			cout << "COUNTRY:\n" + country->displayCountry()
				<< "Requires naval passge: " << ((*mRequiresNaval) ? "Yes\n\n" : "No\n\n");
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
			size = new int();
			*size = 0;
		}
		else
		{
			head = new Node(country, false);
			tail = head;
			size = new int();
			*size = *size + 1;
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
		return *size;
	}

	bool LinkedList::addToHead(Country* country, bool naval)
	{
		bool returnMe = false;
		if (country != NULL)
		{
			if (head == NULL)
			{
				head = new Node(country, naval);
				tail = head;
			}
			else
			{
				Node* n1 = new Node(country, head, NULL, naval);
				head->setPrev(n1);
				head = n1;
			}
			*size = *size + 1;
			returnMe = true;
		}
		else
		{
			cout << "You tried to add nothing";
		}
		return returnMe;
	}

	bool LinkedList::addToTail(Country* country, bool naval)
	{
		bool returnMe = false;
		if (country != NULL)
		{
			if (tail == NULL)
			{
				tail = new Node(country, naval);
				head = tail;
			}
			else
			{
				Node* n1 = new Node(country, NULL, tail, naval);
				tail->setNext(n1);
				tail = n1;
			}
			*size = *size + 1;
			returnMe = true;
		}
		else
		{
			cout << "You tried to add nothing";
		}
		return returnMe;
	}

	bool LinkedList::add(int where, Country* country, bool naval)
	{
		bool returnMe = false;
		if (country != NULL)
		{
			if (where == 0)
				returnMe = addToHead(country, naval);
			else if (where >= *size)
				returnMe = addToTail(country, naval);
			else
			{
				Node* current = head;
				for (int i = 0; i < where; i++)
				{
					current = current->getNext();
				}
				Node* prev = current->getPrev();
				Node* n1 = new Node(country, current, prev, naval);
				prev->setNext(n1);
				current->setPrev(n1);
				*size = *size + 1;
				returnMe = true;
			}
		}
		else
		{
			cout << "You tried to add nothing";
		}
		return returnMe;
	}

	Country* LinkedList::get(int where)
	{
		if (where >= *size)
			return tail->getCountry();
		else if (where <= 0)
			return head->getCountry();
		else
		{
			Node* getMe = head;
			for (int i = 0; i < where; i++)
			{
				getMe = getMe->getNext();
			}
			return getMe->getCountry();
		}
	}

	Country* LinkedList::removeHead()
	{
		if (*size == 0)
			return NULL;
		else
		{
			Node* returnMe = head;
			head = head->getNext();
			head->setPrev(NULL);
			*size = *size - 1;
			return returnMe->getCountry();
		}
	}

	Country* LinkedList::removeTail()
	{
		if (*size == 0)
			return NULL;
		else
		{
			Node* returnMe = tail;
			tail = tail->getPrev();
			tail->setNext(NULL);
			*size = *size - 1;
			return returnMe->getCountry();
		}
	}

	Country* LinkedList::remove(int where)
	{
		if (where == 0)
			return removeHead();
		else if (where >= *size)
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
			*size = *size - 1;
			return current->getCountry();
		}
	}

	void LinkedList::displayLinkedList()
	{
		if (*size == 0)
			cout << "Nothing in the Linked List\n";
		else
		{
			cout << "size: " + to_string(*size) + "\n";
			Node* toDisplay = this->head;
			for (int i = 0; i < *size; i++)
			{

				toDisplay->displayNode();
				toDisplay = this->getNext(toDisplay);
			}
		}
	}

	bool LinkedList::isAdjacent(Country* country)
	{
		if (*size == 0)
		{
			cout << "Nothing in the Linked List\n";
			return false;
		}
					
		else
		{
			Node* node = this->head;
			for (int i = 0; i < *size; i++)
			{
				if (node->getCountry()->getID() == country->getID())
					return true;

				node = this->getNext(node);
			}
			return false;
		}
	}

	LinkedList::~LinkedList()
	{
		Node* current = this->head;
		Node* next;
		for (int i = 0; i < *size; i++)
		{
			next = current->getNext();
			delete current;
			current = next;
		}
		*size = 0;
	}


	// Graphical Tile Map

	TileMap::TileMap()
	{
		src.h = dest.h = GRID_CELL_SIZE;
		src.w = dest.w = GRID_CELL_SIZE;

		src.x = 0;
		src.y = 0;

		dest.x = 0;
		dest.y = 0;

	}

	TileMap::~TileMap()
	{
	}


	void TileMap::drawTileMap(SDL_Renderer* renderer, SDL_Texture* texture)
	{
		int type;

		for (int row = 0; row < MAP_HEIGHT; row++)
		{
			for (int col = 0; col < MAP_WIDTH; col++)
			{
				type = tiles[row][col];
				std::string s = std::to_string(type);
				int digit = s.at(0) - '0';

				if (type < 10)
				{
					src.y = 0;
					src.x = digit * GRID_CELL_SIZE;
				}					
				else
				{
					src.y = digit * GRID_CELL_SIZE;
					src.x = stoi(&s[1]) * GRID_CELL_SIZE;
				}

				dest.x = col * GRID_CELL_SIZE;
				dest.y = row * GRID_CELL_SIZE;
				//std::cout << "xSrc: " << src.x << " ySrc:" << src.y << endl;
				//std::cout << "xDest: " << dest.x << " yDest:" << dest.y << endl;
				TextureLoader::draw(renderer, texture, &src, &dest);
			}
		}
	}

}