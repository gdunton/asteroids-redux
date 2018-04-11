#include "STD.h"
#include "MainGameState.h"
#include "GameStateManager.h"
#include "GameLogic.h"
#include "GameOverState.h"
#include "GamePausedState.h"
#include "LevelCompleteState.h"

#include "../Actor/Player.h"
#include "../Input/Keyboard.h"

#include "Globals.h"


MainGameState::MainGameState( GameStateManager* _parent ) : GameState( _parent )
{
	Keyboard::GetInstance().GetKeyboardState( prevKbState );
}

MainGameState::MainGameState()
{
}

void MainGameState::Enter()
{
	parent->GetGameLogic().Update( 0.16f );
}

void MainGameState::Update( float dt )
{
	// Set up keyboard
	KeyboardState kbState;
	Keyboard::GetInstance().GetKeyboardState( kbState );

	// Get references for convenience
	GameLogic& ref = parent->GetGameLogic();
	Player& playerRef = ref.GetPlayer();

	// Update the game
	ref.Update( dt );

	// Check controls input for the game
	if( kbState.GetKeyState( VK_LEFT ) == pressed )			// Turn left
	{
		playerRef.TurnAntiClockwise(dt);
	}
	else if ( kbState.GetKeyState( VK_RIGHT ) == pressed )	// Turn right
	{
		playerRef.TurnClockwise(dt);
	}
	if( kbState.GetKeyState( VK_UP ) == pressed )			// Boost
	{
		playerRef.Boost( dt );
	}
	else
	{
		// stop the player boosting
		playerRef.EndBoost();
	}
	if( kbState.GetKeyState( VK_SPACE ) == pressed &&
		prevKbState.GetKeyState( VK_SPACE ) == unpressed )  // Fire bullets
	{
		playerRef.FireBullet();
	}


	// Check for required state changes
	if( kbState.GetKeyState( VK_ESCAPE ) == pressed &&
		prevKbState.GetKeyState( VK_ESCAPE ) == unpressed ) // Pause on escape
	{
		parent->SetState( std::shared_ptr<GameState>(new GamePausedState( parent ) ) );
	}

	if( playerRef.GetLives() <= 0 )							// Game over when player is out of lives
	{
		parent->SetState( std::shared_ptr<GameOverState>(new GameOverState(parent)) );
	}

	if( ref.LevelComplete() )								// Level complete when level objectives are met
	{
		parent->SetState( std::shared_ptr<LevelCompleteState>( new LevelCompleteState( parent ) ) );
	}

	prevKbState = kbState;
}

void MainGameState::Exit()
{
	// Stop all sounds from the player. Main target is the continuous thrust sound
	parent->GetGameLogic().GetPlayer().StopSounds();
}

void MainGameState::Render()
{
	// Draw the game as normal
	parent->GetGameLogic().Render();
}