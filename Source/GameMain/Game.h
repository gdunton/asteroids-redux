//-----------------------------------------------------------
// File: Game.h
// Description: Central class of the game. Initializes and destroys
//				all singletons as well as the window. Manages timing 
//				of game update and rendering calls using a high
//				frequency timer. Stores gameLogic and the gameStateManager
//-----------------------------------------------------------

#ifndef GAME_H
#define GAME_H

#include "../UserInterface/Window.h"
#include "../EventManager/Timer.h"
#include "GameLogic.h"
#include "GameStateManager.h"

#include "../Graphics/Font.h"
#include "../Graphics/GraphicsDeviceManager.h"
#include "../Content/TextureManager.h"

class Game
{
public:
	explicit Game(HINSTANCE hInstance);

	void Initialize();
	int Run();

	void Update();
	void Render();

	// Called from the event handler. Destroys all the assets and singletons
	void Close();
	// Posts the quit message to the event handler
	void Quit();

	int GetClientWidth();
	int GetClientHeight();

private:

	// Update called when the required time has past to result in desired fps
	void InternalUpdate(double deltaTime);
	
	Window m_window;
	GraphicsDeviceManager graphicsDeviceManager;
	TextureManager textureManager;

	std::string fpsString;
	Font font;

	// The central rules of the game and the state manager
	GameLogic m_gameLogic;
	GameStateManager gameStateManager;

	Timer m_timer;
	__int64 lastFrame = 0;
	double m_desiredTimePerFrame = 0.0;

	const int FRAMES_PER_SECOND = 60;
};

#endif