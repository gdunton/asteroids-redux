//-----------------------------------------------------------
// File: STD.cpp
//-----------------------------------------------------------

#include "STD.h"

// Find the path to the assets directory
void GetAssetsDir( String& outString )
{
	static String s_assetPath = "";

	if( s_assetPath == "" )
	{
		WCHAR dir[MAX_PATH];
		GetCurrentDirectory( MAX_PATH, dir );

		WString assetPath = dir;
		WStringToString( assetPath, s_assetPath );

		unsigned found = s_assetPath.find_last_of( "/\\" );
		s_assetPath.erase( found, s_assetPath.length() );
		s_assetPath += "\\Assets\\";
	}

	outString = s_assetPath;
}

// Converts strings from wide form to standard format and vice-versa
void StringToWString( const String& string, WString& outWString )
{
	int len;
	int slength = (int)string.length() + 1;
	len = MultiByteToWideChar( CP_ACP, 0, string.c_str(), slength, 0, 0 );

	outWString = WString(len, L'\0');
	MultiByteToWideChar( CP_ACP, 0, string.c_str(), slength, &outWString[0], len );
}

void WStringToString( const WString& wstring, String& outString )
{
	int len;
	int slength = (int)wstring.length() + 1;
	len = WideCharToMultiByte( CP_ACP, 0, wstring.c_str(), slength, 0, 0, 0, 0 );
	outString = String(len, '\0');
	WideCharToMultiByte( CP_ACP, 0, wstring.c_str(), slength, &outString[0], len, 0, 0 );
}

// Definitions of the random functions
float Random()
{
	return (rand() / (float)RAND_MAX);
}

float Random( const float min, const float max )
{
	return min + Random() * (max - min);
}

int RandomInt( const int min, const int max )
{
	return (int)(Random() * (max - min)) + min;
}