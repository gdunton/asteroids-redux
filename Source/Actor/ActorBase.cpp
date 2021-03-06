//-----------------------------------------------------------
// File: PhysicsObject.cpp
//-----------------------------------------------------------

#include "ActorBase.h"
#include <utility>
#include "../GameMain/Globals.h"

ActorBase::ActorBase(const Vector2& pos, const Vector2& size, float rot,
                     const Model2D& model, Vector2 velocity, float mass) noexcept
	: world(pos, size, rot), model(model), mass(mass), velocity(std::move(velocity))
{
	// is model set by now?
	collision.Initialize(model.GetPoints());
}

bool ActorBase::CompleteCollisionCompute(ActorBase& object)
{
	Vector2 res;
	if(CheckCollision(object, res)) // if collision occures
	{
		// Move the objects apart and then perform the elastic
		// collision calculations
		AdvancedMoveApart(object, res);
		PerformCollisionCalculation(object);

		return true;
	}
	return false;
}

bool ActorBase::CheckCollision(ActorBase& object, Vector2& minTranslation)
{
	// Get the distance between the two objects
	Vector2 direction;
	if(wrapAroundWorld)
	{
		GetShortestWrappedDistance(object.GetPos(), world.pos, WORLD_WIDTH, WORLD_HEIGHT, direction);
	}
	else
	{
		direction = world.pos - object.GetPos();
	}

	const float r1 = GetCircle().radius;
	const float r2 = object.GetCircle().radius;

	// if length is less than the collision radii
	if(LengthSquared(direction) < ((r1 + r2) * (r1 + r2)))
	{
		// Set the world value on both collision shapes
		collision.SetWorld(world);
		object.collision.SetWorld(object.world);

		// Do deeper collision check using the collision geometry
		return collision.CheckCollision(object.collision, minTranslation);
	}
	return false;
}

bool ActorBase::CheckCollision(ActorBase& object)
{
	[[maybe_unused]] Vector2 unusedVector2;
	return CheckCollision(object, unusedVector2);
}

void ActorBase::PerformCollisionCalculation(ActorBase& object)
{
	// Check that the objects are in each other
	Vector2 direction;

	// Get the distance between the two objects
	if(wrapAroundWorld)
	{
		GetShortestWrappedDistance(world.pos, object.GetPos(),
		                           WORLD_WIDTH, WORLD_HEIGHT, direction);
	}
	else
	{
		direction = world.pos - object.GetPos();
	}

	// Get normalized distance and tangents between two objects
	Vector2 normal = direction;
	Normalize(normal);
	const Vector2 tangent = Vector2(-normal.y, normal.x);

	{
		// Change both velocities from the collision
		// Get both masses
		const float m1 = mass;
		const float m2 = object.GetMass();

		// Get both the velocities
		const Vector2 v1 = velocity;
		const Vector2 v2 = object.GetVelocity();

		// Create the normal and tangent velocities using the dot product
		const float v1n = Dot(normal, v1);
		const float v1t = Dot(tangent, v1);
		const float v2n = Dot(normal, v2);
		const float v2t = Dot(tangent, v2);

		// use the elastic collision equation to get the new normal velocity for 
		// both physics components
		const float v1nFinal = (v1n * (m1 - m2) + 2 * m2 * v2n) / (m1 + m2);
		const float v2nFinal = (v2n * (m2 - m1) + 2 * m1 * v1n) / (m1 + m2);

		velocity = (v1nFinal * normal) + (v1t * tangent);
		object.velocity = (v2nFinal * normal) + (v2t * tangent);
	}
}

// Moves two physicsObjects apart by minTranslation but takes into account
// whether the world wraps around
void ActorBase::AdvancedMoveApart(ActorBase& ob, Vector2& minTranslation)
{
	// Work out which object is facing the other
	Vector2 wrappedDistance(0, 0);
	if(wrapAroundWorld)
	{
		GetShortestWrappedDistance(ob.GetPos(), world.pos, WORLD_WIDTH, WORLD_HEIGHT, wrappedDistance);
	}
	else
	{
		//wrappedDistance = ob.GetPos() - world.pos;
		wrappedDistance = world.pos - ob.GetPos();
	}
	if(Dot(minTranslation, wrappedDistance) < 0.0f)
	{
		minTranslation = -minTranslation;
	}
	world.pos += minTranslation / 2;
	ob.world.pos -= minTranslation / 2;
}

MathTypes::Circle ActorBase::GetCircle() const
{
	// remember that the circle should be scaled from the model size by the world
	return MathTypes::Circle(world.pos, model.GetModelRadius() * world.scale.Length());
}

void ActorBase::Render(const Camera& camera) const
{
	model.Render(world, camera);
}

void ActorBase::WrapPosition()
{
	if(wrapAroundWorld)
	{
		// Wraps position around the world if pos is greater than world size
		WrapVector2(WORLD_WIDTH, WORLD_HEIGHT, world.pos);
	}
}
