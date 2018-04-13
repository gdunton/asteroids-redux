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

#ifdef DEBUG
//#define PHYSICS_DEBUG_INFO
#endif

// STL headers and typedefs
#include <memory>
#include <list>
#include <map>
#include <vector>
#include <string>
typedef std::wstring WString;
typedef std::string String;
using std::list;
#include <cmath>

// DirectX headers
#include <d3d11.h>
#include <SimpleMath.h>

// The used vector with some operators
using Vector2 = DirectX::SimpleMath::Vector2;


// The typedefs for matrices and colors
typedef DirectX::SimpleMath::Matrix Matrix;
typedef DirectX::XMFLOAT4 Color;
inline Color ColorRGBA( float r, float g, float b, float a ) { return DirectX::XMFLOAT4(r, g, b, a); }
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
float Random( const float min, const float max );
int RandomInt( const int min, const int max );

// Constants
const float PI = 3.14159265358979323f;

// Function to convert values to strings
#include <sstream>
inline void to_String( const double value, const int precision, String& outString )
{
	std::ostringstream out;
	out.precision( precision );
	out << value;
	outString = String( out.str() );
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
void GetAssetsDir( String& outString );

// String converter -- MAX LENGTH IS FILE LENGTH
void StringToWString( const String& str, WString& outWString );
WString StringToWString(const String& str);
void WStringToString( const WString& wStr, String& outString );
String WStringToString( const WString& wStr );

#endif