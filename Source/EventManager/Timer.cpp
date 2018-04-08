//-----------------------------------------------------------
// File: Timer.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "Timer.h"

Timer::Timer()
{
	secondsPerCount = 0;
	fpsDelta = 0;
	prevFPSTime = 0;
}

void Timer::Start()
{
	__int64 countsPerSec;
	QueryPerformanceFrequency( (LARGE_INTEGER*) &countsPerSec );
	secondsPerCount = 1.0 / countsPerSec;

	// Get the current time
	__int64 currTime;
	QueryPerformanceCounter( (LARGE_INTEGER*) &currTime );
	startTime = currTime;

	fpsDelta = 0;
	prevFPSTime = 0;
}

void Timer::TickFPSCounter()
{
	// add to the delta (in seconds)
	__int64 currTime;
	QueryPerformanceCounter( (LARGE_INTEGER*) &currTime );
	fpsDelta += ((currTime - prevFPSTime) * secondsPerCount );

	// Static counter. Incremented each tick
	static int fpsCounter = 59;
	fpsCounter++;

	// a second has passed
	if( fpsDelta >= 1.0 )
	{
		// reset the delta
		fpsDelta = 0.0;

		// store then reset the fps counter
		fps = fpsCounter;
		fpsCounter = 0;
	}

	prevFPSTime = currTime;
}