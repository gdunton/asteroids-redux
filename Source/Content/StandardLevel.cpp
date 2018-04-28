//-----------------------------------------------------------
// File: StandardLevel.cpp
//-----------------------------------------------------------

#include "StandardLevel.h"

#include "../GameMain/GameLogic.h"
#include "../GameMain/Globals.h"

#include "../Actor/Asteroid.h"
#include "../Utilities/Random.h"

StandardLevel::StandardLevel( GameLogic* parent, int levelNumber )
	: Level( parent, levelNumber )
{
}

void StandardLevel::Initialize()
{
	// Initialize the camera array
	std::vector<Camera>& cameras = parent->GetCameras();
	cameras.reserve(9);

	const auto width = static_cast<float>(WORLD_WIDTH);
	const auto height = static_cast<float>(WORLD_HEIGHT);

	cameras.emplace_back(Vector2( 0, 0 ), Vector2(width*0.99f, height*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT );		 // Center-Center
	cameras.emplace_back(Vector2( -width, 0 ), Vector2(width*0.99f, height*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT );	 // Left-Center
	cameras.emplace_back(Vector2(width, 0 ), Vector2(width*0.99f, height*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT );	 // Right-Center
	cameras.emplace_back(Vector2( -width, height), Vector2(width*0.99f, height*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT ); // Left-Top
	cameras.emplace_back(Vector2( 0, height), Vector2(width*0.99f, height*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT );    // Center-Top
	cameras.emplace_back(Vector2(width, height), Vector2(width*0.99f, height*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT );  // Right-Top
	cameras.emplace_back(Vector2( -width, -height), Vector2(width*0.99f, height*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT );// Left-Bottom
	cameras.emplace_back(Vector2( 0, -height), Vector2(width*0.99f, height*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT );	 // Center-Bottom
	cameras.emplace_back(Vector2(width, -height), Vector2(width*0.99f, height*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT ); // Right-Bottom

	
	int numAsteroids = 0;
	switch( difficulty )
	{
	case EASY:
		numAsteroids = 5;
		break;
	case NORMAL:
		numAsteroids = 10;
		break;
	case HARD:
		numAsteroids = 17;
		break;
	case EXTREME:
		numAsteroids = 25;
		break;

	default:
		numAsteroids = 10;
	}

	std::vector<std::shared_ptr<Asteroid>> asteroids;

	// Set up all the asteroids in the parent
	// Initialize all of the asteroids 
	for( int i = 0; i < numAsteroids; ++i )
	{
		Vector2 pos = Vector2( Random( -100, 100 ), Random( -100, 100 ) );
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
		float sizeScale = static_cast<float>(health) / 5.0f; // value to make calculating other values easier
		sizeScale += Random(-1, 1) / 10.0f; // introduce variation into the scale
		if( health == 5 ) sizeScale *= 3; // special case massive asteroids
		float size = sizeScale * (Asteroid::MAX_SIZE - Asteroid::MIN_SIZE) + Asteroid::MIN_SIZE;
		float mass = sizeScale * (Asteroid::MAX_MASS - Asteroid::MIN_MASS) + Asteroid::MIN_MASS;

		Vector2 vel = Vector2( Random( -10, 10 ), Random( -10, 10 ) ); 
		vel *= (2-(sizeScale/2)); // make smaller asteroids faster

		// Pick random model
		auto models = parent->GetAsteroidModels();
		Model2D model = models[ RandomInt( 0, models.size() ) ];
		asteroids.emplace_back(std::make_unique<Asteroid>(pos, size, rot, std::move(model), vel, mass, health));
	}

	parent->AddAsteroids( asteroids );

	// Set up the player in the parent
	parent->GetPlayer().ResetWorld();

	parent->GetPlayer().SetWrapAround(true);
}

bool StandardLevel::Complete()
{
	// There are no asteroids left
	return (parent->NumAsteroids() == 0);
}