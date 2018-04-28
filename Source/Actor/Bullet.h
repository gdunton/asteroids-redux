//-----------------------------------------------------------
// File: Bullet.h
// Description: Bullet inherits PhysicsObject adding a count up
//              lifetime.
//-----------------------------------------------------------

#ifndef BULLET_H
#define BULLET_H

#include "PhysicsObject.h"
#include "../Utilities/MathTypes.h"

class Bullet : public PhysicsObject
{
public:
	Bullet() = default;
	Bullet( const Vector2& pos, const Vector2& size, float rot, const Model2D& model, const Vector2& velocity );

	// Move the bullet 
	void Update( float dt ) override;

	// Set the lifetime to the max to kill the bullet
	void KillBullet();

	// Checks the total lifetime against the maximum lifetime
	// to determine if the bullet is still alive;
	bool IsAlive() const;

	// Returns the collision circle on the bullet
	MathTypes::Circle GetCircle() const override;

	constexpr static float FIRING_SPEED = 60.0f;

private:
	// Lifetime starts at 0 and works up in seconds to MAX_LIFETIME
	float lifetime = 0;

	constexpr static int DAMAGE = 10;
	constexpr static float MAX_LIFETIME = 2.0f;
};

#endif