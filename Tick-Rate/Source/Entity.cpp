#include "../Headers/Entity.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

//A subclass of AnimatedObjects meant to represent living things.
Entity::Entity(Spritesheet* spritesheet, vec2 position, int width, int height)
: AnimatedObject(spritesheet, position, width, height)
{
	hasGravity = true;
	solid = false;
	affectedByTickRate = false;

	timers.push_back(&jumpCooldownTimer);
	timers.push_back(&damageCooldowntimer);
}

//Destructor
Entity::~Entity()
{
	std::cout << "Destroying entity";
}

//Override AnimatedObject update method
void Entity::update(float dT)
{
	AnimatedObject::update(dT);

	isVisible = (damageCooldowntimer.numberOfTicksPassed() > 0);

	if (isGrounded)
	{
		xVel *= (1.0f - dT);
		if (abs(xVel) > 17.0f)
		{
			spriteSheet.setState(1);
			changeHitBox(20, 17);
			spriteSheet.setFlippedOnYAxis(xVel < 0.0f);
			updateTextureCoordinates();
			animationTimer.setDuration(0.04f + ((maxXVel - abs(xVel)) / maxXVel));
		}
		else
		{
			spriteSheet.setState(0);
			changeHitBox(14, 24);
			updateTextureCoordinates();
			animationTimer.setDuration(0.5f);
		}
	}
	else 
	{
		spriteSheet.setState(0);
		changeHitBox(14, 24);
		updateTextureCoordinates();
		animationTimer.setDuration(0.5f);
	}
}

//-----------------------------------------------------------------------

//Entity interaction methods
//Kill the Entity object
void Entity::unalive()
{
	std::cout << "Ded";
}

void  Entity::handleCollisionWith(MovingObject * other)
{
	// if (other is dangerous || other is NOT solid || deeply colliding)
	if (other->isDangerous() || !other->isSolid() || (other->isSolid() && abs(objectCollider.getYCollision(other->objectCollider)) > 2.0f && abs(objectCollider.getXCollision(other->objectCollider)) > (width / 2.0f)))
	{	
		// if (can be damaged)
		if (damageCooldowntimer.numberOfTicksPassed() > 0) damage();
		return;
	}

	MovingObject::handleCollisionWith(other);
}

//Decrements lives and resets damgeCooldownTimer
void Entity::damage(int dmg)
{
	if (damageCooldowntimer.numberOfTicksPassed() > 0)
	{
		std::cout << "\ndamage";
		lives -= dmg;
		damageCooldowntimer.resetClock();
	}
	if (lives <= 0) unalive();
}

//-----------------------------------------------------------------------

//MOVEMENT METHODS
void Entity::jump()
{
	if (isGrounded && jumpCooldownTimer.numberOfTicksPassed() > 0)
	{	
		yVel += (float)jumpHeight;
		isGrounded = false;

		jumpCooldownTimer.resetClock();
	}
}

//Accelerate only when grounded
void Entity::accelerate(glm::vec2 acceleration, float dT)
{
	if (isGrounded == false) return;
	AnimatedObject::accelerate(acceleration, dT);
}



