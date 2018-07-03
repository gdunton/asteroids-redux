//-----------------------------------------------------------
// File: Window.cpp
//-----------------------------------------------------------

#include "Window.h"

#include "../GameMain/Game.h"
#include "../resource.h"
#include "../Debugging/Error.h"
#include "../Utilities/Strings.h"

Window::Window(HINSTANCE hInstance, int clientWidth, int clientHeight,
               const std::string& windowTitle, bool recordSession)
{
	this->clientWidth = clientWidth;
	this->clientHeight = clientHeight;

	// Fill out class description
	WNDCLASSEX wc;
	ZeroMemory( &wc, sizeof( wc ) );
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = CallbackWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE( IDI_ICON1 ));
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.lpszClassName = L"GameWindow";
	wc.lpszMenuName = nullptr;

	const HRESULT hr = RegisterClassEx(&wc);
	ASSERT( hr != E_FAIL );

	RECT wr = { 0, 0, clientWidth, clientHeight };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, false);

	windowWidth = wr.right - wr.left;
	windowHeight = wr.bottom - wr.top;

	// Get the window title in correct format
	std::wstring windowTitleLocal = StringToWString(windowTitle);

	// Create window
	windowHandle = CreateWindow( L"GameWindow", windowTitleLocal.c_str(),
		(WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX),
		CW_USEDEFAULT, CW_USEDEFAULT,
		windowWidth, windowHeight,
		nullptr, nullptr, hInstance, this);

	ASSERT( windowHandle );

	game = std::make_unique<Game>(this, std::make_unique<GraphicsDeviceManager>(this), recordSession);
	game->Initialize();

	// Show window
	ShowWindow(windowHandle, SW_SHOW);
	UpdateWindow(windowHandle);
}

int Window::Run()
{
	MSG msg = { nullptr };

	// Message cycle
	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			game->Update(GetKeyboardState());
		}
	}

	return static_cast<int>(msg.wParam);
}

void Window::Close()
{
	DestroyWindow(windowHandle);
}

KeyboardState Window::GetKeyboardState() const
{
	return keyboard.GetKeyboardState();
}

LRESULT Window::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CLOSE:
		// Recieve the close message and then the game must call the close on the window
		game->Close();
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

	return DefWindowProc(windowHandle, message, wParam, lParam);
}

LRESULT CALLBACK Window::CallbackWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static Window* app = nullptr;

	if(message == WM_CREATE)
	{
		const auto cs = reinterpret_cast<CREATESTRUCT*>(lParam);
		app = static_cast<Window*>(cs->lpCreateParams);
		return 0;
	}

	if(app)
	{
		return app->WndProc(message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
