#include "../Headers/Object.h"

Object::Object(Atlas* atlas, glm::vec2 position, int width, int height)
	:atlas(*atlas), position(position), width(width), height(height)
{
	modelMat4 = glm::translate(modelMat4, glm::vec3(position, 0.0f));

	Left = position.x - (width / 2.0f);
	Right = position.x + (width / 2.0f);
	Top = position.y + height;
	Bottom = position.y;

	arrayData[0] = -(float)atlas->ColumnWidth() / 2.0f;
	arrayData[1] = 0.0f;

	arrayData[5] = -(float)atlas->ColumnWidth() / 2.0f;
	arrayData[6] = (float)atlas->RowHeight();

	arrayData[10] = (float)atlas->ColumnWidth() / 2.0f;
	arrayData[11] = (float)atlas->RowHeight();

	arrayData[15] = (float)atlas->ColumnWidth() / 2.0f;
	arrayData[16] = (float)atlas->RowHeight();

	arrayData[20] = (float)atlas->ColumnWidth() / 2.0f;
	arrayData[21] = 0.0f;

	arrayData[25] = -(float)atlas->ColumnWidth() / 2.0f;
	arrayData[26] = 0.0f;
}

Object::~Object()
{
	std::cout << "Destroying Object" << std::endl;
	delete[] & arrayData;
}

void Object::changePosition(float x, float y)
{
	position.x += x;
	position.y += y;

	Left = position.x - (width / 2.0f);
	Right = position.x + (width / 2.0f);
	Top = position.y + height;
	Bottom = position.y;

	modelMat4 = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
}

void Object::setAtlasPositionByIndex(int x, int y)
{
	atlas.setPositionByIndex(x, y);
	updateTextureCoordinates();
}

void Object::setAtlasBoundsByPixel(int x, int y)
{
	atlas.setPositionByPixel(x, y);
	updateTextureCoordinates();
}