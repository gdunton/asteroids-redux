//-----------------------------------------------------------
// File: GameOverState.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "GameOverState.h"

#include "MainGameState.h"
#include "GameStateManager.h"
#include "GameLogic.h"

#include "Globals.h"
#include "..\Utilities\FastDelegate.h"
#include "..\Input\Keyboard.h"
#include "LevelStartState.h"
#include "MainMenuState.h"

GameOverState::GameOverState( GameStateManager* _parent ) :
	GameState( _parent )
{
	font.Initialize( 30, false, WHITE);

	// Create the menu
	std::shared_ptr<std::vector<MenuItem>> items(new std::vector<MenuItem>());
	items->push_back( MenuItem(
		this, 
		fastdelegate::MakeDelegate(this, &GameOverState::SelectRestartGame), 
		Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), 
		"Restart",
		20,
		"Ariel",
		WHITE) );
	items->push_back( MenuItem(
		this, 
		fastdelegate::MakeDelegate(this, &GameOverState::SelectMainMenu), 
		Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 40), 
		"Main Menu",
		20,
		"Ariel",
		WHITE) );
	menu = Menu( this, items, 100 );

	background.Initialize( "transparent" );
}

GameOverState::~GameOverState()
{
	font.Destroy();
}

void GameOverState::Enter()
{
}

void GameOverState::Update( float dt )
{
	parent->GetGameLogic().Update( dt );
	menu.Update();
}

void GameOverState::Exit()
{
}

void GameOverState::Render()
{	
	// Draw game
	parent->GetGameLogic().Render(false, false);

	// Darken the game
	background.Draw(Vector2(0, 0), Vector2(WINDOW_WIDTH, WINDOW_HEIGHT), 0.7f);

	// Draw menu
	menu.Render();

	// Draw the title
	Vector2 size = font.GetTextSize( "Game Over" );
	Vector2 textPos( (WINDOW_WIDTH / 2) - (size.x / 2), (WINDOW_HEIGHT / 2) - (size.y / 2) - (WINDOW_HEIGHT / 4));
	font.DrawString( "Game Over", textPos );
}

void GameOverState::SelectMainMenu()
{
	parent->SetState( std::shared_ptr<GameState>( new MainMenuState(parent) ) );
}

void GameOverState::SelectRestartGame()
{
	parent->GetGameLogic().Reset();
	// change state to the main game state
	parent->SetState(std::shared_ptr<GameState>( new LevelStartState( parent ) ) );
}