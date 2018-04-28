//-----------------------------------------------------------
// File: AnimationClock.cpp
//-----------------------------------------------------------

#include "AnimationClock.h"

AnimationClock::AnimationClock()
{
	deltaTime = 0.0f;
	stopped = true;
}

void AnimationClock::Start()
{
	stopped = false;
}

void AnimationClock::Tick( float dt )
{
	if( !stopped )
	{
		deltaTime += dt;
	}
}

void AnimationClock::Stop()
{
	stopped = true;
	deltaTime = 0;
}