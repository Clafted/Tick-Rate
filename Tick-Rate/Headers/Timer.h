#pragma once

#include <math.h>

// A class to keep track of time
class Timer
{
public:
	Timer(float duration) : duration(duration)
	{
		timeIsUp = false;
		clock = 0.0f;
		tickCounter = 1;
	}
	~Timer() {}

	void setDuration(float newDuration) { duration = newDuration; }
	bool getTimeIsUp() { return timeIsUp; }

	// Returns the number of times the timer has reached its period
	int numberOfTicksPassed()
	{
		return tickCounter;
	}

	void resetTickCounter()
	{
		tickCounter = 0;
	}

	void resetClock()
	{
		clock = 0.0f;
		tickCounter = 0;
	}
	void updateClock(float period)
	{
		if (duration <= 0.0f)
		{			
			timeIsUp = true;
			return;
		}

		clock += period;
		timeIsUp = (clock >= duration);

		if (timeIsUp)
		{	
			clock = fmod(clock, duration);
			tickCounter++;
		}
	}

private:
	float duration, clock;
	int tickCounter;
	bool timeIsUp;
};

