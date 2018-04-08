//-----------------------------------------------------------
// File: StandardLevel.h
// Description: Inherits level and represents a level with wrap
//				around world (meaning full camera array), player and
//				amounts of asteroids increasing with difficulty. 
//				Level is completed when all asteroids are destroyed
//-----------------------------------------------------------

#ifndef STANDARD_LEVEL_H
#define STANDARD_LEVEL_H

#include "Level.h"

class StandardLevel : public Level
{
public:
	StandardLevel( GameLogic* parent, int levelNumber );

	void Initialize();
	bool Complete();
};

#endif