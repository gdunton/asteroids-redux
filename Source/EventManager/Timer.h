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
private:
	// The frequency of the high res timer
	double secondsPerCount;

	// is updated every second to give an accurate (not calculated) value
	int fps;
	double fpsDelta;
	__int64 prevFPSTime;

	__int64 startTime;

public:
	Timer();

	// Determines the frequency 
	void Start();
	// Returns the high res counter in seconds
	inline double GetCurrTimeSecs()
	{
		__int64 currTime;
		QueryPerformanceCounter( (LARGE_INTEGER*) &currTime );
		return currTime * secondsPerCount;
	}
	// Returns the high res counter in number of ticks
	inline __int64 GetHighResTimer()
	{
		__int64 currTime;
		QueryPerformanceCounter( (LARGE_INTEGER*) &currTime );
		return currTime;
	}
	// Gets the time since the timer was started in seconds
	inline double GetDeltaTime()
	{
		__int64 currTime;
		QueryPerformanceCounter( (LARGE_INTEGER*) &currTime );
		return (currTime - startTime) * secondsPerCount;
	}

	// Counts up until a new fps amount can be determined
	void TickFPSCounter();

	inline double GetSecsPerCount() { return secondsPerCount; }
	inline int GetFPS() { return fps; }

	
};

#endif