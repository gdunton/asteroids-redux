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

#include "WindowsEventListener.h"

class Window
{
private:

	// Reference to the parent game
	IGame* gameParent;
	HWND windowHandle;

	// Window sized
	int windowWidth;
	int windowHeight;

	// Client area size
	int clientWidth;
	int clientHeight;

	// Windows messages to listen for with callbacks for when 
	// message is revieved
	list< EventListener > events;

public:
	Window();
	~Window();

	bool Initialize( HINSTANCE hInstance, int clientWidth, int clientHeight, 
		String windowTitle, IGame* _parent );
	int Run();
	void Close();

	void AddEventListener( const EventListener& _listener );

	// Accessors
	HWND GetWindowHandle() { return windowHandle; }

	int GetClientWidth() { return clientWidth; }
	int GetClientHeight() { return clientHeight; }

private:
	// Windows' own procedure function
	LRESULT WndProc( UINT message, WPARAM wParam, LPARAM lParam );

	// Static function passes on messages to internal procedure
	static LRESULT CALLBACK CallbackWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
};

#endif