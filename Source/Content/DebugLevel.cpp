//-----------------------------------------------------------
// File: DebugLevel.cpp
//-----------------------------------------------------------

#include "DebugLevel.h"

#include "../GameMain/Globals.h"
#include "../GameMain/GameLogic.h"
#include "../Actor/Player.h"

DebugLevel::DebugLevel( GameLogic* _parent, int difficulty )
	: Level( _parent, difficulty ) 
{
}

void DebugLevel::Initialize()
{
	// Initialize the camera array
	std::vector<Camera>& cameras = parent->GetCameras();
	cameras.reserve(9);

	cameras.emplace_back( Vector2( 0.0f, 0.0f ), Vector2(WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT );		 // Center-Center
	cameras.emplace_back( Vector2( -WORLD_WIDTH, 0.0f ), Vector2(WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT );	 // Left-Center
	cameras.emplace_back( Vector2(WORLD_WIDTH, 0.0f ), Vector2(WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT );	 // Right-Center
	cameras.emplace_back( Vector2( -WORLD_WIDTH, WORLD_HEIGHT), Vector2(WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT ); // Left-Top
	cameras.emplace_back( Vector2( 0.0f, WORLD_HEIGHT), Vector2(WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT );    // Center-Top
	cameras.emplace_back( Vector2(WORLD_WIDTH, WORLD_HEIGHT), Vector2(WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT );  // Right-Top
	cameras.emplace_back( Vector2( -WORLD_WIDTH, -WORLD_HEIGHT), Vector2(WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT );// Left-Bottom
	cameras.emplace_back( Vector2( 0.0f, -WORLD_HEIGHT), Vector2(WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT );	 // Center-Bottom
	cameras.emplace_back( Vector2(WORLD_WIDTH, -WORLD_HEIGHT), Vector2(WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT ); // Right-Bottom

	parent->GetPlayer().Reset();
	parent->GetPlayer().SetWrapAround(true);
}

bool DebugLevel::Complete()
{
	// Debug level cannot be completed and must be manually canceled
	return false;
}