//-----------------------------------------------------------
// File: MeteorShowerLevel.h
// Description: Inherits Level. Represents a level that fires asteroids 
//				onto the screen. Player isn't present and level
//				cannot be completed, therefore must be canceled.
//				Asteroids do not wrap around although they will be 
//				removed by gameLogic code. A timer determines whether
//				more asteroids need to be spawned
//-----------------------------------------------------------

#ifndef METEOR_SHOWER_LEVEL_H
#define METEOR_SHOWER_LEVEL_H

#include "Level.h"
#include "../EventManager/AnimationClock.h"

class MeteorShowerLevel : public Level
{
public:
	MeteorShowerLevel(GameLogic* parent, int level);

	// Level runs until externaly cancelled
	bool Complete() override;

	// Update the timer. Check the number of asteroids on screen and add more to come on
	// if there aren't enough
	void Update(float dt) override;

private:
	std::shared_ptr<Asteroid> CreateAsteroid() const;

	// Timer that counts until next spawn
	AnimationClock asteroidTimer;
	float timeTillAsteroids;

	// Constants determining spawn rates
	static const float MAX_TIME_TILL_ASTEROIDS;
	static const float MIN_TIME_TILL_ASTEROIDS;
	static const int MAX_ASTEROIDS_PER_COUNTDOWN;
	static const int MIN_ASTEROIDS_PER_COUNTDOWN;
};

#endif
