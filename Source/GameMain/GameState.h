//-----------------------------------------------------------
// File: GameState.h
// Description: Abstract class manages the state of the game i.e. whether
//				the game is paused or in the main menu etc. 
//				Must be inherited and child must define Enter, Update,
//				Exit and Render.
//-----------------------------------------------------------

#ifndef GAME_STATE_H
#define GAME_STATE_H

class GameStateManager;
class KeyboardState;

class GameState
{
protected:
	GameStateManager* parent;

public:
	GameState() { parent = nullptr; }
	GameState( GameStateManager* _parent ) : parent(_parent) { } 

	virtual ~GameState() = default;

	// Move but not copyable
	GameState(GameState&& other) noexcept = default;
	GameState& operator=(GameState&& other) noexcept = default;
	GameState(const GameState& other) = delete;
	GameState& operator=(const GameState& other) = delete;

	// Called after the old state has been removed
	virtual void Enter() = 0;

	// Called each frame
	virtual void Update(float dt, const KeyboardState& keyboardState) = 0;

	// Called when a new state change has been queued just before 
	// destruction
	virtual void Exit() = 0;

	// Called each frame
	virtual void Render() = 0;
};

#endif