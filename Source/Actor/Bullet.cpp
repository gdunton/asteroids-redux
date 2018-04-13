//-----------------------------------------------------------
// File: Bullet.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "Bullet.h"

// Constant definitions
const int Bullet::DAMAGE = 10;
const float Bullet::MAX_LIFETIME = 2;
const float Bullet::FIRING_SPEED = 60;

Bullet::Bullet()
{
	lifetime = MAX_LIFETIME;
}

Bullet::Bullet(const Vector2& pos, const Vector2& size, float rot, Model2D* model, const Vector2& velocity) : 
	PhysicsObject(pos, size, rot, model, velocity, 10)
{
	lifetime = 0.0f;
}

// Move the bullet 
void Bullet::Update(float dt)
{
	world.pos += velocity * dt;

	// Check the positio against the world size and wrap around if
	// necessary
	WrapPosition();

	// Incriment the lifetime
	lifetime += dt;
}

void Bullet::KillBullet()
{
	lifetime = MAX_LIFETIME;
}

// Checks the total lifetime against the maximum lifetime
// to determine if the bullet is still alive;
bool Bullet::GetAlive() const
{
	return lifetime < MAX_LIFETIME;
}

// Override the default from PhysicsObject
MathTypes::Circle Bullet::GetCircle() const
{
	return MathTypes::Circle(world.pos, world.scale.Length() * model->GetModelRadius());
}
