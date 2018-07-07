//-----------------------------------------------------------
// File: DebugLevel.cpp
//-----------------------------------------------------------

#include "DebugLevel.h"

#include "../GameMain/GameLogic.h"
#include "../Actor/Player.h"

DebugLevel::DebugLevel(GameLogic* parent, int difficulty)
	: Level(parent, difficulty)
{
	parent->SetCameras(CreateCameraArray());

	parent->GetPlayer().Reset();
	parent->GetPlayer().SetWrapAround(true);
}

bool DebugLevel::Complete()
{
	// Debug level cannot be completed and must be manually canceled
	return false;
}
