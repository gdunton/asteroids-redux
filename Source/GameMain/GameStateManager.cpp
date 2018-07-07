//-----------------------------------------------------------
// File: GameStateManager.cpp
//-----------------------------------------------------------

#include "GameStateManager.h"

GameStateManager::GameStateManager() noexcept :
	gameLogic(nullptr)
{}

GameStateManager::GameStateManager(Content* content, GameLogic* logic) noexcept :
	gameLogic(logic),
	content(content)
{}

void GameStateManager::SetInitialState(const std::shared_ptr<GameState>& initialState)
{
	// If current state hasn't yet been set
	if(!currentState.get())
	{
		currentState = initialState;
		currentState->Enter();
	}
}

void GameStateManager::SetState(const std::shared_ptr<GameState>& state)
{
	// If the incoming state hasn't been set then set it
	if(!incomingState.get())
	{
		incomingState = state;
	}
}

void GameStateManager::ResolveIncomingState()
{
	// If there is an incoming state
	if(incomingState)
	{
		// Exit the current state
		currentState->Exit();

		// Destroy the old state and set the new one as current
		currentState.reset();
		currentState = incomingState;

		// Run enter on the new current state
		currentState->Enter();

		// Clear the incoming state
		incomingState.reset();
	}
}

GameState& GameStateManager::GetCurrentState()
{
	return *currentState.get();
}

GameLogic& GameStateManager::GetGameLogic()
{
	return *gameLogic;
}
