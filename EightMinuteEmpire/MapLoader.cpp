#include "MapLoader.h"
#include <fstream>
#include <filesystem>
#include <Windows.h>
#include <iostream>
#include <string>
#include<vector>
#include <regex>

bool MapLoader::isStartingCountrySet = false;

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

 void MapLoader::load()
 {
	 using namespace std;
	 string selectedMapPath = getMapsDir().append("\\").append(selectMap());

	ifstream inputMapFile;
	inputMapFile.open(selectedMapPath);

	if (MapLoader::Parser::isFileStructureValid( inputMapFile ))
	{
		cout << "Genuine Map File" << endl;
		MapLoader::Parser::processAttributes( inputMapFile );
		MapLoader::Parser::processCountries( inputMapFile );
	}
		
	else
		cout << "INVALID";

	inputMapFile.close();
	if ( MapLoader::isStartingCountrySet )
		MapLoader::isStartingCountrySet = false;
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

 void MapLoader::Parser::processAttributes( std::ifstream& inputMapFile )
 {
	 using namespace std;
	 string line;

	 while ( getline(inputMapFile, line) )
	 {
		 if (line.compare("<eme_attributes>") == 0)
		 {
			 // Get Map Attributes here.
			 for (int i = 0; i <= 2; i++)
			 {
				 getline(inputMapFile, line);
				 cout << line << endl;
			 }	

			 break;
		 }
			
	 }
 }

 void MapLoader::Parser::processCountries( std::ifstream& inputMapFile )
 {
	 using namespace std;
	 string line;
	 string p1;

	 const std::regex countryLineIdentifier{"\\[\\d+!?-\\D\\w*\\]"};
	 const std::regex adjacencyIdentifier{"\\{(\\s*\\d+\\~?\\s*\\,?)+\\}"};

	 std::smatch countryAttributes;
	 std::smatch adjacentCountires; 

	 while ( getline(inputMapFile, line) )
	 {
		 if (line.compare("<eme_countries>") == 0)
		 {
			 //Start processing country lines
			 while ( getline(inputMapFile, line) )
			 {
	
				 // Process country attributes
				 if (std::regex_search(line, countryAttributes, countryLineIdentifier))
				 {
					 cout << " Reading: " << countryAttributes[0].str() << endl;
					 initializeCountry(countryAttributes);
				 }
				 //Process adjacent countries
				 if (std::regex_search(line, adjacentCountires, adjacencyIdentifier))
				 {
					 initializeAdjacencyList(adjacentCountires);
				 }
				 else if (line.compare("</eme_countries>") == 0)
					 break;
				 else
					 cout << "Error reading country list." << endl;
			 }

		break;
		 }

	 }
	 

 }

 void MapLoader::Parser::seekToStart(std::ifstream& inputMapFile)
 {
	 inputMapFile.clear();
	 inputMapFile.seekg(0, std::ios::beg);
 }

 void MapLoader::Parser::initializeCountry(std::smatch &countryAttributes)
 {
	 using namespace std;

	 std::smatch country;
	 std::smatch continent;

	 const std::regex countryID{ "\\d+" };
	 const std::regex continentID{ "-\\D\\w*" };

	string s = countryAttributes[0].str();

	 // Getting country ID
	 std::regex_search(s, country, countryID);
	 cout << "Country: " << country[0];

	 // Getting continent of the country 
	 std::regex_search(s, continent, continentID);
	 cout << " - Continent: " << continent[0].str().substr(1) << endl;

	// Setting the starting country if it has not already been set 
	 if (s.find('!') != string::npos && !MapLoader::isStartingCountrySet)
	 {
		 MapLoader::isStartingCountrySet = true;
		 cout << " Is the starting country." << endl;

	 }
	 else if (s.find('!') != string::npos && MapLoader::isStartingCountrySet)
	 {
		 cout << "Invalid Map File. More than one starting country specified.";
	 
	 }


 }

 void MapLoader::Parser::initializeAdjacencyList(std::smatch& adjacentCountires)
 {
	 using namespace std;

	 string s = adjacentCountires.str();

	 cout << "List of adjacent countries: " << s << endl;

 }

 