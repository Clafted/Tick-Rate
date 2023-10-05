#pragma once

#ifndef PLAYER_H
#define PLAYER_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Entity.h"
#include "Anchor.h"


class Player : public Entity
{
public:
	Player(Spritesheet* spritesheet, glm::vec2 position = glm::vec2(0.0f, 0.0f), int width = 20, int height = 20);
	~Player() {};

	void update(float dT);

	// Collisions
	void handleCollisionWith(MovingObject* other);

	// General movement
	void slowDownX();

	// Wall movement
	void ledgeGrab(SIDE side, Map * map);
	void climbLedge(Map * map);
	void wallBoost();
	void wallJump(SIDE side);
	void wallSlide();

private:
	Anchor anchor = Anchor(&position);
	bool grabbingLedge = false;
	int reach = 10;

	Timer climbTimer = Timer(1.0f);
};

#endif PLAYER_H