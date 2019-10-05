#pragma once
#include <iostream>
#include<vector>
#include <fstream>
#include <regex>


class MapLoader
{
public:
	MapLoader();
	~MapLoader();
	static void load();
	static std::string getMapsDir();
	static std::vector<std::string> getInstalledMaps();
	static std::string selectMap();

private:
	static bool isStartingCountrySet;

	class Parser 
	{
		friend class MapLoader;

	private:
		static bool isFileStructureValid(std::ifstream& inputMapFile);
		static void processAttributes(std::ifstream& inputMapFile);
		static void processCountries( std::ifstream& inputMapFile);
		static void seekToStart(std::ifstream& inputMapFile);
		static void initializeCountry(std::smatch& countryAttributes);
		static void initializeAdjacencyList(std::smatch& adjacentCountires);
	};
};

