//-----------------------------------------------------------
// File: Game.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "Game.h"

// State managers. All singletons
#include "..\Input\Keyboard.h"
#include "..\Graphics\GraphicsDeviceManager.h"
#include "..\Content\ModelManager.h"
#include "..\Content\TextureManager.h"
#include "..\Content\AudioManager.h"

#include "Globals.h"

#include "MainMenuState.h"
#include "..\Graphics\DebugFont.h"
#include "MainGameState.h"


Game::Game() : 
	FRAMES_PER_SECOND( 60 ), WINDOWED( true )
{
	m_desiredTimePerFrame = 1.0 / FRAMES_PER_SECOND;
}

Game::~Game()
{
	font.Destroy();
}

void Game::Initialize( const HINSTANCE hInstance )
{
	// Create the window
	bool success = m_window.Initialize( hInstance, 800, 600, "Asteroids Redux", this );
	ASSERT( success );

	// Initialize the keyboard input
	Keyboard::Create();

	// register messages and callbacks required for input
	m_window.AddEventListener( EventListener( WM_KEYDOWN, SetKeyStatePressed ) );
	m_window.AddEventListener( EventListener( WM_KEYUP, SetKeyStateUnpressed ) );

	// Start the gameTimer
	m_timer.Start();
	lastFrame = m_timer.GetHighResTimer();

	// Initialize the graphics device
	GraphicsDeviceManager::Create();
	bool result =  GraphicsDeviceManager::GetInstance().Initialize( m_window, true );
	if( !result )
	{
		MessageBox( NULL, L"Failed to create graphics device", L"Initialization Error", MB_OK );
	}

	TextureManager::Create();

#ifdef DEBUG
	DebugFont::Create();
#endif

	// Initialize the model manager
	ModelManager::Create();

	// Create the audio manager
	AudioManager::Create();
	AudioManager::LoadAllAssets();

	// Create the main font
	font.Initialize( 22, false, "Arial", GREEN );

	// Initialize the game logic
	m_gameLogic.Initialize(this);

	gameStateManager = GameStateManager( &m_gameLogic );
	gameStateManager.SetInitialState( std::shared_ptr<MainMenuState>( new MainMenuState( &gameStateManager ) ) );

	Line::SetLine(GraphicsDeviceManager::GetInstance().GetBatch());
}

void Game::InternalUpdate( const double deltaTime )
{
	to_String( m_timer.GetFPS(), 10, fpsString );
	
	// Update the main state then let the state manager check for any 
	// state changes and resolve them
	gameStateManager.GetCurrentState().Update( (float)deltaTime );
	gameStateManager.ResolveIncomingState();
}

void Game::VRender()
{
	// Begin the rendering
	GraphicsDeviceManager::GetInstance().BeginScene( Color( 0, 0, 0, 0 ) );

	// Start doing the line rendering
	 gameStateManager.GetCurrentState().Render();
	
	// Draw the frames per second on screen
#ifdef DEBUG
	font.DrawString( fpsString, MyVector2( 0, 0 ) );
#endif

	GraphicsDeviceManager::GetInstance().EndScene();
}

// Called from the event handler. Destroys all the assets and singletons
void Game::VClose()
{
	// Close all the singletons in reverse order to creation
#ifdef DEBUG
	DebugFont::Destroy();
#endif
	TextureManager::Destroy();
	ModelManager::Destroy();
	AudioManager::Destroy();
	GraphicsDeviceManager::Destroy();
	Keyboard::Destroy();

	// Close the window
	m_window.Close();
}

int Game::Run()
{
	return m_window.Run();
}

void Game::VUpdate()
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
		VRender();
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