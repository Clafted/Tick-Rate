#include "../Headers/AnimatedObject.h"


//Apply any necessary changes to the AnimatedObject (position, texture, etc.)
void AnimatedObject::update(float dT)
{
	// Change animation frame if needed
	if (changesFrames && animationTimer.getTimeIsUp())
	{
		spriteSheet.nextFrame();
		updateTextureCoordinates();
	}

	MovingObject::update(dT);
}

//---------------------------------------------------------------------------

//ANIMATION METHODS
void AnimatedObject::addState(int numberOfFrames, int yOffset)
{
	spriteSheet.addState(numberOfFrames, yOffset);
	updateTextureCoordinates();
}

void AnimatedObject::setAnimationRate(float rate) 
{
	animationTimer.setDuration(rate); 
}

void AnimatedObject::nextFrame() 
{
	spriteSheet.nextFrame(); 
}

void AnimatedObject::changeHitBox(int width, int height)
{
	this->width = width;
	this->height = height;

	Left = position.x - (width / 2.0f);
	Right = position.x + (width / 2.0f);
	Top = position.y + height;
	Bottom = position.y;

	objectCollider.changeHitbox(width, height);
}