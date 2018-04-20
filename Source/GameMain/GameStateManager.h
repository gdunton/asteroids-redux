//-----------------------------------------------------------
// File: GameStateManager.h
// Description: Class manages states and the state changes. When
//				a state is passes to the state manager it is stored
//				as an incoming state which is then resolved seperately
//				to avoid destruction of a state during an update.
//-----------------------------------------------------------

#ifndef GAME_STATE_MANAGER_H
#define GAME_STATE_MANAGER_H

#include <memory>
#include "GameState.h"

class GameLogic;
class Content;

class GameStateManager
{
private:
	// The state currently running
	std::shared_ptr<GameState> currentState;

	// The state queued up to become the next state
	std::shared_ptr<GameState> incomingState;

	// Reference to the logic
	GameLogic* gameLogic;

	// We need the content to create level states
	Content* content = nullptr;

public:
	GameStateManager();
	GameStateManager(Content* content, GameLogic* logic);

	// Set state functions
	void SetInitialState(std::shared_ptr<GameState> initialState);
	void SetState(std::shared_ptr<GameState> state);

	// Moves the incoming state into the currentState. Calls corrent GameState 
	// functions
	void ResolveIncomingState();

	GameState& GetCurrentState();
	GameLogic& GetGameLogic();

	template<typename T>
	void SetState()
	{
		static_assert(std::is_base_of_v<GameState, T>, "T must be a GameState subtype");
		SetState(std::make_shared<T>(*content, this));
	}
};

#endif
