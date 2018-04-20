//-----------------------------------------------------------
// File: PhysicsObject.h
// Description: PhysicsObject containes functionality to determine
//				reactions to collisions with other PhysicsObjects.
//				Makes use of the CollisionShape class
//-----------------------------------------------------------

#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include "GameEntity.h"

#include "CollisionShape.h"

class PhysicsObject : public GameEntity
{
protected:

	float mass = 0.0f;

	CollisionShape collision;

public:
	Vector2 velocity;
	PhysicsObject() = default;
	PhysicsObject(const Vector2& pos, const Vector2& size, float rot,
	              const Model2D& model, const Vector2& velocity, float mass);

	virtual void Update(float dt) = 0;

	// Performs all collision checks and resolutions. Returns whether collision occured
	bool CompleteCollisionCompute(PhysicsObject& object);

	// Deternine whether object intersects another asteroid
	// returns true if collision occured
	bool CheckCollision(PhysicsObject& object, Vector2& minTranslation);

	bool CheckCollision(PhysicsObject& object);

	// Moves two physicsObjects apart by minTranslation but takes into account
	// whether the world wraps around
	void AdvancedMoveApart(PhysicsObject& ob, Vector2& minTranslation);

	// Both asteroids will then be moved outside of each other 
	// and their velocities will be recalculated
	void PerformCollisionCalculation(PhysicsObject& object);

	// Accessors
	virtual MathTypes::Circle GetCircle() const;

	Vector2 GetVelocity() const { return velocity; }
	float GetMass() const { return mass; }
};

#endif
