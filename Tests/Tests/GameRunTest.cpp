#include "catch.hpp"
#include <GameMain/Game.h>
#include <fstream>
#include "Utilities/Strings.h"

TEST_CASE("Integration test")
{
	auto graphics = std::make_unique<GraphicsDeviceManager>(nullptr, true);
	Game testGame(nullptr, std::move(graphics));
	testGame.Initialize();
}

TEST_CASE("Test run")
{
	auto graphics = std::make_unique<GraphicsDeviceManager>(nullptr, true);
	Game testGame(nullptr, std::move(graphics));
	testGame.Initialize();

	std::ifstream fileStream;
	fileStream.open(R"(..\Tests\Tests\run1.txt)");

	std::string currentLine;
	while(std::getline(fileStream, currentLine))
	{
		const auto iter = std::find(currentLine.begin(), currentLine.end(), '|');
		const auto parts = SplitString(currentLine, '|');

		const double deltaTime = std::stod(std::string(parts.at(0)));
		
		const KeyboardState keyboardState = [&parts]()
		{
			return parts.size() > 1 ? KeyboardState::FromString(std::string(parts.at(1))) : KeyboardState();
		}();

		testGame.InternalUpdate(deltaTime, keyboardState);
	}
}