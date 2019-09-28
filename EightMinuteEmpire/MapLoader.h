#pragma once
#include <iostream>
#include<vector>


class MapLoader
{
public:
	MapLoader();
	~MapLoader();
	static void load();
	static std::string getMapsDir();
	static std::vector<std::string> listInstalledMaps();
	static std::string selectMap();
};

