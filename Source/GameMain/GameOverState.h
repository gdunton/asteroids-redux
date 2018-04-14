//-----------------------------------------------------------
// File: GameOverState.h
// Description: Inherits GameState. Shows a menu over the top of
//				the game. User can select to restart the game or
//				go to the main menu
//-----------------------------------------------------------

#ifndef GAME_OVER_STATE_H
#define GAME_OVER_STATE_H

#include "GameState.h"
#include "../Graphics/Font.h"
#include "../Content/Menu.h"
#include "../Graphics/Sprite.h"

class GameOverState : public GameState
{
public:
	explicit GameOverState(GraphicsDeviceManager& graphics, GameStateManager* parent);

	// Virtuals from GameState
	void Enter() override;
	void Update(float dt, const KeyboardState& keyboardState) override;
	void Exit() override;

	void Render() override;

	// Delegate functions called by the menu
	void SelectMainMenu();
	void SelectRestartGame();

private:
	std::shared_ptr<std::vector<MenuItem>> MakeMenuItems(GraphicsDeviceManager& graphics);

	Font font;
	Menu menu;

	Sprite background;
};

#endif
