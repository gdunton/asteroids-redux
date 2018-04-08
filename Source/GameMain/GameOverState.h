//-----------------------------------------------------------
// File: GameOverState.h
// Description: Inherits GameState. Shows a menu over the top of
//				the game. User can select to restart the game or
//				go to the main menu
//-----------------------------------------------------------

#ifndef GAME_OVER_STATE_H
#define GAME_OVER_STATE_H

#include "GameState.h"
#include "..\Graphics\Font.h"
#include "..\Content\Menu.h"
#include "..\Graphics\Sprite.h"

class GameOverState : public GameState
{
private:
	Font font;
	Menu menu;

	Sprite background;

public:
	GameOverState( GameStateManager* _parent );
	~GameOverState();

	// Virtuals from GameState
	void Enter();
	void Update( float dt );
	void Exit();

	void Render();

	// Delegate functions called by the menu
	void SelectMainMenu();
	void SelectRestartGame();
};

#endif