//-----------------------------------------------------------
// File: MainGameState.h
// Description: Impliments GameState. Displays game in the background.
//				Checks for user input and uses input to control the game.
//-----------------------------------------------------------

#ifndef MAIN_GAME_STATE_H
#define MAIN_GAME_STATE_H

#include "GameState.h"

#include "../Input/KeyboardState.h"

class GraphicsDeviceManager;

class MainGameState : public GameState
{
private:
	KeyboardState prevKbState;

public:
	MainGameState(GraphicsDeviceManager&, GameStateManager* _parent );
	MainGameState() = default;

	// Virtual functions from GameState
	void Enter();
	void Update(float dt, const KeyboardState& keyboardState);
	void Exit();

	void Render();
};

#endif