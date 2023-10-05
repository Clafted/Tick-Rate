#include "../Headers/ObjectCollider.h"
#include <iostream>
#include <string>
ObjectCollider::ObjectCollider(glm::vec2* position, int width, int height, bool passiveTop, bool passiveBottom, bool passiveLeft, bool passiveRight)
: position(position), width(width), height(height), passiveTop(passiveTop), passiveBottom(passiveBottom), passiveLeft(passiveLeft), passiveRight(passiveRight)
{
	updateCoordinates();
}

ObjectCollider::~ObjectCollider()
{}

// Setters
void ObjectCollider::setPositionPointer(glm::vec2* position)
{
	this->position = position;
	updateCoordinates();
}

void ObjectCollider::setIsActive(bool state)
{
	active = state;
}



void ObjectCollider::updateCoordinates()
{
	Top = position->y + height;
	Bottom = position->y;
	Left = position->x - (width / 2.0f);
	Right = position->x + (width / 2.0f);
}

/* Sets a side through which other AnimateObjects can pass
 * Left/Right make corresponding sides passive, and UP/DOWN makes the top/bottom passive */
void ObjectCollider::addPassiveSide(SIDE x, SIDE y)
{
	if (x == SIDE::LEFT)
		passiveLeft = true;
	else if (x == SIDE::RIGHT)
		passiveRight = true;

	if (y == SIDE::TOP)
		passiveTop = true;
	else if (y == SIDE::BOTTOM)
		passiveBottom = true;
}


void ObjectCollider::changeHitbox(int width, int height)
{
	this->width = width;
	this->height = height;

	updateCoordinates();
}

bool ObjectCollider::Active() const
{
	return active;
}

bool ObjectCollider::isCollidingWith(ObjectCollider& other) const
{
	return !(Bottom > other.Top || Top < other.Bottom || Right < other.Left || Left > other.Right);
}

float ObjectCollider::getXCollision(ObjectCollider& other)
{
	if (!active || !isCollidingWith(other)) return 0.0f;

	if (position->x < other.position->x)
	{
		return other.Left - Right;
	}
	else if (position->x > other.position->x)
	{
		return other.Right - Left;
	}

	return 0.0f;
}

float ObjectCollider::getYCollision(ObjectCollider& other)
{

	if (!active || !isCollidingWith(other)) return 0.0f;

	if (position->y + (height / 2.0f) > other.position->y + (other.height / 2.0f))
	{
		return other.Top - Bottom;
	}
	else if (position->y + (height / 2.0f) < other.position->y + (other.height / 2.0f))
	{
		return other.Bottom - Top;
	}
	
	return 0.0f;
}

glm::vec2 ObjectCollider::getCorner(CORNER corner)
{
	switch (corner)
	{
	case CORNER::TOP_LEFT:
		return glm::vec2(Left, Top);
	case CORNER::TOP_RIGHT:
		return glm::vec2(Right, Top);
	case CORNER::BOTTOM_LEFT:
		return glm::vec2(Left, Bottom);
	case CORNER::BOTTOM_RIGHT:
		return glm::vec2(Right, Bottom);
	}
}

SIDE ObjectCollider::getSideLocatedX(ObjectCollider& other)
{
	if (position->x < other.position->x) return SIDE::RIGHT;
	if (position->x > other.position->x) return SIDE::LEFT;
	return SIDE::NONE;
}

SIDE ObjectCollider::getSideLocatedY(ObjectCollider& other)
{
	if (position->y + (height / 2.0f) > other.position->y + (height / 2.0f)) SIDE::TOP;
	if (position->y + (height / 2.0f) < other.position->y + (height / 2.0f)) SIDE::BOTTOM;
	return SIDE::NONE;
}