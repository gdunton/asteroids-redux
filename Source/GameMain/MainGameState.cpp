#include "MainGameState.h"
#include "GameStateManager.h"
#include "GameLogic.h"
#include "GameOverState.h"
#include "GamePausedState.h"
#include "LevelCompleteState.h"

#include "../Actor/Player.h"
#include "../Input/Keyboard.h"

MainGameState::MainGameState(const Content&, GameStateManager* _parent) : GameState(_parent)
{}

void MainGameState::Enter()
{
	parent->GetGameLogic().Update(0.16f);
}

void MainGameState::Update(float dt, const KeyboardState& keyboardState)
{
	// Get references for convenience
	GameLogic& ref = parent->GetGameLogic();
	Player& playerRef = ref.GetPlayer();

	// Update the game
	ref.Update(dt);

	// Check controls input for the game
	if(keyboardState.GetKeyState(VK_LEFT) == KeyState::pressed) // Turn left
	{
		playerRef.TurnAntiClockwise(dt);
	}
	else if(keyboardState.GetKeyState(VK_RIGHT) == KeyState::pressed) // Turn right
	{
		playerRef.TurnClockwise(dt);
	}
	if(keyboardState.GetKeyState(VK_UP) == KeyState::pressed) // Boost
	{
		playerRef.Boost(dt);
	}
	else
	{
		// stop the player boosting
		playerRef.EndBoost();
	}
	if(keyboardState.GetKeyState(VK_SPACE) == KeyState::pressed &&
		prevKbState.GetKeyState(VK_SPACE) == KeyState::unpressed) // Fire bullets
	{
		playerRef.FireBullet();
	}


	// Check for required state changes
	if(prevKbState.GetKeyState(VK_ESCAPE) == KeyState::pressed &&
		keyboardState.GetKeyState(VK_ESCAPE) == KeyState::unpressed) // Pause on escape
	{
		parent->SetState<GamePausedState>();
	}

	if(playerRef.GetLives() <= 0) // Game over when player is out of lives
	{
		parent->SetState<GameOverState>();
	}

	if(ref.LevelComplete()) // Level complete when level objectives are met
	{
		parent->SetState<LevelCompleteState>();
	}

	prevKbState = keyboardState;
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
