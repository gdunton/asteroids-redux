//-----------------------------------------------------------
// File: Game.h
// Description: Central class of the game. Initializes and destroys
//				all singletons as well as the window. Manages timing 
//				of game update and rendering calls using a high
//				frequency timer. Stores gameLogic and the gameStateManager
//-----------------------------------------------------------

#ifndef GAME_H
#define GAME_H

#include "..\UserInterface\Window.h"
#include "..\EventManager\Timer.h"
#include "GameLogic.h"
#include "GameStateManager.h"

#include "..\Graphics\Font.h"

class Game : public IGame
{
public:
	Game();
	~Game();

	void Initialize( const HINSTANCE hInstance );
	int Run();

	void VUpdate();
	void VRender();

	// Called from the event handler. Destroys all the assets and singletons
	void VClose();
	// Posts the quit message to the event handler
	void Quit();

	int GetClientWidth();
	int GetClientHeight();

	int GetWorldWidth();
	int GetWorldHeight();

private:

	// Update called when the required time has past to result in desired fps
	void InternalUpdate( const double deltaTime);
	

private:

	Window m_window;

	String fpsString;
	Font font;

	// The central rules of the game and the state manager
	GameLogic m_gameLogic;
	GameStateManager gameStateManager;

	Timer m_timer;
	__int64 lastFrame;
	double m_desiredTimePerFrame;

	const int FRAMES_PER_SECOND;
	const bool WINDOWED;
};

#endif