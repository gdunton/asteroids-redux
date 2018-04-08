//-----------------------------------------------------------
// File: MainMenuState.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "MainMenuState.h"

#include "GameStateManager.h"
#include "GameLogic.h"
#include "LevelStartState.h"
#include "ControlScreenState.h"
#include "Game.h"

#include "..\Utilities\FastDelegate.h"
#include "Globals.h"

MainMenuState::MainMenuState( GameStateManager* parent ) :
	GameState( parent )
{
	std::shared_ptr<std::vector<MenuItem>> items( new std::vector<MenuItem>() );

	// Add the menu items
	items->push_back( MenuItem(
		this,
		fastdelegate::MakeDelegate( this, &MainMenuState::SelectExit ),
		MyVector2( WINDOW_WIDTH / 2, (WINDOW_HEIGHT / 2) + 30 ),
		"Exit Game",
		20,
		"Ariel",
		WHITE ) );
	items->push_back( MenuItem(
		this,
		fastdelegate::MakeDelegate( this, &MainMenuState::SelectControls ),
		MyVector2( WINDOW_WIDTH / 2, (WINDOW_HEIGHT / 2) ),
		"Controls",
		20,
		"Ariel",
		WHITE ) );
	items->push_back( MenuItem(
		this,
		fastdelegate::MakeDelegate( this, &MainMenuState::SelectStartGame ),
		MyVector2( WINDOW_WIDTH / 2, (WINDOW_HEIGHT / 2) - 30 ),
		"Start Game",
		20,
		"Ariel",
		WHITE ) );
	menu = Menu(this, items, 60);

	titleFont.Initialize( 40, false, "Ariel", WHITE );
	subFont1.Initialize( 20, false, "Ariel", WHITE );

	background.Initialize( "transparent" );
}

MainMenuState::~MainMenuState()
{
	titleFont.Destroy();
	subFont1.Destroy();
}

void MainMenuState::Enter()
{
	// Set the level of the game
	parent->GetGameLogic().StartIdleMode();
}

void MainMenuState::Update( float dt )
{
	menu.Update();
	parent->GetGameLogic().Update( dt );
}

void MainMenuState::Exit()
{
}

void MainMenuState::Render()
{
	// Render the game
	parent->GetGameLogic().Render(false, false);

	background.Draw( MyVector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), MyVector2(WINDOW_WIDTH, WINDOW_HEIGHT), 0.6f );

	MyVector2 size = titleFont.GetTextSize( "Asteroids Redux" );
	MyVector2 textPos( (WINDOW_WIDTH / 2) - (size.x / 2), (WINDOW_HEIGHT / 2) - (size.y / 2) - (WINDOW_HEIGHT / 4));
	titleFont.DrawString( "Asteroids Redux", textPos );

	// draw the menu controls to the screen
	size = subFont1.GetTextSize( "Menu Navigation: Up/Down and Enter" );
	textPos = MyVector2( WINDOW_WIDTH - size.x - 7, WINDOW_HEIGHT - size.y - 7 );
	subFont1.DrawString( "Menu Navigation: Up/Down and Enter", textPos );

	menu.Render();
}

void MainMenuState::SelectStartGame()
{
	// Reset and start the game. Update is to run collision detection once to hide 
	// asteroid spawn errors
	parent->GetGameLogic().Reset();
	parent->GetGameLogic().Update( 0.016f );
	parent->SetState( std::shared_ptr<GameState>( new LevelStartState( parent ) ) );
 }

void MainMenuState::SelectControls()
{
	// Show the control for the game
	parent->SetState( std::shared_ptr<GameState>( new ControlScreenState( parent ) ) );
}

void MainMenuState::SelectExit()
{
	parent->GetGameLogic().GetParent().Quit();
}