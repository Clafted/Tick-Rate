#ifndef MAP_H
#define MAP_H

#include <glm/glm.hpp>
#include <iostream>

class Map
{
public:
    enum MAPSIDE { ABOVE, BELOW, LEFT, RIGHT };
    enum TILETYPE { AIR = -1, GRASS = 0, ENEMY = 10, PLATFORM = 11 };

    int* data;
    const int size, width, height, tileWidth, tileHeight;

    Map(int* map, int size, int width, int tileWidth, int tileHeight)
        : data(map), size(size), width(width), height((size / sizeof(int)) / width), tileWidth(tileWidth), tileHeight(tileHeight){}

    //Return the code of the tile at the chosen position
    int GetSide(int index, MAPSIDE side, bool assumeBorder = false)
    {
        switch (side)
        {
        case ABOVE:
            return (index < width) ? -((int)assumeBorder) : data[index - width];
        case BELOW:
            return (index >= size / sizeof(int) - width) ? -((int)assumeBorder) : data[index + width];
        case LEFT:
            return (index % width == 0) ? -((int)assumeBorder) : data[index - 1];
        case RIGHT:
            return (index % width == width - 1) ? -((int)assumeBorder) : data[index + 1];
        }
    }

    //Return the value of the tile using given WORLD coordinates.
    int TileAt(float x, float y)
    {
        if (x < 0 || y < 0 || (int)x / tileWidth >= width || (int)y / tileHeight >= height) return -1;
        return data[((int)x / tileWidth) + ((int)(height - (y / tileHeight)) * width)];
    }

    int TileAt(glm::vec2 position)
    {
        return TileAt(position.x, position.y);
    }

    //Return the number of elements in the data
    int Size() { return size / sizeof(int); }
};

#endif