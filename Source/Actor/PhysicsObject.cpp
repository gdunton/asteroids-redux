//-----------------------------------------------------------
// File: PhysicsObject.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "PhysicsObject.h"
#include "..\GameMain\Globals.h"

PhysicsObject::PhysicsObject( const MyVector2& _pos, const MyVector2& _size, 
	float _rot, Model2D* _model, const MyVector2& _velocity, 
	float _mass )
		: GameEntity( _pos, _size, _rot, _model )
{
	velocity = _velocity;
	mass = _mass;

	// is model set by now?
	collision.Initialize( model->GetPoints() );
}

bool PhysicsObject::CompleteCollisionCompute( PhysicsObject& object )
{
	MyVector2 res(0, 0);
	if( CheckCollision( object, res ) ) // if collision occures
	{
		// Move the objects apart and then perform the elastic
		// collision calculations
		AdvancedMoveApart( object, res );
		PerformCollisionCalculation( object );

		return true;
	}
	return false;
}

bool PhysicsObject::CheckCollision( PhysicsObject& object, MyVector2& minTranslation )
{
	// Get the distance between the two objects
	MyVector2 direction;
	if( wrapAroundWorld )
	{
		GetShortestWrappedDistance( object.GetPos(), world.pos, WORLD_WIDTH, WORLD_HEIGHT, direction );
	}
	else
	{
		direction = world.pos - object.GetPos();
	}

	float r1 = GetCircle().radius;
	float r2 = object.GetCircle().radius;

	// if length is less than the collision radii
	if( LengthSquared( direction ) < ((r1+r2) * (r1+r2)) )
	{
		// Set the world value on both collision shapes
		collision.SetWorld( world );
		object.collision.SetWorld( object.world );

		// Do deeper collision check using the collision geometry
		return collision.CheckCollision( object.collision, minTranslation );
	}
	else
	{
		return false;
	}
}

void PhysicsObject::PerformCollisionCalculation( PhysicsObject& object )
{
	// Check that the objects are in each other
	MyVector2 direction;
	MyVector2 normal;
	MyVector2 tangent;

	// Get the distance between the two objects
	if( wrapAroundWorld )
	{
		GetShortestWrappedDistance( world.pos, object.GetPos(), 
			WORLD_WIDTH, WORLD_HEIGHT, direction );
	}
	else
	{
		direction = world.pos - object.GetPos();
	}

	// Get normalized distance and tangents between two objects
	normal = direction;
	Normalize( normal );
	tangent = MyVector2( -normal.y, normal.x );

	float r1 = GetCircle().radius;
	float r2 = object.GetCircle().radius;
	
	{ // Change both velocities from the collision
		// Get both masses
		float m1 = mass;
		float m2 = object.GetMass();

		// Get both the velocities
		MyVector2 v1 = velocity;
		MyVector2 v2 = object.GetVelocity();

		// Create the normal and tangent velocities using the dot product
		float v1n = Dot( normal, v1 );
		float v1t = Dot( tangent, v1 );
		float v2n = Dot( normal, v2 );
		float v2t = Dot( tangent, v2 );

		// use the elastic collision equation to get the new normal velocity for 
		// both physics components
		float v1nFinal = (v1n * (m1 - m2) + 2 * m2 * v2n) / (m1 + m2);
		float v2nFinal = (v2n * (m2 - m1) + 2 * m1 * v1n) / (m1 + m2);

		velocity = (v1nFinal * normal) + (v1t * tangent);
		object.velocity = (v2nFinal * normal) + (v2t * tangent);
	}
}

// Moves two physicsObjects apart by minTranslation but takes into account
// whether the world wraps around
void PhysicsObject::AdvancedMoveApart( PhysicsObject& ob, MyVector2& res )
{
	// Work out which object is facing the other
	MyVector2 wrappedDistance(0,0);
	if( wrapAroundWorld )
	{
		GetShortestWrappedDistance( ob.GetPos(), world.pos, WORLD_WIDTH, WORLD_HEIGHT, wrappedDistance );
	}
	else
	{
		//wrappedDistance = ob.GetPos() - world.pos;
		wrappedDistance = world.pos - ob.GetPos();
	}
	if( Dot( res, wrappedDistance) < 0.0f )
	{
		res = -res;
	}
	world.pos += res / 2;
	ob.world.pos -= res / 2;
}

MathTypes::Circle PhysicsObject::GetCircle() const
{
	// remember that the circle should be scaled from the model size by the world
	return MathTypes::Circle( world.pos, model->GetModelRadius() * Length( world.scale ) );
}