//-----------------------------------------------------------
// File: ControlScreenState.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "ControlScreenState.h"

#include "../Graphics/GraphicsDeviceManager.h"
#include "GameStateManager.h"
#include "GameLogic.h"
#include "Globals.h"

#include "../Input/Keyboard.h"
#include "MainMenuState.h"
#include "../Actor/Player.h"

// List of descriptions and their controls
std::string const ControlScreenState::controlStrings[] = {
	"Change Direction: Left/Right",
	"Move Forward: Up",
	"Fire: Space",
	"Pause: Esc"
};

ControlScreenState::ControlScreenState(const Content& content, GameStateManager* parent) :
	GameState(parent),
	titleFont(content.Graphics()),
	subFont1(content.Graphics())
{
	background.Initialize(content.Textures(), "transparent");
}

void ControlScreenState::Enter()
{
	parent->GetGameLogic().StartPlayerOnlyLevel();
}

void ControlScreenState::Update(float dt, const KeyboardState& keyboardState)
{
	// Get the keyboard input
	Player& playerRef = parent->GetGameLogic().GetPlayer();

	// Handle the keyboard input
	if((keyboardState.GetKeyState(VK_ESCAPE) == KeyState::pressed && prevKbState.GetKeyState(VK_ESCAPE) == KeyState::
			unpressed) ||
		(keyboardState.GetKeyState(VK_RETURN) == KeyState::pressed && prevKbState.GetKeyState(VK_RETURN) == KeyState::
			unpressed))
	{
		parent->SetState<MainMenuState>();
	}

	// Check the input for the game
	if(keyboardState.GetKeyState(VK_LEFT) == KeyState::pressed)
	{
		playerRef.TurnAntiClockwise(dt);
	}
	else if(keyboardState.GetKeyState(VK_RIGHT) == KeyState::pressed)
	{
		playerRef.TurnClockwise(dt);
	}
	if(keyboardState.GetKeyState(VK_UP) == KeyState::pressed)
	{
		playerRef.Boost(dt);
	}
	else
	{
		playerRef.EndBoost();
	}
	if(keyboardState.GetKeyState(VK_SPACE) == KeyState::pressed &&
		prevKbState.GetKeyState(VK_SPACE) == KeyState::unpressed)
	{
		playerRef.FireBullet();
	}

	parent->GetGameLogic().Update(dt);

	prevKbState = keyboardState;
}

void ControlScreenState::Exit()
{
	parent->GetGameLogic().GetPlayer().StopSounds();
}

void ControlScreenState::Render()
{
	// Render the game
	parent->GetGameLogic().Render(false, false);

	// Render the background to grey out the game
	background.Draw(Vector2(0, 0), Vector2(WINDOW_WIDTH, WINDOW_HEIGHT), 0.17f);

	// Draw the titles
	Vector2 size = titleFont.GetTextSize("Controls");
	Vector2 textPos((WINDOW_WIDTH / 2) - (size.x / 2), (WINDOW_HEIGHT / 2) - (size.y / 2) - (WINDOW_HEIGHT / 4));
	titleFont.DrawString("Controls", textPos);


	// Draw all the controls strings
	// Set the y coord of the first string
	float sampleHeight = subFont1.GetTextSize("1").y;
	int iterations = sizeof(controlStrings) / sizeof(std::string);
	textPos.y = (WINDOW_HEIGHT / 2) - (sampleHeight * (static_cast<float>(iterations) / 2 + 4));
	for(int i = 0; i < iterations; ++i)
	{
		size = subFont1.GetTextSize(controlStrings[i]);
		textPos.x = (WINDOW_WIDTH / 2) - (size.x / 2);
		textPos.y += (size.y * 2);
		subFont1.DrawString(controlStrings[i], textPos);
	}

	// Print the message showing how to leave
	size = subFont1.GetTextSize("Press Esc or Return to go to Main Menu");
	textPos = Vector2(WINDOW_WIDTH - size.x - 7, WINDOW_HEIGHT - size.y - 7);
	subFont1.DrawString("Press Esc or Return to go to Main Menu", textPos);
}
