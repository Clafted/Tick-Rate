#pragma once
#include "../Headers/Enemy.h"

Enemy::Enemy(Spritesheet * spritesheet, glm::vec2 position, int width, int height)
	: Entity(spritesheet, position, width, height)
{
	dangerous = true;
}

Enemy::~Enemy()
{}