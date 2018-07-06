//-----------------------------------------------------------
// File: DebugLevel.cpp
//-----------------------------------------------------------

#include "DebugLevel.h"

#include "../GameMain/Globals.h"
#include "../GameMain/GameLogic.h"
#include "../Actor/Player.h"

DebugLevel::DebugLevel(GameLogic* _parent, int difficulty)
	: Level(_parent, difficulty)
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
