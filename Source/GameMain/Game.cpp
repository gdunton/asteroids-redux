//-----------------------------------------------------------
// File: Game.cpp
//-----------------------------------------------------------

#include "Game.h"

#include <fstream>
#include "../UserInterface/IWindow.h"
#include "../Graphics/GraphicsDeviceManager.h"

#include "MainMenuState.h"
#include "MainGameState.h"
#include <iomanip>


Game::Game(IWindow* windowHandle, std::unique_ptr<GraphicsDeviceManager> graphics) :
	windowHandle(windowHandle),
	graphicsDeviceManager(std::move(graphics)),
	content(*graphicsDeviceManager),
	font(*graphicsDeviceManager, GREEN),
	m_gameLogic(content)
{
	m_desiredTimePerFrame = 1.0 / FRAMES_PER_SECOND;
	stateBuffer.reserve(1800);
}

void Game::Initialize()
{
	// Start the gameTimer
	m_timer.Start();
	lastFrame = m_timer.GetHighResTimer();

	// Initialize the game logic
	m_gameLogic.Initialize(this);

	gameStateManager = GameStateManager(&content, &m_gameLogic);
	gameStateManager.SetInitialState(std::make_shared<MainMenuState>(content, &gameStateManager));

	Line::SetLine(graphicsDeviceManager->GetBatch());
	Sprite::SetSpriteBatch(graphicsDeviceManager->GetSpriteBatch());
}

void Game::InternalUpdate(const double deltaTime, const KeyboardState& keyboardState)
{
	stateBuffer.emplace_back(deltaTime, keyboardState);

	fpsString = std::to_string(m_timer.GetFPS());

	// Update the main state then let the state manager check for any 
	// state changes and resolve them
	gameStateManager.GetCurrentState().Update(static_cast<float>(deltaTime), keyboardState);
	gameStateManager.ResolveIncomingState();
}

void Game::Render()
{
	// Begin the rendering
	graphicsDeviceManager->BeginScene(Color(0, 0, 0, 0));

	// Start doing the line rendering
	gameStateManager.GetCurrentState().Render();

	// Draw the frames per second on screen
#ifdef DEBUG
	font.DrawString(fpsString, Vector2(0, 0));
#endif

	graphicsDeviceManager->EndScene();
}

void Game::Close() const
{
	// Close the window
	if (windowHandle != nullptr)
	{
		windowHandle->Close();
	}

	// Save the buffer to disk
	std::ofstream outStream;
	outStream.open("run.txt", std::ostream::out);
	outStream << std::setprecision(10);
	if (outStream.is_open())
	{
		for (const auto& frame : stateBuffer)
		{
			outStream << frame.first << '|' << frame.second << '\n';
		}
		outStream.close();
	}
}

void Game::Update(const KeyboardState& keyboardState)
{
	// Get the current time in seconds
	double currTime = m_timer.GetCurrTimeSecs();
	__int64 begin = m_timer.GetHighResTimer();

	double dt = (begin - lastFrame) * m_timer.GetSecsPerCount();
	bool runUpdate = dt > m_desiredTimePerFrame;
	if(runUpdate)
	{
		// Limit the dt value
		if(dt > (m_desiredTimePerFrame * 2))
		{
			dt = m_desiredTimePerFrame;
		}

		// Tick the fps counter
		m_timer.TickFPSCounter();

		InternalUpdate(dt, keyboardState);
		Render();
		lastFrame = begin;
	}
}
