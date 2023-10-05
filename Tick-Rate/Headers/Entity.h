#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include "AnimatedObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

//A subclass of AnimatedObject with functionality of life.
class Entity : public AnimatedObject
{
public:	
	Entity(Spritesheet * spritesheet, glm::vec2 position = glm::vec2(0.0f), int width = 20, int height = 20);
	virtual ~Entity();
	
	// Movement
	void accelerate(glm::vec2 acceleration, float dT);
	void jump();
	void update(float dT);
	
	// Entity Interaction
	virtual void damage(int dmg = 1);

	/*Override of the MovingObject class's handleCollisionWith() method.
	Calls damage() in case other is dangerous or is extensively colliding*/
	virtual void handleCollisionWith(MovingObject * other);
	int numLives() { return this->lives; }
	void setDamageCooldown(float seconds = 1.0f) { damageCooldowntimer.setDuration(seconds); }
	virtual void unalive();

protected:
	Timer jumpCooldownTimer = Timer(0.2f);
	Timer damageCooldowntimer = Timer(1.5f);
	int jumpHeight = 25;
	int lives = 3;
	
	bool solid = true;
};

#endif