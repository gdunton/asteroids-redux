//-----------------------------------------------------------
// File: DebugLevel.h
// Description: Inherits Level. Initializes with just a player
//				and standard camera array. Cannot be completed
//				and must be canceled.
//-----------------------------------------------------------

#ifndef DEBUGLEVEL_H
#define DEBUGLEVEL_H

#include "Level.h"

class DebugLevel : public Level
{
public:
	DebugLevel( GameLogic* parent, int difficulty );

	void Initialize();
	bool Complete();
};

#endif