#pragma once
#ifndef STATE_H
#define STATE_H

class State
{
public:
	virtual State* update(float deltaTime) = 0;

	virtual void enter() = 0;
	virtual void exit() = 0;
};

#endif // !STATE_H
