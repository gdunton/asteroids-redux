//-----------------------------------------------------------
// File: GameEntity.cpp
//-----------------------------------------------------------

#include "GameEntity.h"

#include "../GameMain/Globals.h"

void GameEntity::Render( Camera& camera ) const
{
	model.Render( camera, world );
}

void GameEntity::WrapPosition()
{
	if( wrapAroundWorld )
	{
		// Wraps position around the world if pos is greater than world size
		WrapVector2( WORLD_WIDTH, WORLD_HEIGHT, world.pos );
	}
}