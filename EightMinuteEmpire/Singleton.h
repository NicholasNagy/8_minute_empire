#pragma once
#include "Map.h"

using namespace GraphWorld;

class SingletonClass
{
	public:
		static SingletonClass* instance();
		static SingletonClass* instance(string* mapName, int numCountries, int numContinents);
		void addNode(Country* country);
		void addEdge(Country* currentCountry, Country* adjacentCountry, bool requiresNaval);
		void setStartingCountry(Country* country);
		void printMap();
		void setTileMap(TileMap* tileMap);
		int getNumCountries();
		int getNumContinents();
		int getSize();
		string getMapName();
		Country* getCountry(int id);
		Country* getStartingCountry();
		LinkedList* getAdjacentList(Country* country);
		TileMap* getTileMap();
	private:
		Map* map;
		static SingletonClass* _instance;
		SingletonClass();
		SingletonClass(string* mapName, int numCountries, int numContinents);
};