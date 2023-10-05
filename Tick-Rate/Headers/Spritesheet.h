#pragma once

#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include "Atlas.h"
#include <glm/glm.hpp>
#include <vector>

/* A subclass of Atlas that emphasizes the constant switching of frames to simulate animations. 
Use the Atlas class for Textures with less dynamic textures, like backgrounds. */
class Spritesheet : public Atlas
{
public:

	//Constructor
	Spritesheet(std::string imagePath, int numberOfColumns, int numberOfRows, int columnWidth, int rowHeight)
		:Atlas(imagePath, numberOfColumns, numberOfRows, columnWidth, rowHeight)
	{
		states = std::vector<State>{ State(numberOfColumns, 0) };
	}

	//Destructor
	~Spritesheet() 
	{ std::cout << "Destroying Spritesheet with Texture: " + imagePath << std::endl; }

	float X(TEXTURE_CORNER position)
	{
		if (position == TOP_LEFT || position == BOTTOM_LEFT)
		{ return ((columnIterator + ((flippedOnYAxis) ? 1 : 0)) * columnWidth) / (float) width; }
		else 
		{ return ((columnIterator + ((flippedOnYAxis) ? 0 : 1)) * columnWidth) / (float) width; }
	}

	float Y(TEXTURE_CORNER position)
	{
		if (position == TOP_LEFT || position == TOP_RIGHT)
		{ return (height - states[currentState].yOffset) / (float) height; }
		else
		{ return (height - states[currentState].yOffset - rowHeight) / (float) height; }
	}

	void addState(int numberOfFrames, int yOffset)
	{ states.push_back(State(numberOfFrames, yOffset)); }

	void setState(int index)
	{
		if (index == currentState || index >= states.size()) return;
		currentState = index;
		columnIterator = 0;
	}

	void setFlippedOnYAxis(bool state) 
	{ flippedOnYAxis = state; }


	// Set current frame through an index ranging from 0 to # of frames
	void setIndex(int index)
	{
		if (index >= columnCount * rowCount) return;
		columnIterator = index % columnCount;
		rowIterator = index / rowCount;
	}

	//Goes to the next available frame in the state
	void nextFrame()
	{
		if (columnIterator >= states[currentState].numberOfFrames - 1) columnIterator = 0;
		else columnIterator++;
	}

private:
	struct State
	{
		State(int numberOfFrames, int yOffset) :numberOfFrames(numberOfFrames), yOffset(yOffset){}
		const int numberOfFrames, yOffset;
	};

	std::vector<State> states;
	int currentState = 0;
	bool flippedOnYAxis = false;
};

#endif