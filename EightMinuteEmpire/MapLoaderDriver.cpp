/*#include "Map.h"
#include "MapLoader.h"
#include <iostream>
#include <filesystem>
#include <string>


int main()

{

	MapLoader* mapLoader1 = nullptr;

	while (!mapLoader1)
		mapLoader1 = MapLoader::initiateMapPicker();

	std::cout << *mapLoader1 << std::endl;
	GraphWorld::Map* map1 = new GraphWorld::Map(&mapLoader1->getMapName(), mapLoader1->getNumCountries(), mapLoader1->getNumContinents());

	try 
	{
		if (!mapLoader1->load(map1))
		{
			delete mapLoader1;
			delete map1;
			system("PAUSE");
			return 0;
		}
			
	}
	catch (...)
	{
		//Cleanup
		delete map1;
		delete mapLoader1;
	}


	map1->printMap();

	MapLoader* mapLoader2 = nullptr;

	while (!mapLoader2)
		mapLoader2 = MapLoader::initiateMapPicker();

	std::cout << *mapLoader2 << std::endl;
	GraphWorld::Map* map2 = new GraphWorld::Map(&mapLoader2->getMapName(), mapLoader2->getNumCountries(), mapLoader2->getNumContinents());


		if (!mapLoader2->load(map2))
		{
			delete mapLoader2;
			delete map2;
			system("PAUSE");
			return 0;
		}

		map2->printMap();




	delete mapLoader1;
	delete mapLoader2;
	delete map2;
	delete map1;
	system("PAUSE");
	return 0;

}
*/