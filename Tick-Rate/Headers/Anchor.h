#ifndef ANCHOR_H
#define ANCHOR_H

#include <glm/glm.hpp>

// A class to hold an object at a certain moving or static position
class Anchor
{
public:
	glm::vec2* anchorPosition = nullptr;

	Anchor(glm::vec2 * objectPosition)
	: objectPosition(objectPosition)
	{}

	Anchor(glm::vec2 staticAnchorPosition, glm::vec2 * objectPosition, glm::vec2 originOffset = glm::vec2(0.0f, 0.0f), glm::vec2 relativePosition = glm::vec2(0.0f, 0.0f))
	: staticAnchorPosition(staticAnchorPosition), objectPosition(objectPosition), originOffset(originOffset), relativePosition(relativePosition)
	{
		active = true;
	}

	~Anchor() 
	{}

	// Chnage the anchor's position
	void setObjectAnchor(glm::vec2 * positionPointer)
	{ 
		anchorPosition = positionPointer;
	}

	// Set a new position to for the anchor
	void setStaticAnchor(glm::vec2 position)
	{
		this->staticAnchorPosition = position;
		anchorPosition = &staticAnchorPosition;
	}

	// Set an offset for the origin, relative to an object's position
	void setOriginOffset(glm::vec2 originOffset)
	{
		this->originOffset = originOffset;
	}

	// Set a position for the origin, relative to the anchor
	void setRelativePosition(glm::vec2 relativePosition)
	{
		this->relativePosition = relativePosition;
	}

	void setIsActive(bool state)
	{
		active = state;
	}

	// Return movement to object's correct position
	glm::vec2 getCorrection()
	{ 
		if (!active) return glm::vec2(0.0f);

		return (*anchorPosition + relativePosition) - (*objectPosition + originOffset); 
	}

	glm::vec2 getAnchorPosition()
	{ 
		if (!active) return glm::vec2(0.0f);
		return *anchorPosition; 
	}

	bool isActive()
	{
		return active;
	}

private:
	bool active = false;

	glm::vec2 * objectPosition;

	// Vector towards where the anchor "clips" onto (relative to bottom-left corner, AKA position)
	glm::vec2 originOffset = glm::vec2(0.0f);

	// Origin's correct position relative to anchor
	glm::vec2 relativePosition = glm::vec2(0.0f);
	glm::vec2 staticAnchorPosition = glm::vec2(0.0f);
};

#endif // !ANCHOR_H
