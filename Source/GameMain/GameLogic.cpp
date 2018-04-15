//-----------------------------------------------------------
// File: GameLogic.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "GameLogic.h"
#include "Game.h"
#include "Globals.h"

#include "../Content/ModelManager.h"
#include "../Content/Level.h"
#include "../Content/StandardLevel.h"
#include "../Content/MeteorShowerLevel.h"
#include "../Content/DebugLevel.h"
#include "../Utilities/Functions.h"
#include "../Graphics/Particle.h"

#include "../Content/AudioManager.h"

GameLogic::GameLogic(GraphicsDeviceManager& graphics) :
	game(nullptr),
	font(graphics)
{
	paused = false;
}

void GameLogic::Initialize(Game* _game, const Model2D& quadModel, const Model2D& playerModel, std::vector<Model2D> asteroidModels)
{
	// Store the game reference
	game = _game;

	// Create the camera array
	cameras.reserve(9);

	this->quadModel = quadModel;

	// Initialize the models for all asteroids
	this->asteroidModels = std::move(asteroidModels);

	// Initialize the life model
	lifeModel = playerModel;

	// Initialize the player
	player = Player(Vector2(), Vector2(2, 3), 0, playerModel, Vector2(0, 0), 5,
	                static_cast<int>(WORLD_WIDTH), static_cast<int>(WORLD_HEIGHT));

	// Initialize the quad tree
	quadtree.Initialize(nullptr, MathTypes::Rectangle(Vector2(-WORLD_WIDTH / 2, -WORLD_HEIGHT / 2),
	                                                  Vector2(WORLD_WIDTH, WORLD_HEIGHT)), 0);

	worldArea = MathTypes::Rectangle(Vector2(-WORLD_WIDTH - (WORLD_WIDTH / 2), -WORLD_HEIGHT - (WORLD_HEIGHT / 2)),
	                                 Vector2(WORLD_WIDTH * 3, WORLD_HEIGHT * 3));
}

// Resets the game and starts a level for the background of the main menu
void GameLogic::StartIdleMode()
{
	cameras.clear();
	asteroids.clear();
	quadtree.Reset();
	particleSystem.Reset();

	player.ResetWorld();

	int difficulty = 0;
	currentLevel.reset();
	currentLevel = std::make_shared<MeteorShowerLevel>(this, difficulty + 1);
	currentLevel->Initialize();
}

// Resets the game and starts a level for the controls screen
void GameLogic::StartPlayerOnlyLevel()
{
	cameras.clear();
	asteroids.clear();
	quadtree.Reset();
	particleSystem.Reset();

	player.ResetWorld();
	currentLevel.reset();
	currentLevel = std::make_shared<DebugLevel>(this, 0);
	currentLevel->Initialize();
}

void GameLogic::Update(const float dt)
{
	currentLevel->Update(dt);

	// If game is paused then don't update any of the game objects
	if(!paused)
	{
		// Update all the asteroids movements
		for(auto begin = asteroids.begin(); begin != asteroids.end(); ++begin)
		{
			if(!begin->GetAlive())
			{
				continue;
			}

			begin->Update(dt);

			// Make sure that the asteroids aren't outside the world area
			if(!worldArea.Contains(begin->GetCircle()))
			{
				begin->KillAsteroid();
			}

			// Loop through all the bullets in the array
			Bullet* bulletArray = player.GetBullets();
			for(int j = 0; j < MAX_BULLETS; j++)
			{
				if(bulletArray[j].IsAlive())
				{
					[[maybe_unused]] Vector2 ignoredOutput;
					if(bulletArray[j].CheckCollision(*begin, ignoredOutput))
					{
						// Remove the bullet from the array
						bulletArray[j].KillBullet();
						// Make the asteroid smaller or kill it
						begin->ReduceHealth();
						if(begin->GetAlive()) // if asteroid is still alive than it has split
						{
							// Create another small asteroid
							Asteroid newAsteroid = Asteroid(*begin);
							newAsteroid.SetModel(asteroidModels[RandomInt(0, asteroidModels.size())]);
							newAsteroid.Update(dt);
							begin->SetModel(asteroidModels[RandomInt(0, asteroidModels.size())]);
							begin->SplitAsteroids(newAsteroid);

							// recreate asteroid
							asteroids.push_back(newAsteroid);
							quadtree.AddPhysicsObject(asteroids.back());

							// Play explosion sound
							AudioManager::PlaySoundByName("Bang1");
						}
						else
						{
							currentLevel->AsteroidDestroyed();

							// Create the particle and add to the particle system
							std::vector<Particle> newParticles;
							AsteroidToParticles(*begin, newParticles);
							particleSystem.AddParticles(newParticles);

							// Play the second explosion sound
							AudioManager::PlaySoundByName("Bang2");
						}
					}
				}
			}
		}

		// Remove dead asteroids
		auto i = asteroids.begin();
		while(i != asteroids.end())
		{
			if(!i->GetAlive())
			{
				// Get rid of the asteroid
				RemoveAsteroid(i++);
			}
			else
			{
				++i;
			}
		}

		// update the quad tree to check the collisions of all asteroids
		quadtree.Update();

		int numPhys = quadtree.NumPhysicsObjects();

		// Loop all the asteroids to check the player collision
		if(player.Alive())
		{
			if(quadtree.ComputeIndividual(player))
			{
				// Play the sound of a collision
				AudioManager::PlaySoundByName("Thump");
				if(!player.GetInvulnerable())
				{
					// Handle the collision between player and asteroids e.g. lose health 
					player.RemoveLife();
					player.StartInvulnerability();
				}
			}
		}

		player.Update(dt);

		particleSystem.Update(dt);
	}
}

void GameLogic::Render(bool showLives, bool showLevelNum)
{
	// Draw the quad tree for debug purposes
#ifdef PHYSICS_DEBUG_INFO
	quadtree.DrawQuads(cameras[0], quadModel);
#endif

	particleSystem.Render(cameras[0]);

	// Loop each camera in camera array
	for(auto& camera : cameras)
	{
		for(auto& asteroid : asteroids)
		{
			// Check that the asteroids is in the camera
			if(camera.GetScreenRect().Intersects(asteroid.GetCircle()))
			{
				// Render the object
				asteroid.Render(camera);
			}
		}
	}

	// Render the player in each camera (if appliciple)
	if(player.GetLives() > 0)
	{
		for(auto& camera : cameras)
		{
			if(camera.GetScreenRect().Intersects(player.GetCircle()))
			{
				player.Render(camera);
			}
		}
	}

	// Render all of the player bullets
	for(auto& camera : cameras)
	{
		Bullet* bulletList = player.GetBullets();
		for(int j = 0; j < MAX_BULLETS; ++j)
		{
			if(bulletList[j].IsAlive())
			{
				if(camera.GetScreenRect().Intersects(bulletList[j].GetCircle()))
				{
					bulletList[j].Render(camera);
				}
			}
		}
	}

	// Draw the player lives
	if(showLives)
	{
		for(int i = 0; i < player.GetLives(); i++)
		{
			Vector2 pos(15 + (static_cast<float>(i) * 20), 18);
			lifeModel.Render(pos, Vector2(6, 6), PI);
		}
	}

	// Draw the level number to the screen
	if(showLevelNum)
	{
		std::string levelStr;
		to_String(currentLevel->GetLevelNumber(), 1, levelStr);
		Vector2 size = font.GetTextSize("Level: " + levelStr);
		Vector2 pos((WINDOW_WIDTH / 2) - (size.x / 2), 7);
		font.DrawString("Level: " + levelStr, pos);
	}

	currentLevel->Render();
}

void GameLogic::Reset()
{
	// Clear the old asteroids
	asteroids.clear();
	quadtree.Reset();
	cameras.clear();
	player.Reset();
	particleSystem.Reset();

	// Create the first level
	currentLevel = std::make_shared<StandardLevel>(this, 1);
	currentLevel->Initialize();

	player.StartInvulnerability();
}

void GameLogic::AddAsteroids(std::list<Asteroid>& n)
{
	// try adding the asteroids one at a time
	for(int i = 0, size = n.size(); i < size; i++)
	{
		quadtree.AddPhysicsObject(*n.begin());
		asteroids.splice(asteroids.end(), n, n.begin());
	}
}

void GameLogic::RemoveAllAsteroids()
{
	asteroids.clear();
}

int GameLogic::NumAsteroids()
{
	return asteroids.size();
}

void GameLogic::RemoveAsteroid(int id)
{
	// search the asteroids to find a matching id
	for(auto begin = asteroids.begin(), end = asteroids.end(); begin != end; ++begin)
	{
		if(begin->GetID() == id)
		{
			quadtree.RemovePhysicsObject(begin->GetID());
			asteroids.erase(begin++);
			break;
		}
	}
}

void GameLogic::RemoveAsteroid(std::list<Asteroid>::iterator i)
{
	quadtree.RemovePhysicsObject(i->GetID());
	asteroids.erase(i);
}

void GameLogic::RemoveAsteroid(const Asteroid& asteroid)
{
	int id = asteroid.GetID();
	RemoveAsteroid(id);
}

void GameLogic::AddAsteroidsToQuadTree()
{
	// Add all the asteroids to the quadtree
	for(auto& asteroid : asteroids)
	{
		quadtree.AddPhysicsObject(asteroid);
	}
}

bool GameLogic::LevelComplete() const
{
	return currentLevel->Complete();
}

void GameLogic::IncrementLevel()
{
	asteroids.clear();
	quadtree.Reset();
	particleSystem.Reset();

	player.ResetWorld();

	// Get the difficulty of the current level
	int level = currentLevel->GetLevelNumber();
	currentLevel.reset();
	// Create a new level with an incremented difficulty
	currentLevel = std::static_pointer_cast<Level>(std::make_shared<StandardLevel>(this, level + 1));
	currentLevel->Initialize();

	player.StartInvulnerability();

	Update(0.01f);
	Update(0.01f);
}
