//-----------------------------------------------------------
// File: Window.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "Window.h"

#include "../resource.h"

Window::Window()
{
	gameParent = nullptr;
	windowHandle = nullptr;

	windowWidth = 0;
	windowHeight = 0;

	clientWidth = 0;
	clientHeight = 0;
}

Window::~Window()
{
}

bool Window::Initialize( HINSTANCE hInstance, int clientWidth, int clientHeight,
                         std::string _windowTitle, IGame* _parent )
{
	clientWidth = clientWidth;
	clientHeight = clientHeight;
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
	std::wstring windowTitle;
	StringToWString( _windowTitle, windowTitle );

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

	return true;
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
				gameParent->VUpdate();
			}
		}
	}

	return (int)msg.wParam;
}

void Window::Close()
{
	DestroyWindow( windowHandle );
}

void Window::AddEventListener( const EventListener& _listener )
{
	events.push_front( _listener );
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
			gameParent->VClose();
		}
		else
		{
			// Unless there is no parent in which case the window closes
			Close();
		}
		return 0;

	default:
		break;
	}

	for( auto iter = events.begin(), end = events.end(); iter != end; ++iter )
	{
		if( message == iter->message )
		{
			iter->function( wParam, lParam );
			return 0;
		}
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