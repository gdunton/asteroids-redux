//-----------------------------------------------------------
// File: LevelCompleteState.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "LevelCompleteState.h"

#include "GameStateManager.h"
#include "GameLogic.h"
#include "LevelStartState.h"

const float LevelCompleteState::EXIT_TIME = 1.0f;

LevelCompleteState::LevelCompleteState( GameStateManager* _parent )
	: GameState( _parent )
{
}

void LevelCompleteState::Enter()
{
	// Begin counting down until a new level starts
	clock.Start();
}

void LevelCompleteState::Update( float dt )
{
	// update the clock and the game
	clock.Tick( dt );
	parent->GetGameLogic().Update( dt );

	if( clock.GetDeltaTime() >= EXIT_TIME )
	{
		// Move onto the next state
		parent->SetState( std::shared_ptr< LevelStartState >( new LevelStartState( parent ) ) );
	}
}

void LevelCompleteState::Exit()
{
	// Start a new level
	parent->GetGameLogic().IncrementLevel();
}

void LevelCompleteState::Render()
{
	parent->GetGameLogic().Render();
}