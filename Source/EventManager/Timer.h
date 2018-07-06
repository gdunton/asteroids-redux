//-----------------------------------------------------------
// File: Timer.h
// Description: High frequency timer for keeping track of CPU time.
//				Can return cpu ticks or seconds for delta time
//-----------------------------------------------------------

#ifndef TIMER_H
#define TIMER_H

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class Timer
{
public:
	// Determines the frequency 
	void Start();

	// Returns the high res counter in seconds
	double GetCurrTimeSecs() const;

	// Returns the high res counter in number of ticks
	__int64 GetHighResTimer() const;

	// Gets the time since the timer was started in seconds
	double GetDeltaTime() const;

	// Counts up until a new fps amount can be determined
	void TickFPSCounter();

	double GetSecsPerCount() const { return secondsPerCount; }
	int GetFPS() const { return fps; }

private:
	// The frequency of the high res timer
	double secondsPerCount = 0;

	// is updated every second to give an accurate (not calculated) value
	int fps = 0;
	double fpsDelta = 0;
	__int64 prevFPSTime = 0;

	__int64 startTime = 0;
};

#endif
