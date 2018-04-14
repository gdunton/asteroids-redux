//-----------------------------------------------------------
// File: Window.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "Window.h"

#include "../GameMain/Game.h"
#include "../resource.h"

Window::Window( HINSTANCE hInstance, int clientWidth, int clientHeight,
                         const std::string& _windowTitle, Game* _parent )
{
	this->clientWidth = clientWidth;
	this->clientHeight = clientHeight;
	gameParent = _parent;

	// Fill out class description
	WNDCLASSEX wc;
	ZeroMemory( &wc, sizeof( wc ) );
	wc.cbSize = sizeof( WNDCLASSEX );
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = Window::CallbackWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_ICON1 ) );
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor( nullptr, IDC_ARROW );
	wc.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );
	wc.lpszClassName = L"GuyWindow";
	wc.lpszMenuName = nullptr;

	HRESULT hr = RegisterClassEx( &wc );
	ASSERT( hr != E_FAIL );

	RECT wr = { 0, 0, clientWidth, clientHeight };
	AdjustWindowRect( &wr, WS_OVERLAPPEDWINDOW, false );

	windowWidth = wr.right - wr.left;
	windowHeight = wr.bottom - wr.top;

	// Get the window title in correct format
	std::wstring windowTitle = StringToWString( _windowTitle);

	// Create window
	windowHandle = CreateWindow( L"GuyWindow", windowTitle.c_str(), 
		(WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX),
		CW_USEDEFAULT, CW_USEDEFAULT, 
		windowWidth, windowHeight, 
		nullptr, nullptr, hInstance, this);

	ASSERT( windowHandle );

	// Show window
	ShowWindow( windowHandle, SW_SHOW );
	UpdateWindow( windowHandle );
}

int Window::Run()
{
	MSG msg = {nullptr};

	// Message cycle
	while( msg.message != WM_QUIT )
	{
		if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			if( gameParent )
			{
				gameParent->Update();
			}
		}
	}

	return static_cast<int>(msg.wParam);
}

void Window::Close()
{
	DestroyWindow( windowHandle );
}

KeyboardState Window::GetKeyboardState() const
{
	return keyboard.GetKeyboardState();
}

LRESULT Window::WndProc( UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CLOSE:
		if( gameParent )
		{
			// Recieve the close message and then the game must call the close on the window
			gameParent->Close();
		}
		else
		{
			// Unless there is no parent in which case the window closes
			Close();
		}
		return 0;

	case WM_KEYDOWN:
		keyboard.SetKeyState(wParam, KeyState::pressed);
		break;

	case WM_KEYUP:
		keyboard.SetKeyState(wParam, KeyState::unpressed);
		break;

	default:
		break;
	}

	return DefWindowProc( windowHandle, message, wParam, lParam );
}

LRESULT CALLBACK Window::CallbackWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	static Window* app = nullptr;

	if( message == WM_CREATE )
	{
		CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
		app = (Window*)cs->lpCreateParams;
		return 0;
	}

	if( app )
	{
		return app->WndProc( message, wParam, lParam );
	}
	else
	{
		return DefWindowProc( hWnd, message, wParam, lParam );
	}
}