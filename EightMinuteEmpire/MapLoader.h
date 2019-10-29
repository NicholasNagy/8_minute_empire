#pragma once
#include <iostream>
#include<vector>
#include <fstream>
#include <regex>
#include "Map.h"


class MapLoader
{
public:
	MapLoader();
	~MapLoader();
	MapLoader(string* mapPath, string* mapName, int numCountries, int numContinents); 
	MapLoader(const MapLoader& loader2);
	string getMapName();
	int getNumCountries();
	int getNumContinents();
	void setMapPath(string* path);
	static MapLoader* initiateMapPicker();
	bool load( GraphWorld::Map* map);
	bool initMap(std::ifstream& inputMapFile);
	static std::string getMapsDir();
	static std::vector<std::string> getInstalledMaps();
	static void selectMap();
	static std::string selectedMap;
	friend std::ostream& operator<<(std::ostream&, const MapLoader&);

private:
	string mapPath;
	string mapName;
	int* numCountries;
	int* numContinents;
	static bool isStartingCountrySet;

	class Parser 
	{
		friend class MapLoader;

	private:
		static std::vector<string> vContinents; //Used to validate continents
		static bool isFileStructureValid(std::ifstream& inputMapFile);
		static  bool processCountries( std::ifstream& inputMapFile, GraphWorld::Map* map);
		static  bool initCountry(std::string countryAttributes, GraphWorld::Map* map, const int countryIndex);
		static std::vector<std::string> processAdjacency(std::string adjacentCountires, const int numContries);
		static void initAdjacencyLists(const std::vector<std::vector<std::string>>& adjacentCountries, GraphWorld::Map* map);
		static bool validateAdjacentCountries(const std::vector<std::vector<std::string>>& adjacentCountries, GraphWorld::Map* map);
		static bool validateContinent(std::string continent, const int numContinents);
		static void seekToStart(std::ifstream& inputMapFile);	
		static int removeNavalSymbol(std::string);
	};
};