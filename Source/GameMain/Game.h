//-----------------------------------------------------------
// File: Game.h
// Description: Central class of the game. Initializes and destroys
//				all singletons as well as the window. Manages timing 
//				of game update and rendering calls using a high
//				frequency timer. Stores gameLogic and the gameStateManager
//-----------------------------------------------------------

#ifndef GAME_H
#define GAME_H

#include "../EventManager/Timer.h"
#include "GameLogic.h"
#include "GameStateManager.h"

#include "../Graphics/Font.h"
#include "../Graphics/GraphicsDeviceManager.h"
#include "../Content/TextureManager.h"
#include "../Content/ModelManager.h"
#include "../Content/Content.h"

class IWindow;

class Game
{
public:
	explicit Game(IWindow& windowHandle);

	void Initialize();

	void Update(const KeyboardState& keyboardState);
	void Render();

	// Called from the event handler. Destroys all the assets
	void Close() const;

private:

	// Update called when the required time has past to result in desired fps
	void InternalUpdate(double deltaTime, const KeyboardState& keyboardState);
	
	IWindow& windowHandle;
	GraphicsDeviceManager graphicsDeviceManager;
	Content content;

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