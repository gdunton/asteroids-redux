//-----------------------------------------------------------
// File: Main.cpp
// Description: Windows starting point for the program
//				Creates and intializes a game and runs it
//-----------------------------------------------------------

#include "../UserInterface/Window.h"
#include "Strings.h"

#ifdef RELEASE
#define TIME_SEEDED_RANDOM
#endif

#ifdef TIME_SEEDED_RANDOM
	#include <time.h>
#endif

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/ )
{
#ifdef TIME_BASED_RANDOM
    // seed the randon number funtion using the start time
	srand( time(NULL) );
#endif

	const std::string commandLine = GetCommandLineA();
	const auto args = SplitString(commandLine, ' ');

	bool recordSession = false;
	for (const auto& arg : args)
	{
		if (arg == "--record")
			recordSession = true;
	}		

	Window window(hInstance, 800, 600, "Asteroids Redux", recordSession);

	return window.Run();
}