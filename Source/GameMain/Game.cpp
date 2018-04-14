//-----------------------------------------------------------
// File: Game.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "Game.h"

// State managers. All singletons
#include "../Input/Keyboard.h"
#include "../Graphics/GraphicsDeviceManager.h"
#include "../Content/ModelManager.h"
#include "../Content/TextureManager.h"
#include "../Content/AudioManager.h"

#include "Globals.h"

#include "MainMenuState.h"
#include "MainGameState.h"


Game::Game(HINSTANCE hInstance) :
	m_window(hInstance, 800, 600, "Asteroids Redux", this),
	graphicsDeviceManager(m_window, true),
	textureManager(graphicsDeviceManager),
	font(graphicsDeviceManager, GREEN),
	m_gameLogic(graphicsDeviceManager)
{
	m_desiredTimePerFrame = 1.0 / FRAMES_PER_SECOND;
}

void Game::Initialize()
{
	// Start the gameTimer
	m_timer.Start();
	lastFrame = m_timer.GetHighResTimer();

	// Initialize the model manager
	ModelManager::Create();

	// Create the audio manager
	AudioManager::Create();
	AudioManager::LoadAllAssets();

	// Initialize the game logic
	m_gameLogic.Initialize(this, ModelManager::GetInstance().GetModel("Quad"));

	gameStateManager = GameStateManager(&graphicsDeviceManager, &m_gameLogic );
	gameStateManager.SetInitialState(std::make_shared<MainMenuState>(graphicsDeviceManager, &gameStateManager));

	Line::SetLine(graphicsDeviceManager.GetBatch());
	Sprite::SetSpriteBatch(graphicsDeviceManager.GetSpriteBatch());
}

void Game::InternalUpdate( const double deltaTime )
{
	to_String( m_timer.GetFPS(), 10, fpsString );
	
	// Update the main state then let the state manager check for any 
	// state changes and resolve them
	gameStateManager.GetCurrentState().Update( static_cast<float>(deltaTime), m_window.GetKeyboardState());
	gameStateManager.ResolveIncomingState();
}

void Game::Render()
{
	// Begin the rendering
	graphicsDeviceManager.BeginScene( Color( 0, 0, 0, 0 ) );

	// Start doing the line rendering
	 gameStateManager.GetCurrentState().Render();
	
	// Draw the frames per second on screen
#ifdef DEBUG
	font.DrawString( fpsString, Vector2( 0, 0 ) );
#endif

	graphicsDeviceManager.EndScene();
}

// Called from the event handler. Destroys all the assets and singletons
void Game::Close()
{
	ModelManager::Destroy();
	AudioManager::Destroy();

	// Close the window
	m_window.Close();
}

int Game::Run()
{
	return m_window.Run();
}

void Game::Update()
{
	// Get the current time in seconds
	double currTime = m_timer.GetCurrTimeSecs();
	__int64 begin = m_timer.GetHighResTimer();
	
	double dt = (begin - lastFrame) * m_timer.GetSecsPerCount();
	bool runUpdate = dt > m_desiredTimePerFrame;
	if( runUpdate )
	{
		// Limit the dt value
		if( dt > (m_desiredTimePerFrame * 2) )
		{
			dt = m_desiredTimePerFrame;
		}

		// Tick the fps counter
		m_timer.TickFPSCounter();

		InternalUpdate(dt);
		Render();
		lastFrame = begin;
	}

	
}

// Posts the quit message to the event handler
void Game::Quit()
{
	PostMessage( m_window.GetWindowHandle(), WM_CLOSE, 0, 0 );
}

int Game::GetClientWidth()
{
	return m_window.GetClientWidth();
}
int Game::GetClientHeight()
{
	return m_window.GetClientHeight();
}

