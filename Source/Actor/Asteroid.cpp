//-----------------------------------------------------------
// File: Asteroid.cpp
//-----------------------------------------------------------

#include "Asteroid.h"

#include "../Graphics/Camera.h"
#include "../Graphics/Model2D.h"


// Constant definitions
const float Asteroid::MAX_SIZE = 6.0f;
const float Asteroid::MIN_SIZE = 0.25f;

const int Asteroid::MAX_MASS = 200;
const int Asteroid::MIN_MASS = 40;


Asteroid::Asteroid(const Vector2& pos, const float& size, float rot,
                   const Model2D& model, const Vector2& velocity, float mass, int health) :
	ActorBase(pos, Vector2(size, size), rot, model, velocity, mass), health(health), alive(true)
{ }

void Asteroid::Update(float dt)
{
	// Update the position
	world.pos += velocity * dt;

	// Check that the position doesn't wrap around the screen
	WrapPosition();
}

void Asteroid::Render(const Camera& camera) const
{
	if(alive)
	{
		// Render the object to screen
		ActorBase::Render(camera);
	}
}

void Asteroid::ReduceHealth()
{
	--health;

	// If the asteroid is still alive then shrink it and remove mass
	if(health > 0)
	{
		world.scale /= 2;
		mass /= 10;
	}
	else
	{
		// If health is 0 or less then set it's flag for removal
		KillAsteroid();
	}
}

void Asteroid::KillAsteroid()
{
	// Sets the alive flag so that the game will remove the asteroid
	alive = false;
}

void Asteroid::SplitAsteroids(Asteroid& asteroid)
{
	// make sure that both asteroids are alive
	if(!alive || !asteroid.GetAlive()) return;

	// Set the velocity to push the objects apart
	Vector2 res(0, 0);
	if(CheckCollision(asteroid, res))
	{
		AdvancedMoveApart(asteroid, res);
		PerformCollisionCalculation(asteroid);

		Vector2 tangent = Vector2(-velocity.y, velocity.x);

		// Set the velocity to push the objects apart
		velocity += tangent * 0.5f;
		asteroid.velocity += -tangent * 0.5f;
	}
}
