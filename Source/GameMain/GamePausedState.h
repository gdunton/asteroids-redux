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
public:
	GamePausedState(const Content& content, GameStateManager* parent);

	// Virtuals from GameState
	void Enter() override;
	void Update(float dt, const KeyboardState& keyboardState) override;
	void Exit() override;

	void Render() override;

	// Delegate functions for the menu
	void SelectContinueGame() const;
	void SelectMainMenu() const;

private:
	std::shared_ptr<std::vector<MenuItem>> MakeMenuItems(const GraphicsDeviceManager& graphics);

	Font font;
	Menu menu;

	KeyboardState prevKbState;

	Sprite background;
};

#endif
