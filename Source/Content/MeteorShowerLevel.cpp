//-----------------------------------------------------------
// File: MeteorShowerLevel.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "MeteorShowerLevel.h"

#include "..\GameMain\GameLogic.h"
#include "..\GameMain\Globals.h"
#include "..\Actor\Asteroid.h"

const float MeteorShowerLevel::MAX_TIME_TILL_ASTEROIDS = 3.0f;
const float MeteorShowerLevel::MIN_TIME_TILL_ASTEROIDS = 1.5f;

const int MeteorShowerLevel::MAX_ASTEROIDS_PER_COUNTDOWN = 3;
const int MeteorShowerLevel::MIN_ASTEROIDS_PER_COUNTDOWN = 1;

MeteorShowerLevel::MeteorShowerLevel( GameLogic* parent, int level ) :
	Level( parent, level )
{
	timeTillAsteroids = MIN_TIME_TILL_ASTEROIDS + Random() * 
		(MAX_TIME_TILL_ASTEROIDS - MIN_TIME_TILL_ASTEROIDS);
	asteroidTimer.Start();
}

// Override of the initialize to set up everything in the level
void MeteorShowerLevel::Initialize()
{
	// Create the camera
	std::vector<Camera>& cameras = parent->GetCameras();
	cameras.push_back( Camera() );
	cameras[0].Initialize( Vector2( 0, 0 ), Vector2( WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT );		 // Center-Center

	int numAsteroids = MIN_ASTEROIDS_PER_COUNTDOWN + Random() * 
		(MAX_ASTEROIDS_PER_COUNTDOWN - MIN_ASTEROIDS_PER_COUNTDOWN) * 4;

	// Create the asteroids
	list<Asteroid> asteroids;
	for( int i = 0; i < numAsteroids; i++ )
	{
		Asteroid asteroid;
		CreateAsteroid( asteroid );
		asteroids.push_back( asteroid );
	}

	parent->AddAsteroids( asteroids );

	// Setup the player
	parent->GetPlayer().Reset();
	parent->GetPlayer().SetWrapAround( false );
	parent->GetPlayer().KillPlayer();
}

// Level runs until externaly cancelled
bool MeteorShowerLevel::Complete()
{
	return false;
}

// Update the timer. Check the number of asteroids on screen and add more to come on
// if there aren't enough
void MeteorShowerLevel::Update( float dt )
{
	asteroidTimer.Tick( dt );

	// Check the amount of asteroids in the scene and the amount on screen.
	if( asteroidTimer.GetDeltaTime() >= timeTillAsteroids )
	{
		// Add some more asteroids
		int numAsteroids = MIN_ASTEROIDS_PER_COUNTDOWN + Random() * 
			(MAX_ASTEROIDS_PER_COUNTDOWN - MIN_ASTEROIDS_PER_COUNTDOWN);

		list<Asteroid> newAsteroids;
		for( int i = 0; i < numAsteroids; i++ )
		{
			Asteroid asteroid;
			CreateAsteroid( asteroid );
			newAsteroids.push_back( asteroid );
		}

		parent->AddAsteroids( newAsteroids );

		asteroidTimer.Stop();
		asteroidTimer.Start();
		timeTillAsteroids = MIN_TIME_TILL_ASTEROIDS + Random() * (MAX_TIME_TILL_ASTEROIDS - MIN_TIME_TILL_ASTEROIDS);
	}
}

void MeteorShowerLevel::CreateAsteroid( Asteroid& asteroid )
{
	Vector2 pos = Vector2( 
				Random( (WORLD_WIDTH/2) + 30 , WORLD_WIDTH + WORLD_WIDTH/2), 
				Random( -WORLD_HEIGHT/2, WORLD_HEIGHT/2 ) );
	float rot = Random( -PI, PI );
	// Create the health then base the size, mass and velocity on health
	float random = Random();
	int health;
	if( random <= 0.1 ) health = 1;
	else if ( random > 0.1 && random <= 0.3) health = 2;
	else if ( random > 0.3 && random <= 0.96) health = 3;
	else if ( random > 0.96 && difficulty > 1 ) health = 5;
	else health = 3;

	// Create the size
	float sizeScale = (float)health / 5.0f; // value to make calculating other values easier
	if( health == 5 ) sizeScale *= 2;
	float size = sizeScale * (Asteroid::MAX_SIZE - Asteroid::MIN_SIZE) + Asteroid::MIN_SIZE;
	int mass = sizeScale * (Asteroid::MAX_MASS - Asteroid::MIN_MASS) + Asteroid::MIN_MASS;

	float maxVelY = WORLD_HEIGHT/2 - 20;
	float minVelY = -WORLD_WIDTH / 2 + 20;
	Vector2 vel = Vector2( -WORLD_WIDTH/2, minVelY + Random() * (maxVelY - minVelY) );
	vel -= pos;
	Normalize( vel );
	vel *= Random(2, 20);

	// Pick random model
	auto models = parent->GetAsteroidModels();
	Model2D* model = models[ RandomInt( 0, models.size() ) ];
	asteroid = Asteroid( pos, size, rot, model, vel, mass, health );
	asteroid.SetWrapAround( false );
}