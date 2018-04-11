//-----------------------------------------------------------
// File: GameLogic.h
// Description: The class containing all the code for the actual
//				running of the game. Manages the asteroids and 
//				the player as well as the levels. Uses a quadtree
//				to optimise the collisions. 
//-----------------------------------------------------------

#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "../Actor/Asteroid.h"
#include "../Actor/Player.h"
#include "../Actor/Bullet.h"
#include "../Graphics/Camera.h"
#include "../Actor/Quadtree.h"
#include "../Graphics/ParticleSystem.h"
#include "../Graphics/Font.h"

class Game;
class Level;

class GameLogic 
{
private:
	// Pointer to the parent game
	Game* game;

	// Game objects
	std::vector<Camera> cameras; 
	std::vector<Asteroid> asteroids;

	std::shared_ptr<Level> currentLevel;

	Quadtree quadtree;

	ParticleSystem particleSystem;

	// Player
	Player player;
	Model2D* lifeModel;

	// Complete world area
	MathTypes::Rectangle worldArea;

	// Asteroid models in a single array to randomly choose
	std::vector<Model2D*> asteroidModels;

	// is game paused
	bool paused;

	// Font for writing the level number
	Font font;

public:
	GameLogic();
	~GameLogic();

	void Initialize(Game* _game);

	void Update( const float dt );
	void Render( bool showLives = true, bool showLevelNum = true );

	// Sets all ojects to starting values
	void Reset();

	// Level based functions for classes to be able to control level progression
	bool LevelComplete() const;
	void IncrementLevel();

	// Set the game into idle mode. Used for main menu
	void StartIdleMode();
	// Create player only level. Used for controls screen
	void StartPlayerOnlyLevel();

	Player& GetPlayer() { return player; }
	Game& GetParent() { return *game; } 

	std::vector<Model2D*>& GetAsteroidModels() { return asteroidModels; }

	// Functions to add and remove asteroids from the list. Used for levels
	void AddAsteroids(std::vector<Asteroid>& asteroids);
	void RemoveAllAsteroids();
	int NumAsteroids();

	std::vector<Camera>& GetCameras() { return cameras; }

private:
	
	// Functions for removing asteroids by id or index
	void RemoveAsteroid( int ID );
	void RemoveAsteroid( std::vector<Asteroid>::iterator i );
	void RemoveAsteroid( const Asteroid& asteroid );

	void AddAsteroidsToQuadTree();
};


#endif