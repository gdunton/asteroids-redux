//-----------------------------------------------------------
// File: WindowEventListener.h
// Description: Structure used by the window to pick out windows
//				messages and invoke required callback
//-----------------------------------------------------------

#ifndef WINDOWSEVENTLISTENER_H
#define WINDOWSEVENTLISTENER_H

typedef void (*callback_function)(WPARAM, LPARAM);

struct EventListener
{
	// WM_XXXX type message that is added to the messageProc function
	UINT message;
	// Delegate function called when the message is received
	callback_function function;

	EventListener( const UINT _message, const callback_function _function )
	{
		message = _message;
		function = _function;
	}
};

#endif