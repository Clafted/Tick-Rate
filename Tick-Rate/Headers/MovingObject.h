#pragma once
#ifndef MOVINGOBJECT_H
#define MOVINGOBJECT_H

#include "Object.h"
#include "ObjectCollider.h"
#include "Spritesheet.h"
#include "Map.h"
#include "Timer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class MovingObject : public Object
{
public:
	// Object Collision
	ObjectCollider objectCollider;
	glm::vec2 apparentPosition;

	MovingObject(Atlas* atlas, glm::vec2 position = glm::vec2(0.0f), int width = 20, int height = 20)
		: Object(atlas, position, width, height), objectCollider(&position, width, height)
	{
		apparentPosition = position;
	}

	~MovingObject()
	{
		for (Timer* timer : timers) delete timer;
		timers.clear();
	}

	// Updates
	virtual void update(float dT);
	static void setTickRate(float tickRate) { tickRateTimer.setDuration(tickRate); }
	static void updateTickRateClock(float period) { tickRateTimer.updateClock(period); }
	void savePosition() { apparentPosition = position; }
	bool IsVisible() { return isVisible; }

	// Map Collisions
	virtual bool checkMapCollisionX(Map* map);
	virtual bool checkMapCollisionY(Map* map);
	void correctMapCollisionX();
	void correctMapCollisionY();

	// MovingObject Collisions
	virtual void handleCollisionWith(MovingObject* other);
	bool isCollidingWith(MovingObject& other);
	bool isDangerous() { return dangerous; }
	bool const isSolid() { return solid; }

	// Movement
	virtual void accelerate(glm::vec2 acceleration, float dT);
	void setHasGravity(bool hasGravity);
	void setIsGrounded(bool state) { isGrounded = state; }
	void setXVelocity(float xVel);
	void setYVelocity(float yVel);
	void stop();
	int sgn(float value);
	float XVel() const;
	float YVel() const;

protected:

	// UPDATES
	bool affectedByTickRate = true;

	// MOVEMENT
	void move(float dT);
	bool bouncesOffWalls = true;
	bool hasGravity = false;
	bool isGrounded = false;
	float yVel = 0.0f, xVel = 0.0f;
	float maxXVel = 5.0f, maxYVel = 20.0f;
	float xCollisionVel = 0.0f, yCollisionVel = 0.0f;

	// TIMERS
	static Timer tickRateTimer;
	std::vector<Timer*> timers;	

	// MAP COLLISION
	bool dangerous = false;
	bool solid = false;
	bool isVisible = true;
	float collisionSpeedX = 0.0f, collisionSpeedY = 0.0f;
	float correctionX = 0.0f, correctionY = 0.0f;
};

#endif // !MOVINGOBJECT_H

