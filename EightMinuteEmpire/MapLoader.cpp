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
std::vector<std::string> MapLoader::Parser::vContinents;
std::string MapLoader::selectedMap;

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
	//MapLoader::selectMap();
	mapLoader->mapPath =  getMapsDir().append("\\").append(MapLoader::selectedMap);
	cout << mapLoader->mapPath << endl;
	ifstream inputMapFile;
	inputMapFile.open(mapLoader->mapPath);

	if (!MapLoader::Parser::isFileStructureValid(inputMapFile))
	{
		inputMapFile.close();
		delete mapLoader;
		return nullptr;
	}
		cout << "Genuine Map File" << endl;
		if (!mapLoader->initMap(inputMapFile))
			return nullptr;

	inputMapFile.close();
	return mapLoader;
}

 bool MapLoader::load(GraphWorld::Map* map)
 {
	 using namespace std;
	 // Reset static variables
	 if (MapLoader::isStartingCountrySet)
		 MapLoader::isStartingCountrySet = false;
	 MapLoader::Parser::vContinents.clear();

	 ifstream inputMapFile;
	 inputMapFile.open(mapPath);
	 
	 if (!MapLoader::Parser::processCountries(inputMapFile, map))
	 {
		 inputMapFile.close();
		 return false;
	}
	 cout << "Map Successfully loaded." << endl;
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

void MapLoader::selectMap()
 {
	 using namespace std;

	 vector<string> maps = getInstalledMaps();

	 for (vector<string>::size_type i = 0; i != maps.size(); i++)	 
		 cout << (i + 1) << "." << maps.at(i) << endl;

	 int n;
	 cout << "Chose the map you want to load: ";
	 cin >> n;
	 MapLoader::selectedMap = maps.at((double)n - 1);
	 cout << "You have chosen the map " << MapLoader::selectedMap << endl;
 }


 bool MapLoader::Parser::isFileStructureValid( std::ifstream& inputMapFile)
 {
	 using namespace std;

	 string firstLine;
	 getline(inputMapFile, firstLine);

	 if (firstLine.compare("<eme_map_file>") != 0)
	 {
		 cout << "\nINVALID MAP FILE.\n" << endl;
		 return false;
	 }

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
		 {
			 cout << "INVALID MAP FILE. Missing closing tags. " << endl;
			 return false;
		 }
		 
	 }

	 seekToStart(inputMapFile);
	 return true;
 }


 bool MapLoader::initMap(std::ifstream& inputMapFile)
 {
	 using namespace std;
	 string line;
	 std::smatch mapName;
	 std::smatch numContinents;
	 std::smatch numCountries;

	 const std::regex mapNameIdentifier{ "(Map_Name:)\\w+" };
	 const std::regex continentsIdentifier{ "(Num_Continents:)\\d+" };
	 const std::regex countriesIdentifier{ "(Num_Countries:)\\d+" };

	 while (getline(inputMapFile, line))
	 {
		 if (line.compare("<eme_attributes>") == 0)
		 {
			 getline(inputMapFile, line);			
			 if (std::regex_search(line, mapName, mapNameIdentifier))
				 this->mapName = mapName[0].str().erase(0, 9);
			 else
			 {
				 cout << "Error loading Map Attributes. Could not find 'Map_Name'.\n";
				 return false;
			 }
			 getline(inputMapFile, line);
			 if (std::regex_search(line, numContinents, continentsIdentifier))
				 *this->numContinents = stoi(numContinents[0].str().erase(0, 15));
			 else
			 {
				 cout << "Error loading Map Attributes. Could not find 'Num_Continents'.\n";
				 return false;
			 }
			getline(inputMapFile, line);
			if (std::regex_search(line, numCountries, countriesIdentifier))
				*this->numCountries = stoi(numCountries[0].str().erase(0, 14));
			else
			{
				cout << "Error loading Map Attributes. Could not find 'Num_Countries'.\n";
				return false;
			}

			return true;
		 }
	 }
	 cout << "Error loading Map Attributes. Could not find '<eme_attributes>' tag.\n";
	return false;
 }


bool MapLoader::Parser::processCountries( std::ifstream& inputMapFile, GraphWorld::Map* map )
 {
	 using namespace std;
	 string line;
	 int n = 0;
	 string p1;

	 const std::regex countryLineIdentifier{"\\[\\d+!?\\+?;\\D\\w*\\]"};
	 const std::regex adjacencyIdentifier{"\\{(\\s*\\d+\\s*\\+?\\s*\\,?)+\\}"};

	 std::smatch countryAttributesMatch;
	 std::smatch adjacentCountiresMatch; 

	 std::vector<std::vector<std::string>> adjacentCountries; //Holds vectors of adjacent countries of each country
	 vector<std::string> adjs; //holds adjacent countries

	 while ( getline(inputMapFile, line) )
	 {
		 n++;
		 if (line.compare("<eme_countries>") == 0)
		 {
			 //Start processing country lines
			 for (int i = 0; i < map->getNumCountries(); i++)
			 {
				 n++;
				 getline(inputMapFile, line);
				 if (line == "")
					 getline(inputMapFile, line);

				 // Process country attributes
				 if (std::regex_search(line, countryAttributesMatch, countryLineIdentifier))
				 {			
					 if (!initCountry(countryAttributesMatch[0].str(), map, i))
						 return false;

				 //Process adjacent countries
					 if (std::regex_search(line, adjacentCountiresMatch, adjacencyIdentifier))
					 {

						 adjs = processAdjacency(adjacentCountiresMatch[0].str(), map->getNumCountries());
						 if (adjs.empty())
							 return false;
						 adjacentCountries.push_back(adjs);
					 }
					 else
					 {
						 std::cout << "Error reading adjacent country list on line [" << (n) << "].\n";
						 return false;
					 }					 
				 }
				 else 
				 {
					 std::cout << "Error reading country list. Check line [" << (n) << "].\n";
					 return false;
				 }					 
			 }

		 if (!validateAdjacentCountries(adjacentCountries, map))
				 return false;
		initAdjacencyLists(adjacentCountries, map);
		return true;
		 }
	 }

	 cout << "Error loading Map Attributes. Could not find '<eme_countries>' tag.\n";
	 return false;
 }

 bool MapLoader::Parser::initCountry(std::string countryAttributes, GraphWorld::Map* map, const int countryIndex)
 {
	 using namespace std;

	 string continent;
	 std::smatch countryMatch;
	 std::smatch continentMatch;
	 bool startCountry = false;
	 bool navalCountry = false;
	 const std::regex countryID{ "\\d+" };
	 const std::regex continentID{ ";\\D\\w*" };

	 // Getting country ID
	 std::regex_search(countryAttributes, countryMatch, countryID);

	 if (stoi(countryMatch[0].str()) != countryIndex)
	 {
		 cout << "Error initializing country object. Country '" << countryIndex << "' not found."<< endl;
		 return false;
	 }

	 // Getting continent of the country 
	 std::regex_search(countryAttributes, continentMatch, continentID);
	 continent = continentMatch[0].str().substr(1);
	 if (!validateContinent(continent, map->getNumContinents()))
		 return false;

	 //Getting naval status
	 if (countryAttributes.find('+') != string::npos)
		 navalCountry = true;

	// Setting the starting country if it has not already been set 
	 if (countryAttributes.find('!') != string::npos && !MapLoader::isStartingCountrySet)
	 {
		 MapLoader::isStartingCountrySet = true;
		 startCountry = true;
	 }
	 else if (countryAttributes.find('!') != string::npos && MapLoader::isStartingCountrySet)
	 {
		 cout << "Invalid Map File. More than one starting country specified." << endl;
		 return false;
	 }

	 GraphWorld::Country* country= new GraphWorld::Country(GraphWorld::Country(stoi(countryMatch[0].str()), startCountry, navalCountry, &continentMatch[0].str().substr(1)));
	 map->addNode(country);
	 return true;
 }

 bool MapLoader::Parser::validateContinent(std::string continent, const int numContinents)
 {
	vContinents.push_back(continent);
	std::sort(vContinents.begin(), vContinents.end());
	 for (int i = 0; i < vContinents.size() - 1; i++)
	 {
		 if (vContinents[i] == vContinents[i + 1])
		 {
			 vContinents.erase(vContinents.begin() + i);
			 i--;
		 }
	 }

	 if (vContinents.size() > numContinents)
	 {
		 std::cout << "Error. Found more than " << numContinents << " number of continents: \n -";
		 for (auto i : vContinents)
			 std::cout << i << "-";
		 std::cout << std::endl;
		return false;
	 }


	 return true;
 }

 std::vector<string> MapLoader::Parser::processAdjacency(std::string adjacentCountiresMatch, const int numCountries)
 {
	 using namespace std;
	 string t;
	 int country;
	 std::vector<std::string> adjCountries;
	 std::vector<int> ids; // used to check for duplicates

	 //Remove brackets
	 if (adjacentCountiresMatch.size() > 2)
	 {
		 adjacentCountiresMatch.pop_back();
		 adjacentCountiresMatch.erase(adjacentCountiresMatch.begin());
	 }

	 std::stringstream ss(adjacentCountiresMatch);
	 while (getline(ss, t, ','))
	 {
		 country = removeNavalSymbol(t);
	
		 if (country > numCountries)
		 {
			 cout << "Error! Country " << country << " is not a valid ID.\n";
			 adjCountries.clear();
			 return {};
		 }
		 if (std::find(ids.begin(), ids.end(), country) != ids.end())
		 {
				
			 cout << "Error! Duplicate country '" << country << "' found.\n";
			 return {};
		 }
		 else 
		 {
			 adjCountries.push_back(t);
			 ids.push_back(country);
		 }

	 }
	 return adjCountries;
 }

 bool MapLoader::Parser::validateAdjacentCountries(const std::vector<std::vector<std::string>>& adjacentCountries, GraphWorld::Map* map)
 {
	 int i = 0;
	 bool check = false;
	 int t;
	 int t2;
	 bool isNaval;
	 std::string message = "Invalid adjacency list for country ";
	 for (auto vec : adjacentCountries) 
	 {		 
		 isNaval = map->getCountry(i)->isNavalCountry();

		 for (auto c1 : vec) 
		 {		
			 t = removeNavalSymbol(c1);
			 if (!isNaval && c1.find('+') != std::string::npos)
			 {
				 std::cout << message << i << "\n Country " << t << " cannot be a naval passage to a non naval country.\n";
				 return false;
			 }			
			 for (auto c2 : adjacentCountries[t])
			 {
				 t2 = removeNavalSymbol(c2);

				if((c1.find('+') != std::string::npos && c2.find('+') != std::string::npos && t2 == i) || (t2 == i && c1.find('+') == std::string::npos && c2.find('+') == std::string::npos))
				 {
					 check = true;
					 break;
				 }
				else if (t2 == i)
				{
					std::cout << message << t << ".\nNaval passage mismatch detected with country " << t2 << ".\n";
					return false;
				}

			 } 
			 if (!check) 
			 {
				 std::cout << "Invalid adjacency list for country " << (t);
				 std::cout << "It is missing country " << i << " in it's adjacency list.\n";
				 return false;
			 }

		 check = false;
		 }
		 i++;
	 }
	 return true;
 }

 void MapLoader::Parser::initAdjacencyLists(const std::vector<std::vector<std::string>>& adjacentCountries, GraphWorld::Map* map)
 {
	 using namespace std;
	 int temp; // Country ID
	 bool naval = false;

	 for (int i = 0; i < adjacentCountries.size(); i++)
	 {
		 for (int j = 0; j < adjacentCountries[i].size(); j++)
		 { 
			 //remove '+' symbol from string
			 temp = removeNavalSymbol(adjacentCountries[i][j]);
			 //Check if the the adjacent country is naval (has a plus symbol next to it)
			 //And that the node country is also a naval country
			 if (adjacentCountries[i][j].find('+') != std::string::npos && map->getCountry(i)->isNavalCountry() && map->getCountry(temp)->isNavalCountry())
				 naval = true;
			map->addEdge(map->getCountry(i), map->getCountry( temp), naval);
			naval = false;
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

int MapLoader::Parser::removeNavalSymbol(std::string country)
 {
	 if (country.find('+') != std::string::npos)
		 country.pop_back();
	 return stoi(country);
 }