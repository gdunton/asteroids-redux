//-----------------------------------------------------------
// File: MainGameState.h
// Description: Impliments GameState. Displays game in the background.
//				Checks for user input and uses input to control the game.
//-----------------------------------------------------------

#ifndef MAIN_GAME_STATE_H
#define MAIN_GAME_STATE_H

#include "GameState.h"

#include "../Input/KeyboardState.h"

class Content;
class GraphicsDeviceManager;

class MainGameState : public GameState
{
public:
	MainGameState(const Content&, GameStateManager* _parent);
	MainGameState() = default;

	void Enter() override;
	void Update(float dt, const KeyboardState& keyboardState) override;
	void Exit() override;

	void Render() override;

private:
	KeyboardState prevKbState;
};

#endif
