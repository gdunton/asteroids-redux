//-----------------------------------------------------------
// File: Level.h
// Description: Level abstract class is intented to be used by the GameLogic
//				to convienently initialize things needed for each
//				level type such as cameras and asteroids
//-----------------------------------------------------------

#ifndef LEVEL_H
#define LEVEL_H

#include "../Utilities/Functions.h"

class GameLogic;

class Level
{
protected:
	GameLogic* parent;
	int levelNumber;

	// Enum for difficulty e.g. Easy, Normal etc
	Difficulty difficulty;

	static std::vector<Camera> CreateCameraArray();

public:
	Level(GameLogic* _parent, int _levelNumber) :
		parent(_parent), levelNumber(_levelNumber), difficulty(LevelToDifficulty(_levelNumber))
	{ }

	virtual ~Level() = default;

	// Called per frame to check when the level is completed
	virtual bool Complete() = 0;

	virtual void AsteroidDestroyed() {}

	// Called each frame
	virtual void Update(float /*dt*/) {}
	virtual void Render() {}

	int GetDifficulty() const { return difficulty; }
	int GetLevelNumber() const { return levelNumber; }
};

#endif
