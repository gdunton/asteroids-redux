//-----------------------------------------------------------
// File: ControlScreenState.h
// Description: Inherits GameState. Forces the gamelogic to start
//				a level with just the player. Collects input for the 
//				player, displayes the player but greyed out with
//				controls over the top.
//-----------------------------------------------------------

#ifndef CONTROL_SCREEN_STATE_H
#define CONTROL_SCREEN_STATE_H

#include "GameState.h"
#include "../Graphics/Font.h"
#include "../Graphics/Sprite.h"
#include "../Input/KeyboardState.h"

class ControlScreenState : public GameState
{
private:
	// State title font
	Font titleFont;
	// Font for any other text
	Font subFont1;

	// Strings containing descriptions and corrisponding controls
	static String const controlStrings[];

	Sprite background;

	KeyboardState prevKbState;

public:
	ControlScreenState( GameStateManager* parent );
	~ControlScreenState();

	void Enter();
	void Update( float dt );
	void Exit();

	void Render();
};

#endif