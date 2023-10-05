#pragma once
#ifndef SOLIDANIMATEDOBJECT_H
#define SOLIDANIMATEDOBJECT_H

#include "AnimatedObject.h"

class SolidAnimatedObject : public AnimatedObject
{
public:
	SolidAnimatedObject(Spritesheet * spritesheet, glm::vec2 position = glm::vec2(0.0f), int width = 20, int height = 20, float animationRate = 0.5f)
		: AnimatedObject(spritesheet, position, width, height, animationRate)
	{
		solid = true;
		changesFrames = false;
	}

	~SolidAnimatedObject(){};

	//Collision
	void handleCollisionWith(MovingObject * other) {};
};

#endif
