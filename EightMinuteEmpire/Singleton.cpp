#pragma once

#include "Singleton.h"
#include "Map.h"

SingletonClass* SingletonClass::_instance;

SingletonClass::SingletonClass()
{
	map = nullptr;
}

SingletonClass::SingletonClass(string* mapName, int numCountries, int numContinents)
{
	map = new Map(mapName, numCountries, numContinents);
}

SingletonClass* SingletonClass::instance()
{
	if (!_instance)
		_instance = new SingletonClass();
	return _instance;
}
SingletonClass* SingletonClass::instance(string* mapName, int numCountries, int numContinents)
{
	if (!_instance)
		_instance = new SingletonClass(mapName, numCountries, numContinents);
	return _instance;
}

void SingletonClass::addNode(Country* country)
{
	map->addNode(country);
}

void SingletonClass::addEdge(Country* currentCountry, Country* adjacentCountry, bool requiresNaval)
{
	map->addEdge(currentCountry, adjacentCountry, requiresNaval);
}

void SingletonClass::setStartingCountry(Country* country)
{
	map->setStartingCountry(country);
}

void SingletonClass::printMap()
{
	map->printMap();
}

void SingletonClass::setTileMap(TileMap* tileMap)
{
	map->setTileMap(tileMap);
}

int SingletonClass::getNumCountries()
{
	return map->getNumCountries();
}

int SingletonClass::getNumContinents()
{
	return map->getNumContinents();
}

int SingletonClass::getSize()
{
	return map->getSize();
}

string SingletonClass::getMapName()
{
	return map->getMapName();
}

Country* SingletonClass::getCountry(int id)
{
	return map->getCountry(id);
}

Country* SingletonClass::getStartingCountry()
{
	return map->getStartingCountry();
}

LinkedList* SingletonClass::getAdjacentList(Country* country)
{
	return map->getAdjacentList(country);
}

TileMap* SingletonClass::getTileMap()
{
	return map->getTileMap();
}