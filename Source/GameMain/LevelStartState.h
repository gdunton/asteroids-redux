//-----------------------------------------------------------
// File: LevelStartState.h
// Description: Implements GameState. Shows the current level
//				in tha background but does not update the game.
//				Displays a countdown before the level starts
//-----------------------------------------------------------

#ifndef LEVEL_START_STATE_H
#define LEVEL_START_STATE_H

#include "GameState.h"
#include "../EventManager/AnimationClock.h"

#include "../Graphics/Font.h"
#include "../Graphics/Sprite.h"

class LevelStartState : public GameState
{
public:
	LevelStartState( GameStateManager* parent );

	// Virtual functions from GameState
	void Enter() override;
	void Update( float dt ) override;
	void Exit() override;

	void Render() override;

private:
	AnimationClock clock;
	Sprite background;
	Font font;

	// Variables to show text and position
	Vector2 textPosition;
	std::string onScreenText;
	Vector2 countDownPos;
	std::string countDownText;

	const static float INTRO_TIME;
};

#endif