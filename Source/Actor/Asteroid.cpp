//-----------------------------------------------------------
// File: Asteroid.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "Asteroid.h"

#include "..\Graphics\Camera.h"
#include "..\Graphics\Model2D.h"
#include "..\Content\ModelManager.h"

#include "..\GameMain\Globals.h"

// Constant definitions
const float Asteroid::MAX_SIZE = 6.0f;
const float Asteroid::MIN_SIZE = 0.25f;

const int Asteroid::MAX_MASS = 200;
const int Asteroid::MIN_MASS = 40;


Asteroid::Asteroid()
{
	alive = false;
}

Asteroid::Asteroid( const MyVector2& pos, const float& size, float rot,
		Model2D* model, const MyVector2& velocity, float mass, int _health )
		: PhysicsObject( pos, MyVector2(size,size), rot, model, velocity, mass ), 
		health( _health )
{
	alive = true;
}

void Asteroid::Update( float dt )
{
	// Update the position
	world.pos += velocity * dt;

	// Check that the position doesn't wrap around the screen
	WrapPosition();	
}

void Asteroid::Render( Camera& camera ) const
{
	if( alive )
	{
        // Render the object to screen
		GameEntity::Render( camera );
        
#ifdef PHYSICS_DEBUG_INFO
		MyVector2 screenPos = camera.ConvertWorldToScreenPos( world.pos );

        // Render the bounding circle of the asteroid to the screen
		MathTypes::Circle c = GetCircle();
        ModelManager::GetModel("Circle")->SetColor( BLUE );
		ModelManager::GetModel("Circle")->Render( camera, World( c.position, MyVector2(c.radius, c.radius), 0 ) );
#endif
	}
}

void Asteroid::ReduceHealth()
{
	--health;

    // If the asteroid is still alive then shrink it and remove mass
	if( health > 0 )
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

void Asteroid::SplitAsteroids( Asteroid& asteroid )
{
	// make sure that both asteroids are alive
	if( !alive || !asteroid.GetAlive() ) return;

	// Check that the asteroids don't have the same id
	if( id == asteroid.GetID() )
	{
		asteroid.ResetID();
	}

	// Set the velocity to push the objects apart
	MyVector2 res(0,0);
	if( CheckCollision( asteroid, res ) )
	{
		AdvancedMoveApart( asteroid, res );
		PerformCollisionCalculation( asteroid );

		MyVector2 tangent = MyVector2( -velocity.y, velocity.x );

		// Set the velocity to push the objects apart
		velocity += tangent * 0.5f;
		asteroid.velocity += -tangent * 0.5f;
	}
}