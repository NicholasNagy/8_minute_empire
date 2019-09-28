#include "MapLoader.h"
#include <fstream>
#include <filesystem>
#include <Windows.h>
#include <iostream>
#include <string>
#include<vector>

MapLoader::MapLoader()
{
}


MapLoader::~MapLoader()
{
}

 std::string MapLoader::getMapsDir()
 {
	 std::string mapDir = "\\Maps";
	 char dir[MAX_PATH];
	 GetCurrentDirectory( MAX_PATH, dir );
	 return dir + mapDir;
	 
 }

 std::vector<std::string> MapLoader::listInstalledMaps()
 {
	 std::vector<std::string> maps;

	 for (const auto& map : std::filesystem::directory_iterator( getMapsDir() ) )
	 { 
		maps.push_back( map.path().stem().string() );
		// log: std::cout << map.path().filename().string() << std::endl;
	 }

	 return maps;
 }

 std::string MapLoader::selectMap()
 {
	 using namespace std;

	 vector<string> maps = listInstalledMaps();

	 for (vector<string>::size_type i = 0; i != maps.size(); i++)	 
		 cout << (i + 1) << "." << maps.at(i) << endl;

	 int n;
	 std::string selectedMap;
	 cout << "Chose the map you want to load: ";
	 cin >> n;
	 selectedMap = maps.at((double)n - 1);
	 cout << "You have chosen the map " << selectedMap << endl;


	 return selectedMap;
 }

 void MapLoader::load()
 {
	 using namespace std;

	 string selectedMapPath = getMapsDir().append("\\").append(selectMap()).append(".map");
	 string line;

	 ifstream inputMapFile( selectedMapPath );

	 if ( inputMapFile.is_open() )
	 {
		 while ( getline( inputMapFile, line ) )
		 {
			 cout << line << '\n';
		 }
		 inputMapFile.close();
	 }

	 else cout << "Error loading map file.";
	 
 }
