//-----------------------------------------------------------
// File: MainMenuState.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "MainMenuState.h"

#include "../Graphics/GraphicsDeviceManager.h"
#include "GameStateManager.h"
#include "GameLogic.h"
#include "LevelStartState.h"
#include "ControlScreenState.h"
#include "Game.h"

#include "Globals.h"

MainMenuState::MainMenuState(const Content& content, GameStateManager* parent) :
	GameState(parent),
	titleFont(content.Graphics()),
	subFont1(content.Graphics()),
	menu(content, this, MakeMenuItems(content.Graphics()), 100)
{
	background.Initialize(content.Textures(), "transparent");
}

void MainMenuState::Enter()
{
	// Set the level of the game
	parent->GetGameLogic().StartIdleMode();
}

void MainMenuState::Update(float dt, const KeyboardState& keyboardState)
{
	menu.Update(keyboardState);
	parent->GetGameLogic().Update(dt);
}

void MainMenuState::Exit()
{}

void MainMenuState::Render()
{
	// Render the game
	parent->GetGameLogic().Render(false, false);

	background.Draw(Vector2(0, 0), Vector2(WINDOW_WIDTH, WINDOW_HEIGHT), 0.6f);

	menu.Render();

	Vector2 size = titleFont.GetTextSize("Asteroids Redux");
	Vector2 textPos((WINDOW_WIDTH / 2) - (size.x / 2), (WINDOW_HEIGHT / 2) - (size.y / 2) - (WINDOW_HEIGHT / 4));
	titleFont.DrawString("Asteroids Redux", textPos);

	// draw the menu controls to the screen
	size = subFont1.GetTextSize("Menu Navigation: Up/Down and Enter");
	textPos = Vector2(WINDOW_WIDTH - size.x - 7, WINDOW_HEIGHT - size.y - 7);
	subFont1.DrawString("Menu Navigation: Up/Down and Enter", textPos);
}

void MainMenuState::SelectStartGame()
{
	// Reset and start the game. Update is to run collision detection once to hide 
	// asteroid spawn errors
	parent->GetGameLogic().Reset();
	parent->GetGameLogic().Update(0.016f);
	parent->SetState<LevelStartState>();
}

void MainMenuState::SelectControls()
{
	// Show the control for the game
	parent->SetState<ControlScreenState>();
}

void MainMenuState::SelectExit() const
{
	parent->GetGameLogic().GetParent().Quit();
}

std::shared_ptr<std::vector<MenuItem>> MainMenuState::MakeMenuItems(const GraphicsDeviceManager& graphics)
{
	auto newItems = std::make_shared<std::vector<MenuItem>>();

	// Add the menu items
	newItems->push_back(MenuItem(
		graphics,
		[this]() { SelectExit(); },
		Vector2(WINDOW_WIDTH / 2, (WINDOW_HEIGHT / 2) + 30),
		"Exit Game",
		WHITE));
	newItems->push_back(MenuItem(
		graphics,
		[this]() { SelectControls(); },
		Vector2(WINDOW_WIDTH / 2, (WINDOW_HEIGHT / 2)),
		"Controls",
		WHITE));
	newItems->push_back(MenuItem(
		graphics,
		[this]() { SelectStartGame(); },
		Vector2(WINDOW_WIDTH / 2, (WINDOW_HEIGHT / 2) - 30),
		"Start Game",
		WHITE));

	return newItems;
}
