#ifndef MAPREADER_H
#define MAPREADER_H

#include <fstream>
#include <string>
#include "Map.h"

class MapReader
{
public:
	MapReader(){};
	~MapReader(){};

	static MapReader getInstance();
	static void loadMap(std::string filePath);
	static Map* CurrentMap() { return currentMap; }

private:
	static MapReader * instance;
	static Map * currentMap;
};

#endif