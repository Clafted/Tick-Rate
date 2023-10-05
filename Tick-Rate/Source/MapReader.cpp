#include "../Headers/MapReader.h"
#include <iostream>
#include <fstream>

//Singleton instance method
MapReader * MapReader::instance = nullptr;
Map* MapReader::currentMap = nullptr;
MapReader MapReader::getInstance()
{
	if (instance == nullptr)
	{
		instance = new MapReader();
	}

	return *instance;
}

//Load a map from a given filePath
void MapReader::loadMap(std::string filePath)
{
	int mapWidth = 0, mapHeight = 0;
	int* mapData = nullptr;

	std::ifstream file(filePath, std::iostream::in);
	std::string token = "_";
	
	if (file.is_open())
	{
		//Look for start of useful data
		while (token != "W:" && token != "")
		{
			token = "";
			file >> token;
		}

		//Assign mapWidth
		token = "";
		file >> token;
		mapWidth = std::stoi(token);
		file >> token;

		//Assign mapHeight
		if (token == "H:")
		{
			token = "";
			file >> token;
			mapHeight = std::stoi(token);
			
		}
		
		token = "";
		file >> token;

		//Initialize mapData array and assign tile values
		mapData = new int[mapWidth * mapHeight];
		if (token == "Map:")
		{
			std::cout << "Reading map" << std::endl;
			for (int i = 0; i < mapWidth * mapHeight; i++)
			{
				token = "";
				file >> token;
				mapData[i] = std::stoi(token);
			}
		}

		//Set the current map to that given in the map file
		currentMap = new Map(mapData, sizeof(int) * mapWidth * mapHeight, mapWidth, 20, 20);
	}
	else {
		std::cout << "Unable to load " + filePath << std::endl;
	}
	
}