//-----------------------------------------------------------
// File: GamePausedState.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "GamePausedState.h"

#include "GameStateManager.h"
#include "Globals.h"

#include "../Input/Keyboard.h"

#include "MainGameState.h"
#include "MainMenuState.h"
#include "GameLogic.h"


GamePausedState::GamePausedState(GameStateManager* parent) :
	GameState(parent),
	menu(this, MakeMenuItems(), 100)
{
	Keyboard::GetInstance().GetKeyboardState(prevKbState);
	background.Initialize("transparent");
}

void GamePausedState::Enter()
{}

void GamePausedState::Update(float dt)
{
	// Menu handles navigation
	menu.Update();

	// Handle escape input to return to game
	KeyboardState kbState;
	Keyboard::GetInstance().GetKeyboardState(kbState);

	if(kbState.GetKeyState(VK_ESCAPE) == pressed &&
		prevKbState.GetKeyState(VK_ESCAPE) == unpressed)
	{
		SelectContinueGame();
	}

	prevKbState = kbState;
}

void GamePausedState::Exit()
{}

void GamePausedState::Render()
{
	// Draw game
	parent->GetGameLogic().Render();

	// Partialy obscure the game using translucent texture
	background.Draw(Vector2(0, 0), Vector2(WINDOW_WIDTH, WINDOW_HEIGHT), 0.7f);

	// Draw the menu
	menu.Render();

	// Render the title
	Vector2 size = font.GetTextSize("Paused");
	Vector2 textPos((WINDOW_WIDTH / 2) - (size.x / 2), (WINDOW_HEIGHT / 2) - (size.y / 2) - (WINDOW_HEIGHT / 4));
	font.DrawString("Paused", textPos);
}

void GamePausedState::SelectContinueGame() const
{
	parent->SetState(std::make_shared<MainGameState>(parent));
}

void GamePausedState::SelectMainMenu() const
{
	parent->SetState(std::make_shared<MainMenuState>(parent));
}

std::shared_ptr<std::vector<MenuItem>> GamePausedState::MakeMenuItems()
{
	auto items = std::make_shared<std::vector<MenuItem>>();

	items->push_back(MenuItem(
		[this]() { SelectMainMenu(); },
		Vector2(WINDOW_WIDTH / 2, (WINDOW_HEIGHT / 2) + 30),
		"Main Menu",
		WHITE));
	items->push_back(MenuItem(
		[this]() { SelectContinueGame(); },
		Vector2(WINDOW_WIDTH / 2, (WINDOW_HEIGHT / 2)),
		"Return",
		WHITE));

	return items;
}
