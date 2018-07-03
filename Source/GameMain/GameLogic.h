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
#include "../Content/Content.h"

class Game;
class Level;
class Content;

class GameLogic
{
private:
	// Pointer to the parent game
	Game* game;

	Content& content;

	// Game objects
	std::vector<Camera> cameras;
	std::vector<std::shared_ptr<Asteroid>> asteroids;

	std::shared_ptr<Level> currentLevel;

	Quadtree quadtree;

	ParticleSystem particleSystem;

	// Player
	Player player;
	Model2D lifeModel;

	// Complete world area
	MathTypes::Rectangle worldArea;

	// Asteroid models in a single array to randomly choose
	std::vector<Model2D> asteroidModels;

	// is game paused
	bool paused;

	// Font for writing the level number
	Font* font;

	// Quad model for debugging
	Model2D quadModel;

public:
	GameLogic(Content& content);

	void Initialize(Game* _game);

	void Update(float dt);
	void Render(bool showLives = true, bool showLevelNum = true);

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

	const std::vector<Model2D>& GetAsteroidModels() const { return asteroidModels; }

	// Functions to add and remove asteroids from the list. Used for levels
	void AddAsteroids(const std::vector<std::shared_ptr<Asteroid>>& asteroids);
	void RemoveAllAsteroids();
	int NumAsteroids() const;

	void AddCameras(std::vector<Camera> cameras);

private:

	void RemoveDeadAsteroids();

	void AddAsteroidsToQuadTree();
};


#endif
