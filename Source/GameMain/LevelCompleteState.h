//-----------------------------------------------------------
// File: LevelCompleteState.h
// Description: Implements GameState. Enters just as the game
//				ends and continues running the game for a couple
//				of seconds before starting a new level
//-----------------------------------------------------------

#ifndef LEVEL_COMPLETE_STATE_H
#define LEVEL_COMPLETE_STATE_H

#include "GameState.h"
#include "../EventManager/AnimationClock.h"

class Content;
class GraphicsDeviceManager;

class LevelCompleteState : public GameState
{
public:
	LevelCompleteState(const Content&, GameStateManager* _parent);

	// Virtual functions from GameState
	void Enter() override;
	void Update(float dt, const KeyboardState& keyboardState) override;
	void Exit() override;

	void Render() override;

private:
	AnimationClock clock;
	
	const static float EXIT_TIME;
};

#endif
