//-----------------------------------------------------------
// File: STD.h
// Description: Precompiled header for all of the program.
//				Adds the headers that are needed in all the program
//				as well as some functions used everywhere
//-----------------------------------------------------------

#ifndef STD_H
#define STD_H

#include <Windows.h>
#define WIN32_LEAN_AND_MEAN

#include <wrl.h>
using Microsoft::WRL::ComPtr;

#ifdef DEBUG
//#define PHYSICS_DEBUG_INFO
#endif

// STL headers and typedefs
#include <memory>
#include <list>
#include <map>
#include <vector>
#include <string>
#include <cmath>

// DirectX headers
#include <d3d11.h>
#include <SimpleMath.h>

using Vector2 = DirectX::SimpleMath::Vector2;
using Matrix = DirectX::SimpleMath::Matrix;
using Color = DirectX::XMFLOAT4;

inline Color ColorRGBA(float r, float g, float b, float a) { return DirectX::XMFLOAT4(r, g, b, a); }

// Define color constants
constexpr Color GREEN = Color(255, 0, 255, 255);
constexpr Color BLACK = Color(0, 0, 0, 255);
constexpr Color WHITE = Color(255, 255, 255, 255);
constexpr Color RED = Color(255, 0, 0, 255);
constexpr Color BLUE = Color(0, 0, 255, 255);

// Commonly used header throughout the program
#include "../Debugging/Error.h"
#include "templates.h"
#include "Interfaces.h"
#include "MathTypes.h"

// Random functions
float Random();
float Random(float min, float max);
int RandomInt(int min, int max);

// Constants
const float PI = 3.14159265358979323f;

// Function to convert values to strings
#include <sstream>

inline void to_String(const double value, const int precision, std::string& outString)
{
	std::ostringstream out;
	out.precision(precision);
	out << value;
	outString = std::string(out.str());
}

// Enum for setting the difficulty
enum Difficulty
{
	EASY,
	NORMAL,
	HARD,
	EXTREME
};


// function to get the assets dir
void GetAssetsDir(std::string& outString);

// String converter -- MAX LENGTH IS FILE LENGTH
void StringToWString(const std::string& str, std::wstring& outWString);
std::wstring StringToWString(const std::string& str);
void WStringToString(const std::wstring& wStr, std::string& outString);
std::string WStringToString(const std::wstring& wStr);

#endif
