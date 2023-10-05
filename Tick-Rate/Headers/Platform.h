#pragma once
#ifndef PLATFORM_H
#define PLATFORM_H

#include "AnimatedObject.h"

//A class for moving AnimatedObjects that can only be stood on.
class Platform : public AnimatedObject
{
public:
	Platform(Spritesheet * spritesheet, glm::vec2 position = glm::vec2(0.0f, 0.0f), int width = 20, int height = 20, float animationRate = 0.5f)
		: AnimatedObject(spritesheet, position, width, height, animationRate)
	{
		changesFrames = false;
		solid = false;
	}
	~Platform() {};

	//Collision
	void handleCollisionWith(MovingObject * other) {};
};

#endif