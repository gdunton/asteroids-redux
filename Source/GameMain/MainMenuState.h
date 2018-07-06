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
public:
	MainMenuState(const Content& content, GameStateManager* parent);

	// Virtual functions from GameState
	void Enter() override;
	void Update(float dt, const KeyboardState& keyboardState) override;
	void Exit() override;

	void Render() override;

	// Delegate functions
	void SelectStartGame();
	void SelectControls();
	void SelectExit() const;

private:

	std::shared_ptr<std::vector<MenuItem>> MakeMenuItems(const GraphicsDeviceManager& graphics);

	Font titleFont;
	Font subFont1;
	Menu menu;

	Sprite background;
};

#endif
