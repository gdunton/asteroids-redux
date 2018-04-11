//-----------------------------------------------------------
// File: MainMenuState.h
// Description: Implements GameState. Starts a special level with 
//				no player and asteroid that do not wrap around.
//				Displays a menu with options for the controls screen,
//				start a game and close the game
//-----------------------------------------------------------

#ifndef MAIN_MENU_STATE
#define MAIN_MENU_STATE

#include "GameState.h"
#include "../Graphics/Font.h"
#include "../Content/Menu.h"

#include "../Graphics/Sprite.h"

class MainMenuState : public GameState
{
private:
	Font titleFont;
	Font subFont1;
	Menu menu;

	Sprite background;
	
public:
	MainMenuState( GameStateManager* parent );
	~MainMenuState();

	// Virtual functions from GameState
	void Enter();
	void Update( float dt );
	void Exit();

	void Render();

	// Delegate functions
	void SelectStartGame();
	void SelectControls();
	void SelectExit();
};

#endif