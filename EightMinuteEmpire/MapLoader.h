#pragma once
#include <iostream>
#include<vector>
#include <fstream>


class MapLoader
{
public:
	MapLoader();
	~MapLoader();
	static void load();
	static std::string getMapsDir();
	static std::vector<std::string> getInstalledMaps();
	static std::string selectMap();

	class Parser 
	{
		friend class MapLoader;

	private:
		static bool isFileStructureValid(std::ifstream& inputMapFile);
		static void processAttributes(std::ifstream& inputMapFile);
		static void processCountries( std::ifstream& inputMapFile);
		static void seekToStart(std::ifstream& inputMapFile);
	};
};

