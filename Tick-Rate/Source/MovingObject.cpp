#include "../Headers/MovingObject.h"

//UPDATE METHODS 
Timer MovingObject::tickRateTimer(0.0f);

//Apply any necessary changes to the MovingObject (position, texture, etc.)
void MovingObject::update(float dT)
{
	// Increment timer variables
	for (Timer* timer : timers)
	{
		timer->updateClock(dT);
	}

	// Apply gravity
	if (hasGravity == true) yVel -= 98.0f * dT;

	// Move character
	move(dT);

	// Reset collision velocities
	xCollisionVel = 0.0f;
	yCollisionVel = 0.0f;

	// Re-position side coordinates
	Left = position.x - (width / 2.0f);
	Right = position.x + (width / 2.0f);
	Top = position.y + height;
	Bottom = position.y;
	objectCollider.updateCoordinates();

	// Update position variables if tickRate is surpassed or if MovingObject is not restricted by tickRate.
	if (tickRateTimer.getTimeIsUp() || affectedByTickRate == false)
	{
		apparentPosition = position;
	}

	// Update world matrix
	modelMat4 = glm::translate(glm::mat4(1.0f), glm::vec3(apparentPosition, 0.0f));
}

//---------------------------------------------------------------------------

//MOVEMENT METHODS
void MovingObject::setXVelocity(float xVel) { this->xVel = xVel; }
void MovingObject::setYVelocity(float yVel) { this->yVel = yVel; }
float MovingObject::XVel() const { return xVel; }
float MovingObject::YVel() const { return yVel; }

int MovingObject::sgn(float value) { return (0 < value) - (value < 0); }

//Apply acceleration on the object
void MovingObject::accelerate(glm::vec2 acceleration, float dT)
{
	acceleration *= dT;
	if (abs(xVel + acceleration.x) <= maxXVel) xVel += acceleration.x;
	if (abs(yVel + acceleration.y) <= maxYVel) yVel += acceleration.y;
}

//Move the MovingObject by a factor of the deltaTime
void MovingObject::move(float dT)
{
	position.x += (xVel + xCollisionVel) * dT * 5;
	position.y += (yVel + yCollisionVel) * dT * 5;
}

//Stops the MovingObject from moving
void MovingObject::stop() { if (isGrounded) xVel = 0.0f; }

//Enable/disable gravity
void MovingObject::setHasGravity(bool hasGravity) { this->hasGravity = hasGravity; }

//---------------------------------------------------------------------------

//COLLISION METHODS

// Correct that MovingObject's x-position upon collision
void MovingObject::correctMapCollisionX()
{
	collisionSpeedX = xVel;

	// Correct collisions
	changePosition(correctionX, 0.0f);
	std::cout << "XCorrection: " + std::to_string(correctionX);

	// Modify xVelocity based on characteristics.
	xVel *= (bouncesOffWalls == true) ? -1.0f : 0.5f;
}

// Correct that MovingObject's y-position upon collision
void MovingObject::correctMapCollisionY()
{
	collisionSpeedY = yVel;

	// Correct collisions
	changePosition(0.0f, correctionY);
	yVel *= 0.5f;
}

// Check collision with the given map. Assumes that tiles are square.
bool MovingObject::checkMapCollisionX(Map* map)
{
	float totalXVel = xVel + xCollisionVel;
	correctionX = 0.0f;
	collisionSpeedX = 0.0f;

	float collidingSide = (((totalXVel) > 0.0f) ? Right : Left);

	// Iterate from the top to the bottom
	for (int y = 1; y < height - 1; y++)
	{
		// if (tile on the leadingSide is solid)
		if (map->TileAt(collidingSide, Top - y) != Map::AIR)
		{
			correctionX = -glm::mod(collidingSide, (float)map->tileWidth);
			if (totalXVel < 0.0f) correctionX += map->tileWidth;
			return abs(correctionX) > 0.0f;
		}
	}

	return false;
}

// Check collision with the given map. Assumes that tiles are square.
bool MovingObject::checkMapCollisionY(Map* map)
{
	float totalYVel = yVel + yCollisionVel;

	correctionY = 0.0f;
	collisionSpeedY = 0.0f;

	float collidingSide = ((totalYVel > 0.0f) ? Top : Bottom);

	// Iterate from the top to the bottom
	for (int x = 3; x < width - 3; x++)
	{
		// if (collidingSide is colliding)
		if (map->TileAt(Left + x, collidingSide) != Map::AIR)
		{
			// Calculate correction
			correctionY = -glm::mod(collidingSide, (float)map->tileHeight);
			if (totalYVel <= 0.0f) correctionY += map->tileHeight;

			if(correctionY > 0.0f) isGrounded = true;

			// Makes sure that collision isn't large enough to be a horizontal collision
			return abs(correctionY) > 0.0f;
		}
	}

	isGrounded = false;
	return false;
}

//Handle an existing collision between two MovingObjects
void MovingObject::handleCollisionWith(MovingObject* other)
{	
	float xCorrection = objectCollider.getXCollision(other->objectCollider),
		yCorrection = objectCollider.getYCollision(other->objectCollider);

	if (abs(xCorrection) < abs(yCorrection))
	{
		changePosition(xCorrection, 0.0f);

		if (abs(xVel) < abs(other->xVel) || (xVel > 0.0f != other->xVel > 0.0f))
		{
			xVel = other->xVel;
		}
	}
	else
	{
		if (yCorrection > 0.0f) isGrounded = true;
		
		changePosition(0.0f, yCorrection);

		// if (this is moving slower than other || this is going in the opposite direction
		if (abs(yVel) < abs(other->yVel) || sgn(yVel) != sgn(other->yVel))
		{
			yVel = other->yVel;
			xCollisionVel = other->xVel;
		}
	}	
}

bool MovingObject::isCollidingWith(MovingObject& other)
{
	// if (both or neither are affected by tickRate)
	if (other.affectedByTickRate == affectedByTickRate)
	{
		// both use real positions
		objectCollider.setPositionPointer(&position);
		other.objectCollider.setPositionPointer(&other.position);
	}
	else if(other.affectedByTickRate) {
		// this uses real position to check other's apparent position
		objectCollider.setPositionPointer(&position);
		other.objectCollider.setPositionPointer(&other.apparentPosition);
	}
	else {
		// this uses apparent position to check other's real position
		objectCollider.setPositionPointer(&apparentPosition);
		other.objectCollider.setPositionPointer(&other.position);
	}

	return objectCollider.isCollidingWith(other.objectCollider);
}


