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

 std::vector<std::string> MapLoader::getInstalledMaps()
 {
	 std::vector<std::string> maps;

	 /*
	 for (const auto& map : std::filesystem::directory_iterator( getMapsDir() ) )
	 { 
		maps.push_back(map.path().filename().string());
		// log: std::cout << map.path().filename().string() << std::endl;
	 }
	 */
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

	 while ( getline(inputMapFile, line) )
	 {
		 if (line.compare("<eme_countries>") == 0)
		 {
			 // Get Countries here
			 while (getline(inputMapFile, line))
			 {
				 if (line.compare("</eme_countries>") == 0)
					 break;

				 cout << line << endl;
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


