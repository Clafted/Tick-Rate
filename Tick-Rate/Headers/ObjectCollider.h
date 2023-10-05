#ifndef  OBJECTCOLLIDER_H
#define OBJECTCOLLIDER_H

#include <glm/glm.hpp>

enum class SIDE { TOP, BOTTOM, LEFT, RIGHT, NONE };
enum class CORNER { TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT, NONE };

class ObjectCollider
{
public:
	float Left, Right, Top, Bottom;

	ObjectCollider(glm::vec2 * position, int width, int height, bool passiveTop = false, bool passiveBottom = false, bool passiveLeft = false, bool passiveRight = false);
	~ObjectCollider();
	
	// Getters
	bool Active() const;
	bool isCollidingWith(ObjectCollider& other) const;
	
	glm::vec2 getCorner(CORNER corner);

	float getXCollision(ObjectCollider& other);
	float getYCollision(ObjectCollider& other);

	// Returns the side on which other ObjectCollider is from this ObjectCollider
	SIDE getSideLocatedX(ObjectCollider& other);
	SIDE getSideLocatedY(ObjectCollider& other);

	// Setters
	void setPositionPointer(glm::vec2* position);
	void setIsActive(bool state);

	// Modifiers
	void updateCoordinates();
	void addPassiveSide(SIDE x, SIDE y);
	void changeHitbox(int width, int height);

private:
	glm::vec2 * position;
	
	bool passiveTop, passiveBottom, passiveLeft, passiveRight;
	bool active = true;
	int width, height;
	float correctionX = 0.0f, correctionY = 0.0f;
};

#endif // ! OBJECTCOLLIDER_H

