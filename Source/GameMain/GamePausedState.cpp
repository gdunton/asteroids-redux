//-----------------------------------------------------------
// File: GamePausedState.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "GamePausedState.h"

#include "GameStateManager.h"
#include "../Utilities/FastDelegate.h"
#include "Globals.h"

#include "../Input/Keyboard.h"

#include "MainGameState.h"
#include "MainMenuState.h"
#include "GameLogic.h"


GamePausedState::GamePausedState( GameStateManager* parent ) :
	GameState( parent )
{
	font.Initialize( 30, false, WHITE);

	// Create the menu
	std::shared_ptr<std::vector<MenuItem>> items(new std::vector<MenuItem>());
	items->push_back( MenuItem(
		this,
		fastdelegate::MakeDelegate( this, &GamePausedState::SelectMainMenu),
		Vector2( WINDOW_WIDTH / 2, (WINDOW_HEIGHT / 2) + 30 ),
		"Main Menu",
		20,
		"Ariel",
		WHITE ) );
	items->push_back( MenuItem(
		this,
		fastdelegate::MakeDelegate( this, &GamePausedState::SelectContinueGame),
		Vector2( WINDOW_WIDTH / 2, (WINDOW_HEIGHT / 2) ),
		"Return",
		20,
		"Ariel",
		WHITE ) );
	menu = Menu( this, items, 100 );

	Keyboard::GetInstance().GetKeyboardState( prevKbState );

	background.Initialize( "transparent" );
}

GamePausedState::~GamePausedState()
{
	font.Destroy();
}

void GamePausedState::Enter()
{
}

void GamePausedState::Update( float dt )
{
	// Menu handles navigation
	menu.Update();

	// Handle escape input to return to game
	KeyboardState kbState;
	Keyboard::GetInstance().GetKeyboardState( kbState );

	if( kbState.GetKeyState( VK_ESCAPE ) == pressed &&
		prevKbState.GetKeyState( VK_ESCAPE ) == unpressed )
	{
		SelectContinueGame();
	}

	prevKbState = kbState;
}

void GamePausedState::Exit()
{
}

void GamePausedState::Render()
{
	// Draw game
	parent->GetGameLogic().Render();

	// Partialy obscure the game using translucent texture
	background.Draw( Vector2(0, 0), Vector2(WINDOW_WIDTH, WINDOW_HEIGHT), 0.7f );

	// Draw the menu
	menu.Render();

	// Render the title
	Vector2 size = font.GetTextSize( "Paused" );
	Vector2 textPos( (WINDOW_WIDTH / 2) - (size.x / 2), (WINDOW_HEIGHT / 2) - (size.y / 2) - (WINDOW_HEIGHT / 4));
	font.DrawString( "Paused", textPos );
}

void GamePausedState::SelectContinueGame()
{
	parent->SetState( std::shared_ptr<GameState>( new MainGameState(parent) ) );
}

void GamePausedState::SelectMainMenu()
{
	parent->SetState( std::shared_ptr<GameState>( new MainMenuState( parent ) ) );
}