//-----------------------------------------------------------
// File: Bullet.h
// Description: Bullet inherits PhysicsObject adding a count up
//              lifetime.
//-----------------------------------------------------------

#ifndef BULLET_H
#define BULLET_H

#include "PhysicsObject.h"

class Bullet : public PhysicsObject
{
private:
	// Lifetime starts at 0 and works up in seconds to MAX_LIFETIME
	float lifetime;

	static const int DAMAGE;
	static const float MAX_LIFETIME;
	

public:
	Bullet();
	Bullet( const MyVector2& _pos, const MyVector2& _size, float _rot,
		Model2D* _model, const MyVector2& velocity );

	// Move the bullet 
	void Update( float dt );

	// Set the lifetime to the max to kill the bullet
	void KillBullet();

	// Checks the total lifetime against the maximum lifetime
	// to determine if the bullet is still alive;
	bool GetAlive() const;

	// Returns the collision circle on the bullet
	MathTypes::Circle GetCircle() const;

	static const float FIRING_SPEED;
};

#endif