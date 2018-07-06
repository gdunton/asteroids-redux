//-----------------------------------------------------------
// File: ActorBase.h
// Description: ActorBase containes functionality to determine
//				reactions to collisions with other PhysicsObjects.
//				Makes use of the CollisionShape class
//-----------------------------------------------------------

#ifndef ACTORBASE_H
#define ACTORBASE_H

#include "CollisionShape.h"
#include "../Graphics/Model2D.h"

class ActorBase
{
public:

	ActorBase() = default;
	ActorBase(const Vector2& pos, const Vector2& size, float rot,
	          const Model2D& model, Vector2 velocity, float mass);
	virtual ~ActorBase() = default;

	virtual void Update(float dt) = 0;

	// Performs all collision checks and resolutions. Returns whether collision occured
	bool CompleteCollisionCompute(ActorBase& object);

	// Deternine whether object intersects another asteroid
	// returns true if collision occured
	bool CheckCollision(ActorBase& object, Vector2& minTranslation);

	bool CheckCollision(ActorBase& object);

	// Moves two physicsObjects apart by minTranslation but takes into account
	// whether the world wraps around
	void AdvancedMoveApart(ActorBase& ob, Vector2& minTranslation);

	// Both asteroids will then be moved outside of each other 
	// and their velocities will be recalculated
	void PerformCollisionCalculation(ActorBase& object);

	// Accessors
	virtual MathTypes::Circle GetCircle() const;

	Vector2 GetVelocity() const { return velocity; }
	float GetMass() const { return mass; }

	Vector2 GetPos() const { return world.pos; }
	Vector2 GetSize() const { return world.scale; }
	float GetRot() const { return world.rot; }
	const Model2D& GetModel() const { return model; }
	const World& GetWorld() const { return world; }

	void SetWrapAround(bool value) { wrapAroundWorld = value; }

	// Overrideable rendering
	virtual void Render(const Camera& camera) const;

	// Wrap the position around the game world
	void WrapPosition();

protected:

	CollisionShape collision;
	World world;
	Model2D model;

	float mass = 0.0f;
	bool wrapAroundWorld = true;
	Vector2 velocity;
};

#endif
