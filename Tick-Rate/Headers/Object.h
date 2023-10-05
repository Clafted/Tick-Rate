#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <array>

#include "Texture.h"
#include "Atlas.h"

/* A class for all things to be rendered. Does not include movement or collision methods and variables. 
For movement and collision, consider using an AnimatedObject instead. */
class Object
{
public:
	unsigned int VAO = 0, VBO = 0;
	float Left, Right, Top, Bottom;
	static Texture* defaultTexture;

	//Constructors
	Object(Atlas* atlas, glm::vec2 position = glm::vec2(0.0f), int width = 20, int height = 20);
	//Destructor
	~Object();

	// Getters
	unsigned int const TextureID() { return atlas.TextureID(); }
	glm::vec2 const Position() { return position; }
	glm::mat4 const ModelMat4() { return modelMat4;}
	float * const ArrayData() { return arrayData;}
	int const ArraySize() { return sizeof(arrayData); }
	int const Width() { return width; }
	int const Height() { return height; }
	
	// Setters
	void setPosition(float x, float y) { position = glm::vec2(x, y); }
	virtual void changePosition(float x, float y);
	void changePosition(glm::vec2 movement) { changePosition(movement.x, movement.y); }
	// Change the portion of the atlas to be displayed
	void setAtlasPositionByIndex(int x, int y);
	void setAtlasBoundsByPixel(int x, int y);
	
protected:
	Atlas& atlas;
	glm::vec2 position = glm::vec2(0.0f, 0.0f);
	glm::mat4 modelMat4 = glm::mat4(1.0f);

	int width, height;

	float arrayData[30]
	{
			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, //Bottom left
			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, //Top left
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f, //Top right
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f, //Top right
			1.0f, 0.0f, 0.0f, 1.0f, 0.0f, //Bottom right
			0.0f, 0.0f, 0.0f, 0.0f, 0.0f //Bottom left
	};

	

	//Update texture coordinates
	void updateTextureCoordinates()
	{
		arrayData[3] = atlas.X(Atlas::BOTTOM_LEFT);
		arrayData[4] = atlas.Y(Atlas::BOTTOM_LEFT);

		arrayData[8] = atlas.X(Atlas::TOP_LEFT);
		arrayData[9] = atlas.Y(Atlas::TOP_LEFT);

		arrayData[13] = atlas.X(Atlas::TOP_RIGHT);
		arrayData[14] = atlas.Y(Atlas::TOP_RIGHT);

		arrayData[18] = atlas.X(Atlas::TOP_RIGHT);
		arrayData[19] = atlas.Y(Atlas::TOP_RIGHT);

		arrayData[23] = atlas.X(Atlas::BOTTOM_RIGHT);
		arrayData[24] = atlas.Y(Atlas::BOTTOM_RIGHT);

		arrayData[28] = atlas.X(Atlas::BOTTOM_LEFT);
		arrayData[29] = atlas.Y(Atlas::BOTTOM_LEFT);
	}	
};

#endif

