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
	void initMap(std::ifstream& inputMapFile);
	static std::string getMapsDir();
	static std::vector<std::string> getInstalledMaps();
	static std::string selectMap();
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
		static bool isFileStructureValid(std::ifstream& inputMapFile);
		static  bool processCountries( std::ifstream& inputMapFile, GraphWorld::Map* map, const int size);
		static  bool initCountry(std::smatch& countryAttributes, GraphWorld::Map* map, const int size);
		static std::vector<std::string> processAdjacency(std::smatch& adjacentCountires);
		static void initAdjacencyLists(std::vector<std::vector<string>> adjacentCountries, GraphWorld::Map* map);
		static void seekToStart(std::ifstream& inputMapFile);	
	};
};