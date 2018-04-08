//-----------------------------------------------------------
// File: AnimationClock.h
// Description: This is a simple clock used in the game to keep 
//				track of any in game time changes. This clock
//				must have update called every frame in order to
//				increment time.
//-----------------------------------------------------------

#ifndef ANIMATION_CLOCK_H
#define ANIMATION_CLOCK_H

class AnimationClock
{
private:
	float deltaTime;

	bool stopped;

public:
	AnimationClock();

	// Changes the stopped flag
	void Start();

	// Update the deltatime
	void Tick( float dt );

	// Resets the clock
	void Stop();

	bool GetStopped() { return stopped; }
	float GetDeltaTime() { return deltaTime; }

	void SetStopped( bool value ) { stopped = value; }
};

#endif