#include "../Headers/Player.h"

Player::Player(Spritesheet * spritesheet, glm::vec2 position, int width, int height)
	:Entity(spritesheet, position, width, height)
{
	dangerous = false;
	bouncesOffWalls = false;
	affectedByTickRate = false;
	grabbingLedge = false;

	timers.push_back(&climbTimer);

	maxXVel = 30.0f;
}

// Override
void Player::update(float dT)
{
	Entity::update(dT);

	anchor.setIsActive(grabbingLedge);

	// Update Player bounds and texture
	if (anchor.isActive())
	{
		changeHitBox(20, 20);

		// Update texture
		spriteSheet.setState(2);

		// Move towards anchor
		changePosition(anchor.getCorrection());

		// Flip on y-axis if (Left side is close to anchor than Right)
		spriteSheet.setFlippedOnYAxis(abs(Left - anchor.getAnchorPosition().x) < abs(Right - anchor.getAnchorPosition().x));
		updateTextureCoordinates();

		// Reset velocities
		xVel = 0.0f;
		yVel = 0.0f;	
	}
}

void Player::handleCollisionWith(MovingObject* other)
{
	float forwardSide = 0.0f, grabbedSide = 0.0f;
	
	// if (falling downwards && NOT grabbing a ledge && NOT grounded && other is NOT dangerous && other is solid && within reach)
	if (yVel <= 0.0f && !grabbingLedge && !isGrounded && !other->isDangerous() && other->isSolid() && abs(other->Top - Top) <= reach)
	{
		changeHitBox(20, 20);
		
		// if (to the right of other && within reach from Left)
		if (objectCollider.getSideLocatedX(other->objectCollider) == SIDE::LEFT && abs(Left - other->objectCollider.Right) <= reach)
		{
			grabbingLedge = true;
			forwardSide = Left;
			grabbedSide = other->objectCollider.Right;
		}
		// else if (same thing but with the other side)
		else if (objectCollider.getSideLocatedX(other->objectCollider) == SIDE::RIGHT && abs(other->objectCollider.Left - Right) <= reach)
		{
			grabbingLedge = true;
			forwardSide = Right;
			grabbedSide = other->objectCollider.Left;
		}

		if (grabbingLedge)
		{
			// Anchor onto other MovingObject
			anchor.setOriginOffset(glm::vec2(forwardSide - position.x, Top - Bottom));
			anchor.setObjectAnchor(&other->apparentPosition);
			return;
		}
	}
	else if (grabbingLedge && anchor.anchorPosition == &other->apparentPosition)
	{
		return;
	}

	Entity::handleCollisionWith(other);
}

// Slow down x-Vel if grounded and release any ledges
void Player::slowDownX()
{
	if(isGrounded == true) xVel *= 0.95f;
	grabbingLedge = false;
}

// Search for a ledge to anchor onto and relocate to anchor
void Player::ledgeGrab(SIDE side, Map * map)
{
	// if (anchor exists and is grabbing ledge)
	if (grabbingLedge)
	{
		if (climbTimer.numberOfTicksPassed() > 0)
		{
			climbLedge(map);
			climbTimer.resetTickCounter();
		}
	}
	// else if (falling downwards && not right above ground && NOT grounded)
	else if (yVel <= 0.0f && !isGrounded)
	{
		changeHitBox(20, 20);
		float leadingX = ((side == SIDE::LEFT) ? Left : Right);
		glm::vec2 grabbingPosition(0.0f);

		// for (upper reach limit to top)
		for (int y = Top + reach; y >= Top; y--)
		{
			// for (leadingSide to horizontal reach limit)
			for (int x = leadingX; abs(x - leadingX) <= reach; x += ((side == SIDE::LEFT) ? -1 : 1))
			{
				if (map->TileAt(x, y) != Map::AIR)
				{
					grabbingPosition = glm::vec2(x, y);
					break;
				}
			}
		}

		// if (tile above grabbed-tile is also solid)
		if (map->TileAt(grabbingPosition) == Map::AIR || map->TileAt(grabbingPosition.x, grabbingPosition.y + 1) != Map::AIR) return;

		grabbingLedge = true;

		anchor.setStaticAnchor(grabbingPosition);
		anchor.setOriginOffset(glm::vec2(((side == SIDE::LEFT) ? -(width / 2.0f) : (width / 2.0f)), Top - Bottom));
		anchor.setRelativePosition(glm::vec2(((side == SIDE::LEFT) ? 1 : -1), 0.0f));

		// Start counting until Player climbs ledge
		climbTimer.resetClock();
	}
}

void Player::climbLedge(Map * map)
{
	glm::vec2 movement = glm::vec2(((anchor.getAnchorPosition().x > position.x) ? width : -width), height + 1);
	if (grabbingLedge && map->TileAt(position.x + movement.x, position.y + movement.y + height) == Map::AIR)
	{
		changePosition(movement.x, movement.y);
		grabbingLedge = false;
	}
}

// Launches a player upwards
void Player::wallBoost()
{
	if (isGrounded == true) return;

	if (grabbingLedge == true)
	{
		changePosition(0.0f, reach * 2.0f);
		yVel += jumpHeight;
		grabbingLedge = false;
	}
	else if (collisionSpeedX != 0.0f)
	{
		// Push player back a bit
		xVel *= -0.1f;
		// Boost the player up depending on horizontal collision
		yVel += abs(collisionSpeedX);
	}
}

// Jump off of the wall, kicking in the given direction
void Player::wallJump(SIDE side)
{
	if (grabbingLedge)
	{
		changePosition(0.0f, reach * 2.0f);

		float sideJumpSpeed = (float)sqrt(pow(jumpHeight, 2) - pow(jumpHeight * 0.6f, 2));
		xVel += ((side == SIDE::LEFT) ? sideJumpSpeed : -sideJumpSpeed);
		yVel += (float)sqrt(pow(jumpHeight, 2) - pow(sideJumpSpeed, 2));

		grabbingLedge = false;
	}
	// else if (not touching ground && touching wall && player is not moving away from wall)
	else if (isGrounded == false && abs(collisionSpeedX) != 0.0f && ((side == SIDE::LEFT) != collisionSpeedX > 0.0f))
	{
		// Jump horzontally depending on horizontal collision
		xVel *= -(abs(collisionSpeedX) / (float)jumpHeight);
		yVel += abs((collisionSpeedX * 0.4f) / jumpHeight);
	}
}

// Slow the player's fall against a wall
void Player::wallSlide()
{
	if (grabbingLedge || isGrounded == true || collisionSpeedX == 0.0f) return;
	if (yVel < 0.0f) yVel *= (maxXVel - abs(collisionSpeedX)) / maxXVel;
}