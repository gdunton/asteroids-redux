//-----------------------------------------------------------
// File: ControlScreenState.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "ControlScreenState.h"

#include "GameStateManager.h"
#include "GameLogic.h"
#include "Globals.h"

#include "..\Input\Keyboard.h"
#include "MainMenuState.h"
#include "..\Actor\Player.h"

// List of descriptions and their controls
String const ControlScreenState::controlStrings[] = { 
	"Change Direction: Left/Right", 
	"Move Forward: Up", 
	"Fire: Space", 
	"Pause: Esc"
};

ControlScreenState::ControlScreenState( GameStateManager* _parent )
	: GameState( _parent )
{
	titleFont.Initialize( 40, false, WHITE);
	subFont1.Initialize( 20, false, ColorRGBA( 255, 255, 255, 255 ));

	background.Initialize( "transparent" );

	Keyboard::GetInstance().GetKeyboardState( prevKbState );
}

ControlScreenState::~ControlScreenState()
{
	titleFont.Destroy();
	subFont1.Destroy();
}

void ControlScreenState::Enter() 
{
	parent->GetGameLogic().StartPlayerOnlyLevel();
}

void ControlScreenState::Update( float dt )
{
	// Get the keyboard input
	KeyboardState kbState;
	Keyboard::GetInstance().GetKeyboardState( kbState );

	Player& playerRef = parent->GetGameLogic().GetPlayer();

	// Handle the keyboard input
	if( (kbState.GetKeyState( VK_ESCAPE ) == pressed && prevKbState.GetKeyState( VK_ESCAPE ) == unpressed) ||
		(kbState.GetKeyState( VK_RETURN ) == pressed && prevKbState.GetKeyState( VK_RETURN ) == unpressed) )
	{
		parent->SetState( std::shared_ptr<GameState>( new MainMenuState( parent ) ) );
	}

	// Check the input for the game
	if( kbState.GetKeyState( VK_LEFT ) == pressed )
	{
		playerRef.TurnAntiClockwise(dt);
	}
	else if ( kbState.GetKeyState( VK_RIGHT ) == pressed )
	{
		playerRef.TurnClockwise(dt);
	}
	if( kbState.GetKeyState( VK_UP ) == pressed )
	{
		playerRef.Boost( dt );
	}
	else
	{
		playerRef.EndBoost();
	}
	if( kbState.GetKeyState( VK_SPACE ) == pressed &&
		prevKbState.GetKeyState( VK_SPACE ) == unpressed )
	{
		playerRef.FireBullet();
	}

	parent->GetGameLogic().Update( dt );

	prevKbState = kbState;
}

void ControlScreenState::Exit()
{
	parent->GetGameLogic().GetPlayer().StopSounds();
}

void ControlScreenState::Render()
{
	// Render the game
	parent->GetGameLogic().Render(false, false);

	// Render the background to grey out the game
	background.Draw( MyVector2(0, 0), MyVector2(WINDOW_WIDTH, WINDOW_HEIGHT), 0.17f );

	// Draw the titles
	MyVector2 size = titleFont.GetTextSize( "Controls" );
	MyVector2 textPos( (WINDOW_WIDTH / 2) - (size.x / 2), (WINDOW_HEIGHT / 2) - (size.y / 2) - (WINDOW_HEIGHT / 4));
	titleFont.DrawString( "Controls", textPos );


	// Draw all the controls strings
	// Set the y coord of the first string
	int sampleHeight = subFont1.GetTextSize( "1" ).y;
	int iterations = sizeof( controlStrings ) / sizeof( String );
	textPos.y = (WINDOW_HEIGHT / 2) - (sampleHeight * (iterations / 2 + 4));
	for( int i = 0; i < iterations; ++i )
	{
		size = subFont1.GetTextSize( controlStrings[i] );
		textPos.x = (WINDOW_WIDTH / 2) - (size.x / 2);
		textPos.y += (size.y * 2);
		subFont1.DrawString( controlStrings[i], textPos );
	}

	// Print the message showing how to leave
	size = subFont1.GetTextSize( "Press Esc or Return to go to Main Menu" );
	textPos = MyVector2( WINDOW_WIDTH - size.x - 7, WINDOW_HEIGHT - size.y - 7 );
	subFont1.DrawString( "Press Esc or Return to go to Main Menu", textPos );
}