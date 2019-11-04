#ifndef MAP_H
#define MAP_H
#include "SDL.h"
#include <string>
#include <set>

class Player;

using std::string;

namespace GraphWorld
{
	class Map;
	class LinkedList;
	class Country;

	class TileMap
	{
	public:
		TileMap();
		~TileMap();
		void drawTileMap(SDL_Renderer* renderer, SDL_Texture* texture);
		int tiles[20][32];

	private:
		SDL_Rect src, dest;

	};


	class Map
	{
		class Node;
		string mapName;
		int* size;
		int* numCountries;
		int* numContinents;
		typedef Node* NodePtr;
		NodePtr* arrayOfCountries;
		TileMap* tileMap;
		Country* startingCountry;

		public:
			Map(string* mapName, int numCountries, int numContinents);
			void addNode(Country* country);
			void addEdge(Country* currentCountry, Country* adjacentCountry, bool requiresNaval);
			Country* getCountry(int id);
			string getMapName();
			int getNumCountries();
			int getNumContinents();
			void setStartingCountry(Country*);
			Country* getStartingCountry();
			LinkedList* getAdjacentList(Country* country);
			void printMap();
			int getSize();
			TileMap* getTileMap();
			void setTileMap(TileMap* tileMap);
			~Map();
	};

	struct Cmp
	{
		bool operator ()(const std::pair<int, Player*>& a, const std::pair<int, Player*>& b) const
		{
			return a.first > b.first;
		}
	};

	class Country
	{
		int* country_ID;
		bool* mIsStartCountry;
		bool* mIsNavalCountry;
		string continent;
		std::multiset<std::pair<int, Player*>, Cmp> mOccupyingPlayers;  //Players occupying this country, sorted by most holdings(armies/cities) which are the keys

		Player* owner;   

		public:
			Country();
			Country(const Country&);
			Country(int country_ID, bool startCountry, bool navalCountry, string* continent);
			int getID();
			bool isStartCountry();
			bool isNavalCountry();
			string getContinent();
			std::multiset<std::pair<int, Player*>, Cmp>& occupyingPlayers();
			void updateOccupyingPlayerScore(int Score, Player*);
			void setID(int id);
			void setStartCountry(bool maybe);
			void setContinent(string* continent);
			string displayCountry();
			void setCountryOwner(Player*);
			Player* getCountryOwner();
			~Country();
			friend std::ostream& operator<<(std::ostream&, const Country&);
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
		bool addToHead(Country* country, bool requiresNaval);
		bool addToTail(Country* country, bool requiresNaval);

		public:
			LinkedList(Country* country);
			int sizeOf();
			bool add(int where, Country* country, bool requiresNaval);
			Country* get(int where);
			bool isAdjacent(Country* country);
			Country* remove(int where);
			void displayLinkedList();
			~LinkedList();
	};



}


#endif

