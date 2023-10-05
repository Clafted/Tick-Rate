#pragma once

#include "MovingObject.h"

//A subclass of Object which allows both for animation, movement, and collision.
class AnimatedObject : public MovingObject
{
public:
	AnimatedObject(Spritesheet * spriteSheet, glm::vec2 position = glm::vec2(0.0f), int width = 20, int height = 20, float animationRate = 0.5f)
		: MovingObject(spriteSheet, position, width, height), spriteSheet(*spriteSheet), animationTimer(animationRate)
	{
		timers.push_back(&animationTimer);
	};
	virtual ~AnimatedObject()
	{};

	//Animation
	void addState(int numberOfFrames, int yOffset);
	void setState(int index) 
	{
		spriteSheet.setState(index);
		updateTextureCoordinates();
	}
 	void setChangesFrames(bool state) { changesFrames = state; }
	void setAnimationRate(float rate);
	void nextFrame();

	//Override
	virtual void update(float dT);

protected:
	
	//ANIMATION
	Spritesheet& spriteSheet;
	bool changesFrames = true;
	Timer animationTimer;

	void changeHitBox(int width, int height);
};

