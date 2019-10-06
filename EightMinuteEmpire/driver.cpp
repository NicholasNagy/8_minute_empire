//#include "Map.h"
//#include "MapLoader.h"
//#include <iostream>
//#include <filesystem>
//#include <string>
//
//
//int main()
//
//{
//	cout << "Eight Minute Empire - By MAKRAM, AVERY, NICK and KHALED \n";

//	MapLoader* mapLoader = nullptr;
//
//	while (!mapLoader)
//		mapLoader = MapLoader::initiateMapPicker();
//
//	std::cout << *mapLoader << std::endl;
//	GraphWorld::Map* map = new GraphWorld::Map(&mapLoader->getMapName(), mapLoader->getNumCountries(), mapLoader->getNumContinents());
//
//	try
//	{
//		if (!mapLoader->load(map))
//		{
//			delete mapLoader;
//			delete map;
//			system("PAUSE");
//			return 0;
//		}
//
//	}
//	catch (...)
//	{
//		//Cleanup
//		delete map;
//		delete mapLoader;
//	}
//
//	map->printMap();
//
//
//
//	delete mapLoader;
//	delete map;
//	system("PAUSE");
//	return 0;
