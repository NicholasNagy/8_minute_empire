#include "MapLoader.h"
#include <fstream>
#include <filesystem>
#include <Windows.h>
#include <iostream>
#include <string>
#include<vector>
#include <regex>
#include <sstream> 

bool MapLoader::isStartingCountrySet = false;

MapLoader::MapLoader()
{
	this->numCountries = new int(0);
	this->numContinents = new int(0);
}


MapLoader::MapLoader(string* mapPath, string* mapName, int numCountries, int numContinents)
{
	this->mapPath = string(*mapPath);
	this->mapName = string(*mapName);
	this->numCountries = new int(numCountries);
	this->numContinents = new int(numContinents);

}

MapLoader::MapLoader(const MapLoader& loader2)
{
	this->mapPath = loader2.mapPath;
	this->mapName = loader2.mapName;
	this->numCountries = loader2.numCountries;
	this->numContinents = loader2.numContinents;

}

string MapLoader::getMapName()
{
	return mapName;
}

int MapLoader::getNumCountries()
{
	return *numCountries;
}

int MapLoader::getNumContinents()
{
	return *numContinents;
}

void MapLoader::setMapPath(string* path)
{
	this->mapPath.clear();
	this->mapPath = *path;
}



MapLoader::~MapLoader()
{
	mapPath.clear();
	mapName.clear();
	delete numCountries;
	delete numContinents;
}

MapLoader* MapLoader::initiateMapPicker()
{
	using namespace std;

	MapLoader* mapLoader = new MapLoader();
	mapLoader->mapPath =  getMapsDir().append("\\").append(selectMap());
	ifstream inputMapFile;
	inputMapFile.open(mapLoader->mapPath);

	if (!MapLoader::Parser::isFileStructureValid(inputMapFile))
	{
		cout << "INVALID MAP FILE" << endl;
		inputMapFile.close();
		delete mapLoader;
		return nullptr;
	}
		cout << "Genuine Map File" << endl;
		mapLoader->initMap(inputMapFile);

	inputMapFile.close();
	if (MapLoader::isStartingCountrySet)
		MapLoader::isStartingCountrySet = false;

	return mapLoader;
}

 bool MapLoader::load(GraphWorld::Map* map)
 {
	 using namespace std;

	 ifstream inputMapFile;
	 inputMapFile.open(mapPath);
	 
	 if (!MapLoader::Parser::processCountries(inputMapFile, map, this->getNumCountries() ))
	 {
		 inputMapFile.close();
		 return false;
	}

	 return true;
 }

 std::string MapLoader::getMapsDir()
 {
	 std::string mapDir = "\\Maps";
	 char dir[MAX_PATH];
	 GetCurrentDirectory( MAX_PATH, dir );
	 return dir + mapDir;
	 
 }

 std::vector<std::string> MapLoader::getInstalledMaps()
 {
	 std::vector<std::string> maps;

	 
	 for (const auto& map : std::filesystem::directory_iterator( getMapsDir() ) )
	 { 
		maps.push_back(map.path().filename().string());
		// log: std::cout << map.path().filename().string() << std::endl;
	 }
	 
	 return maps;
 }

 std::string MapLoader::selectMap()
 {
	 using namespace std;

	 vector<string> maps = getInstalledMaps();

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


 bool MapLoader::Parser::isFileStructureValid( std::ifstream& inputMapFile)
 {
	 using namespace std;

	 string firstLine;
	 getline(inputMapFile, firstLine);

	 if (firstLine.compare("<eme_map_file>") != 0)
		 return false;

	 else {

		 inputMapFile.seekg(-1, ios_base::end);
		 bool loop = true;
		 while (loop) {
			 char ch;
			 inputMapFile.get(ch);

			 if ((int)inputMapFile.tellg() <= 1) {
				 inputMapFile.seekg(0);
				 loop = false;
			 }
			 else if (ch == '\n') {
				 loop = false;
			 }
			 else {
				 inputMapFile.seekg(-2, ios_base::cur);
			 }
		 }

		 string lastLine;
		 string x = "</eme_map_file>";
		 getline(inputMapFile, lastLine);

		 if (lastLine.compare(x) != 0)
			 return false;
		 
	 }

	 seekToStart(inputMapFile);
	 return true;
 }


 void MapLoader::initMap(std::ifstream& inputMapFile)
 {
	 using namespace std;
	 string line;
	 string mapName;
	 string numContinents;
	 string numCountries;

	 while (getline(inputMapFile, line))
	 {
		 if (line.compare("<eme_attributes>") == 0)
		 {
			 getline(inputMapFile, mapName);
			 //if line.contains "Map"
			 mapName = mapName.erase(0, 4);
			 //cout << "Map Name: " << mapName << endl;
			 //if line.contains "Continents"
			 getline(inputMapFile, numContinents);
			 numContinents = numContinents.erase(0, 11);
			//cout << "# Continents: " << stoi(numContinents) << endl;
			  //if line.contains "Countries"
			 getline(inputMapFile, numCountries);
			 numCountries = numCountries.erase(0, 10);
			//cout << "# Countries: " << numCountries << endl;

			 break;
		 }
	 }
	this->mapName = mapName;
	*this->numContinents = stoi(numContinents);
	*this->numCountries = stoi(numCountries);
 }

bool MapLoader::Parser::processCountries( std::ifstream& inputMapFile, GraphWorld::Map* map, const int size )
 {
	 using namespace std;
	 string line;
	 string p1;

	 const std::regex countryLineIdentifier{"\\[\\d+!?\\+?;\\D\\w*\\]"};
	 const std::regex adjacencyIdentifier{"\\{(\\s*\\d+\\s*\\,?)+\\}"};

	 std::smatch countryAttributesMatch;
	 std::smatch adjacentCountiresMatch; 

	 std::vector<std::vector<string>> adjacentCountries; //Holds adjacent countries of each country

	 while ( getline(inputMapFile, line) )
	 {
		 if (line.compare("<eme_countries>") == 0)
		 {
			 //Start processing country lines
			 while ( getline(inputMapFile, line) )
			 {
				 // Process country attributes
				 if (std::regex_search(line, countryAttributesMatch, countryLineIdentifier))
				 {			
					 if (!initCountry(countryAttributesMatch, map, size))
						 return false;
				 }
				 //Process adjacent countries
				 if (std::regex_search(line, adjacentCountiresMatch, adjacencyIdentifier))
				 {
					adjacentCountries.push_back(processAdjacency(adjacentCountiresMatch));
				 }
				 else if (line.compare("</eme_countries>") == 0)
					 break;
				 else 
				 {
					 std::cout << "Error reading country list." << endl;
					 return false;
				 }					 
			 }
		break;
		 }
	 }

	 initAdjacencyLists(adjacentCountries, map);
	 return true;
 }

 bool MapLoader::Parser::initCountry(std::smatch &countryAttributes, GraphWorld::Map* map, const int size)
 {
	 using namespace std;

	 std::smatch countryMatch;
	 std::smatch continentMatch;
	 bool startCountry = false;
	 bool navalCountry = false;
	 const std::regex countryID{ "\\d+" };
	 const std::regex continentID{ ";\\D\\w*" };
	 string s = countryAttributes[0].str();

	 // Getting country ID
	 std::regex_search(s, countryMatch, countryID);

	 if (stoi(countryMatch[0].str()) >= size)
	 {
		 cout << "Error. Map ID higher than map size." << endl;
		 return false;
	 }

	 // Getting continent of the country 
	 std::regex_search(s, continentMatch, continentID);

	 //Getting naval status
	 if (s.find('+') != string::npos)
		 navalCountry = true;

	// Setting the starting country if it has not already been set 
	 if (s.find('!') != string::npos && !MapLoader::isStartingCountrySet)
	 {
		 MapLoader::isStartingCountrySet = true;
		 startCountry = true;
	 }
	 else if (s.find('!') != string::npos && MapLoader::isStartingCountrySet)
	 {
		 cout << "Invalid Map File. More than one starting country specified." << endl;
		 return false;
	 }

	 GraphWorld::Country* country= new GraphWorld::Country(GraphWorld::Country(stoi(countryMatch[0].str()), startCountry, navalCountry, &continentMatch[0].str().substr(1)));
	 map->addNode(country);
	 return true;
 }

 std::vector<std::string> MapLoader::Parser::processAdjacency(std::smatch& adjacentCountiresMatch)
 {
	 using namespace std;
	 string s = adjacentCountiresMatch.str();
	 string t;
	 std::vector<std::string> adjCountries;

	 //Remove brackets
	 if (s.size() > 2)
	 {
		 s.pop_back();
		 s.erase(s.begin());
	 }

	 std::stringstream ss(s);
	 while (getline(ss, t, ','))
		 adjCountries.push_back(t);

	 return adjCountries;
 }


 void MapLoader::Parser::initAdjacencyLists(std::vector<std::vector<string>> adjacentCountries, GraphWorld::Map* map)
 {
	 using namespace std;
	 string temp; // Country ID

	 for (int i = 0; i < adjacentCountries.size(); i++)
	 {
		 for (int j = 0; j < adjacentCountries[i].size(); j++)
		 {
			 temp = adjacentCountries[i][j];
			map->addEdge(map->getCountry(i), map->getCountry( stoi(temp)));		 
		 }
	 }
 }


 void MapLoader::Parser::seekToStart(std::ifstream& inputMapFile)
 {
	 inputMapFile.clear();
	 inputMapFile.seekg(0, std::ios::beg);
 }

 std::ostream& operator<<(std::ostream& s, const MapLoader& mapLoader)
 {
	 return  s << 
		 "Loading Map: " << mapLoader.mapPath << std::endl <<
		 "Map_Name: "<< mapLoader.mapName << std::endl << 
		 "Num_Continents: " << *mapLoader.numContinents << std::endl <<
		 "Num_Countries: "<< *mapLoader.numCountries << std::endl;
 }
