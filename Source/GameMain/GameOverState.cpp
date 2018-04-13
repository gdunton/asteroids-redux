//-----------------------------------------------------------
// File: GameOverState.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "GameOverState.h"

#include "MainGameState.h"
#include "GameStateManager.h"
#include "GameLogic.h"

#include "Globals.h"
#include "../Input/Keyboard.h"
#include "LevelStartState.h"
#include "MainMenuState.h"

GameOverState::GameOverState(GameStateManager* parent) :
	GameState(parent),
	menu(this, MakeMenuItems(), 100)
{
	background.Initialize("transparent");
}

void GameOverState::Enter()
{}

void GameOverState::Update(float dt)
{
	parent->GetGameLogic().Update(dt);
	menu.Update();
}

void GameOverState::Exit()
{}

void GameOverState::Render()
{
	// Draw game
	parent->GetGameLogic().Render(false, false);

	// Darken the game
	background.Draw(Vector2(0, 0), Vector2(WINDOW_WIDTH, WINDOW_HEIGHT), 0.7f);

	// Draw menu
	menu.Render();

	// Draw the title
	Vector2 size = font.GetTextSize("Game Over");
	Vector2 textPos((WINDOW_WIDTH / 2) - (size.x / 2), (WINDOW_HEIGHT / 2) - (size.y / 2) - (WINDOW_HEIGHT / 4));
	font.DrawString("Game Over", textPos);
}

void GameOverState::SelectMainMenu()
{
	parent->SetState(std::make_shared<MainMenuState>(parent));
}

void GameOverState::SelectRestartGame()
{
	parent->GetGameLogic().Reset();
	// change state to the main game state
	parent->SetState(std::make_shared<LevelStartState>(parent));
}

std::shared_ptr<std::vector<MenuItem>> GameOverState::MakeMenuItems()
{
	auto menuItems = std::make_shared<std::vector<MenuItem>>();

	menuItems->push_back(MenuItem(
		[this]() { SelectRestartGame(); },
		Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2),
		"Restart",
		WHITE));
	menuItems->push_back(MenuItem(
		[this]() { SelectMainMenu(); },
		Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 40),
		"Main Menu",
		WHITE));

	return menuItems;
}
