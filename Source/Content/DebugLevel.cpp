//-----------------------------------------------------------
// File: DebugLevel.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "DebugLevel.h"

#include "..\GameMain\Globals.h"
#include "..\GameMain\GameLogic.h"
#include "..\Actor\Player.h"

DebugLevel::DebugLevel( GameLogic* _parent, int difficulty )
	: Level( _parent, difficulty ) 
{
}

void DebugLevel::Initialize()
{
	// Initialize the camera array
	std::vector<Camera>& cameras = parent->GetCameras();
	for( int i = 0; i < 9; i++ )
	{
		cameras.push_back( Camera() );
	}
	cameras[0].Initialize( Vector2( 0, 0 ), Vector2( WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT );		 // Center-Center
	cameras[1].Initialize( Vector2( -WORLD_WIDTH, 0 ), Vector2( WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT );	 // Left-Center
	cameras[2].Initialize( Vector2( WORLD_WIDTH, 0 ), Vector2( WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT );	 // Right-Center
	cameras[3].Initialize( Vector2( -WORLD_WIDTH, WORLD_HEIGHT ), Vector2( WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT ); // Left-Top
	cameras[4].Initialize( Vector2( 0, WORLD_HEIGHT ), Vector2( WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT );    // Center-Top
	cameras[5].Initialize( Vector2( WORLD_WIDTH, WORLD_HEIGHT ), Vector2( WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT );  // Right-Top
	cameras[6].Initialize( Vector2( -WORLD_WIDTH, -WORLD_HEIGHT ), Vector2( WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT );// Left-Bottom
	cameras[7].Initialize( Vector2( 0, -WORLD_HEIGHT ), Vector2( WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT );	 // Center-Bottom
	cameras[8].Initialize( Vector2( WORLD_WIDTH, -WORLD_HEIGHT ), Vector2( WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f ), WINDOW_WIDTH, WINDOW_HEIGHT ); // Right-Bottom

	parent->GetPlayer().Reset();
}

bool DebugLevel::Complete()
{
	// Debug level cannot be completed and must be manually canceled
	return false;
}