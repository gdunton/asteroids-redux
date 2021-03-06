//-----------------------------------------------------------
// File: Asteroid.h
// Description: Asteroid inherits physics object and adds code
// to give it the abilty to split into two if health is high enough
//-----------------------------------------------------------

#ifndef ASTEROID_H
#define ASTEROID_H

#include "ActorBase.h"
#include "../Utilities/Vector2.h"

class Camera;
class Model2D;

class Asteroid : public ActorBase
{
public:
	Asteroid() = default;
	Asteroid(const Vector2& pos, const float& size, float rot, const Model2D& model,
	         const Vector2& velocity, float mass, int health);

	void Update(float dt) override;
	void Render(const Camera& camera) const override;

	// Functions give the ability to reduce the health of an asteroid making
	// it smaller and lighter or outright kill the asteroid
	void ReduceHealth();
	void KillAsteroid();

	// Moves two asteroids away from each other and alters their
	// velocities to continue that movement
	void SplitAsteroids(Asteroid& asteroid);

	bool GetAlive() const { return alive; }
	void SetAlive(bool value) { alive = value; }

	void SetModel(const Model2D& newModel) { this->model = newModel; }

	// pulic constants
	static const float MAX_SIZE;
	static const float MIN_SIZE;

	static const int MAX_MASS;
	static const int MIN_MASS;

private:
	int health = 0;
	bool alive = false;
};

#endif
