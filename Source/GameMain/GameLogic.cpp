//-----------------------------------------------------------
// File: GameLogic.cpp
//-----------------------------------------------------------

#include "GameLogic.h"
#include "Game.h"
#include "Globals.h"
#include <algorithm>

#include "../Content/ModelManager.h"
#include "../Content/Level.h"
#include "../Content/StandardLevel.h"
#include "../Content/MeteorShowerLevel.h"
#include "../Content/DebugLevel.h"
#include "../Utilities/Functions.h"
#include "../Graphics/Particle.h"

#include "../Content/AudioManager.h"
#include "../Debugging/Error.h"
#include "../Utilities/Random.h"

GameLogic::GameLogic(Content& content) :
	game(nullptr),
	content(content),
	font(nullptr)
{
	paused = false;
	font = content.DefaultFont();

	ASSERT(font != nullptr);
}

void GameLogic::Initialize(Game* _game)
{
	// Store the game reference
	game = _game;

	// Create the camera array
	cameras.reserve(9);

	this->quadModel = content.Models().CreateQuadModel();

	// Initialize the models for all asteroids
	asteroidModels = {
		content.Models().CreateAsteroidModel1(), 
		content.Models().CreateAsteroidModel2(),
		content.Models().CreateAsteroidModel3()
	};

	// Initialize the life model
	lifeModel = content.Models().CreatePlayerModel();

	// Initialize the player
	player = Player(Vector2(), Vector2(2, 3), 0, content.Models().CreatePlayerModel(), Vector2(0, 0), 5, &content.Audio());

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

	const int difficulty = 0;
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

	if(paused) return;

	std::vector<std::shared_ptr<Asteroid>> newAsteroids;

	// Update all the asteroids movements
	for(auto& asteroid : asteroids)
	{
		if(!asteroid->GetAlive())
		{
			continue;
		}

		asteroid->Update(dt);

		// Make sure that the asteroids aren't outside the world area
		if(!worldArea.Contains(asteroid->GetCircle()))
		{
			asteroid->KillAsteroid();
		}

		// Loop through all the bullets in the array
		Bullet* bulletArray = player.GetBullets();
		for(int j = 0; j < MAX_BULLETS; j++)
		{
			if(bulletArray[j].IsAlive())
			{
				if(bulletArray[j].CheckCollision(*asteroid))
				{
					// Remove the bullet from the array
					bulletArray[j].KillBullet();
					// Make the asteroid smaller or kill it
					asteroid->ReduceHealth();
					if(asteroid->GetAlive()) // if asteroid is still alive than it has split
					{
						// Create another small asteroid
						auto newAsteroid = std::make_shared<Asteroid>(*asteroid);
						newAsteroid->SetModel(asteroidModels[RandomInt(0, asteroidModels.size())]);
						newAsteroid->Update(dt);
						asteroid->SetModel(asteroidModels[RandomInt(0, asteroidModels.size())]);
						asteroid->SplitAsteroids(*newAsteroid);

						newAsteroids.push_back(newAsteroid);

						// Play explosion sound
						content.Audio().PlaySoundByName("Bang1");
					}
					else
					{
						currentLevel->AsteroidDestroyed();

						// Create the particle and add to the particle system
						std::vector<Particle> newParticles = AsteroidToParticles(*asteroid);
						particleSystem.AddParticles(newParticles);

						// Play the second explosion sound
						content.Audio().PlaySoundByName("Bang2");
					}
				}
			}
		}
	}

	// Add any new asteroids
	asteroids.insert(asteroids.end(), newAsteroids.begin(), newAsteroids.end());
	std::for_each(newAsteroids.begin(), newAsteroids.end(), 
		[this](const auto& asteroid) { quadtree.AddPhysicsObject(asteroid); });

	// update the quad tree to check the collisions of all asteroids
	quadtree.Update();

	int numPhys = quadtree.NumPhysicsObjects();

	// Loop all the asteroids to check the player collision
	if(player.Alive())
	{
		if(quadtree.ComputeIndividual(player))
		{
			// Play the sound of a collision
			content.Audio().PlaySoundByName("Thump");
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

	RemoveDeadAsteroids();
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
			if(camera.GetScreenRect().Intersects(asteroid->GetCircle()))
			{
				// Render the object
				asteroid->Render(camera);
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
		const std::string levelStr = std::to_string(currentLevel->GetLevelNumber());
		const Vector2 size = font->GetTextSize("Level: " + levelStr);
		const Vector2 pos((WINDOW_WIDTH / 2) - (size.x / 2), 7);
		font->DrawString("Level: " + levelStr, pos);
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

void GameLogic::AddAsteroids(const std::vector<std::shared_ptr<Asteroid>>& newAsteroids)
{
	for (const auto& asteroid : newAsteroids)
	{
		quadtree.AddPhysicsObject(asteroid);
	}

	asteroids.insert(asteroids.begin(), newAsteroids.begin(), newAsteroids.end());
}

void GameLogic::RemoveAllAsteroids()
{
	asteroids.clear();
}

int GameLogic::NumAsteroids() const
{
	return asteroids.size();
}

void GameLogic::RemoveDeadAsteroids()
{
	for(const auto& asteroid : asteroids)
	{
		if (!asteroid->GetAlive())
		{
 			quadtree.RemovePhysicsObject(asteroid);
		}
	}

	asteroids.erase(
		std::remove_if(asteroids.begin(), asteroids.end(), [](auto asteroid) { return !asteroid->GetAlive(); }), 
		asteroids.end());
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
