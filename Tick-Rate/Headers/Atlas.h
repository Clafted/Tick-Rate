#ifndef  ATLAS_H
#define ATLAS_H

#include "Texture.h"

/* A child class of Texture class that allows the use of several textures of the same size
which have been packed into a single Atlas image. */
class Atlas : public Texture
{
public:
	enum TEXTURE_CORNER { TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT };

	Atlas(std::string imagePath, int numberOfColumns, int numberOfRows, int columnWidth, int rowHeight)
		:Texture(imagePath), columnCount(numberOfColumns), rowCount(numberOfRows), columnWidth(columnWidth), rowHeight(rowHeight)
	{}
	~Atlas() {}

	// Getters

	unsigned int TextureID() { return textureID; }
	int ColumnWidth() { return columnWidth; }
	int RowHeight() { return rowHeight; }
	glm::vec2 getCornerPosition(TEXTURE_CORNER position)
	{ return glm::vec2(X(position), Y(position)); }

	// Setters

	void setPositionByIndex(int x, int y)
	{
		if (x != -1 && x < columnCount) columnIterator = x;
		if (y != -1 && y < rowCount) rowIterator = y;
	}

	void setPositionByPixel(int x, int y)
	{
		setPositionByIndex(x / columnWidth, y / rowHeight);
	}

	virtual float X(TEXTURE_CORNER position)
	{
		if (position == TOP_LEFT || position == BOTTOM_LEFT)
		{ return columnIterator / (float)columnCount; }
		else 
		{ return (columnIterator + 1) / (float)columnCount; }
	}

	virtual float Y(TEXTURE_CORNER position)
	{
		if (position == TOP_LEFT || position == TOP_RIGHT)
		{ return (rowCount - rowIterator) / (float)rowCount; }
		else 
		{ return ((rowCount - rowIterator - 1) * rowHeight) / (float)height; }
	}

protected:
	int columnCount, rowCount, columnWidth, rowHeight;
	int rowIterator = 0, columnIterator = 0;
};
#endif // ! ATLAS_H
