//-----------------------------------------------------------
// File: Timer.cpp
//-----------------------------------------------------------

#include "Timer.h"


void Timer::Start()
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	secondsPerCount = 1.0 / countsPerSec;

	// Get the current time
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	startTime = currTime;

	fpsDelta = 0;
	prevFPSTime = 0;
}

void Timer::TickFPSCounter()
{
	// add to the delta (in seconds)
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	fpsDelta += ((currTime - prevFPSTime) * secondsPerCount);

	// Static counter. Incremented each tick
	static int fpsCounter = 59;
	fpsCounter++;

	// a second has passed
	if(fpsDelta >= 1.0)
	{
		// reset the delta
		fpsDelta = 0.0;

		// store then reset the fps counter
		fps = fpsCounter;
		fpsCounter = 0;
	}

	prevFPSTime = currTime;
}

__int64 Timer::GetHighResTimer() const
{
	__int64 currTime;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currTime));
	return currTime;
}

double Timer::GetCurrTimeSecs() const
{
	__int64 currTime;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currTime));
	return currTime * secondsPerCount;
}

double Timer::GetDeltaTime() const
{
	__int64 currTime;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currTime));
	return (currTime - startTime) * secondsPerCount;
}
