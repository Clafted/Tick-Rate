#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

class Enemy : public Entity
{
public:
	Enemy(Spritesheet * spritesheet, glm::vec2 position = glm::vec2(0.0f, 0.0f), int width = 20, int height = 20);
	~Enemy();
};

#endif