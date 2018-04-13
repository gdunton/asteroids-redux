//-----------------------------------------------------------
// File: STD.cpp
//-----------------------------------------------------------

#include "STD.h"

// Find the path to the assets directory
void GetAssetsDir(std::string& outString)
{
	static std::string s_assetPath = "";

	if( s_assetPath == "" )
	{
		WCHAR dir[MAX_PATH];
		GetCurrentDirectory( MAX_PATH, dir );

		std::wstring assetPath = dir;
		WStringToString( assetPath, s_assetPath );

		unsigned found = s_assetPath.find_last_of( "/\\" );
		s_assetPath.erase( found, s_assetPath.length() );
		s_assetPath += "\\Assets\\";
	}

	outString = s_assetPath;
}

// Converts strings from wide form to standard format and vice-versa
void StringToWString(const std::string& str, std::wstring& outWString)
{
	outWString = StringToWString(str);
}

std::wstring StringToWString(const std::string& str)
{
	const int slength = static_cast<int>(str.length()) + 1;
	const int length = MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, nullptr, 0);

	auto outWString = std::wstring(length, L'\0');
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, &outWString[0], length);
	return outWString;
}

void WStringToString(const std::wstring& wStr, std::string& outString)
{
	outString = WStringToString(wStr);
}

std::string WStringToString(const std::wstring& wStr)
{
	const int slength = static_cast<int>(wStr.length()) + 1;
	const int length = WideCharToMultiByte(CP_ACP, 0, wStr.c_str(), slength, nullptr, 0, nullptr, nullptr);
	auto outString = std::string(length, '\0');
	WideCharToMultiByte(CP_ACP, 0, wStr.c_str(), slength, &outString[0], length, nullptr, nullptr);
	return outString;
}

// Definitions of the random functions
float Random()
{
	return rand() / static_cast<float>(RAND_MAX);
}

float Random( const float min, const float max )
{
	return min + Random() * (max - min);
}

int RandomInt( const int min, const int max )
{
	return static_cast<int>(Random() * (max - min)) + min;
}