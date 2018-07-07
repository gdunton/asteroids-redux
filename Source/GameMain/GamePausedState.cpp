//-----------------------------------------------------------
// File: GamePausedState.cpp
//-----------------------------------------------------------

#include "GamePausedState.h"

#include "GameStateManager.h"
#include "Globals.h"

#include "../Input/Keyboard.h"
#include "../Graphics/GraphicsDeviceManager.h"
#include "MainGameState.h"
#include "MainMenuState.h"
#include "GameLogic.h"


GamePausedState::GamePausedState(const Content& content, GameStateManager* parent) :
	GameState(parent),
	font(content.Graphics()),
	menu(content, MakeMenuItems(content.Graphics()), 100),
	background(content.Textures(), "transparent")
{ }

void GamePausedState::Enter()
{}

void GamePausedState::Update(float /*dt*/, const KeyboardState& keyboardState)
{
	// Menu handles navigation
	menu.Update(keyboardState);

	// Handle escape input to return to game
	if(prevKbState.GetKeyState(VK_ESCAPE) == KeyState::pressed &&
		keyboardState.GetKeyState(VK_ESCAPE) == KeyState::unpressed)
	{
		SelectContinueGame();
	}

	prevKbState = keyboardState;
}

void GamePausedState::Exit()
{}

void GamePausedState::Render()
{
	// Draw game
	parent->GetGameLogic().Render();

	// Partialy obscure the game using translucent texture
	background.Draw(Vector2(0, 0), 0.7f);

	// Draw the menu
	menu.Render();

	// Render the title
	const Vector2 size = font.GetTextSize("Paused");
	const Vector2 textPos((WINDOW_WIDTH / 2) - (size.x / 2), (WINDOW_HEIGHT / 2) - (size.y / 2) - (WINDOW_HEIGHT / 4));
	font.DrawString("Paused", textPos);
}

void GamePausedState::SelectContinueGame() const
{
	parent->SetState<MainGameState>();
}

void GamePausedState::SelectMainMenu() const
{
	parent->SetState<MainMenuState>();
}

std::shared_ptr<std::vector<MenuItem>> GamePausedState::MakeMenuItems(const GraphicsDeviceManager& graphics)
{
	auto items = std::make_shared<std::vector<MenuItem>>();

	items->push_back(MenuItem(
		graphics,
		[this]() { SelectMainMenu(); },
		Vector2(WINDOW_WIDTH / 2, (WINDOW_HEIGHT / 2) + 30),
		"Main Menu",
		WHITE));
	items->push_back(MenuItem(
		graphics,
		[this]() { SelectContinueGame(); },
		Vector2(WINDOW_WIDTH / 2, (WINDOW_HEIGHT / 2)),
		"Return",
		WHITE));

	return items;
}
