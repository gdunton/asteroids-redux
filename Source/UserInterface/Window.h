//-----------------------------------------------------------
// File: Window.h
// Description: Class to create a window and manage the message loop.
//				Uses a reference to the game to call rendering and
//				update functions as well as the close function. 
//				EventListeners are used to pick out messages and
//				perform actions such as handle keyboard input
//-----------------------------------------------------------

#ifndef WINDOW_H
#define WINDOW_H

#include "../Input/Keyboard.h"

class Game;

class Window
{
public:

	Window(HINSTANCE hInstance, int clientWidth, int clientHeight,
	       const std::string& windowTitle, Game* _parent);
	int Run();
	void Close();

	KeyboardState GetKeyboardState() const;

	// Accessors
	HWND GetWindowHandle() { return windowHandle; }

	int GetClientWidth() const { return clientWidth; }
	int GetClientHeight() const { return clientHeight; }

private:
	// Windows' own procedure function
	LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);

	// Static function passes on messages to internal procedure
	static LRESULT CALLBACK CallbackWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


	// Reference to the parent game
	Game* gameParent = nullptr;
	HWND windowHandle = nullptr;

	// Window sized
	int windowWidth = 0;
	int windowHeight = 0;

	// Client area size
	int clientWidth = 0;
	int clientHeight = 0;

	Keyboard keyboard;
};

#endif
