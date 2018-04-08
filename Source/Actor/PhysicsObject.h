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
	
	float mass;

	CollisionShape collision;

public:
	MyVector2 velocity;
	PhysicsObject() : GameEntity(), velocity( MyVector2( 0, 0 ) ), mass( 0.0f ){ }
	PhysicsObject( const MyVector2& pos, const MyVector2& size, float rot,
		Model2D* model, const MyVector2& velocity, float mass );

	virtual void Update( float dt ) = 0;

	// Performs all collision checks and resolutions. Returns whether collision occured
	bool CompleteCollisionCompute( PhysicsObject& object );

	// Deternine whether object intersects another asteroid
	// returns true if collision occured
	bool CheckCollision( PhysicsObject& object, MyVector2& minTranslation );

	// Moves two physicsObjects apart by minTranslation but takes into account
	// whether the world wraps around
	void AdvancedMoveApart( PhysicsObject& ob, MyVector2& minTranslation );

	// Both asteroids will then be moved outside of each other 
	// and their velocities will be recalculated
	void PerformCollisionCalculation( PhysicsObject& object );

	// Accessors
	virtual MathTypes::Circle GetCircle() const;

	MyVector2 GetVelocity() const { return velocity; }
	float GetMass() const { return mass; }
};

#endif