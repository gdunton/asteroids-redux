//-----------------------------------------------------------
// File: GamePausedState.h
// Description: Inherits GameState. Shows a menu over the top of
//				the game which is not updating but is darkened.
//				Menu options allow returning to the game or 
//				going to the main menu
//-----------------------------------------------------------

#ifndef GAME_PAUSED_STATE_H
#define GAME_PAUSED_STATE_H

#include "GameState.h"
#include "../Graphics/Font.h"
#include "../Content/Menu.h"
#include "../Input/KeyboardState.h"

#include "../Graphics/Sprite.h"

class GameStateManager;

class GamePausedState : public GameState
{
private:
	Font font;
	Menu menu;

	KeyboardState prevKbState;

	Sprite background;
	
public:
	GamePausedState( GameStateManager* parent );
	~GamePausedState();

	// Virtuals from GameState
	void Enter();
	void Update( float dt );
	void Exit();

	void Render();

	// Delegate functions for the menu
	void SelectContinueGame();
	void SelectMainMenu();
};

#endif