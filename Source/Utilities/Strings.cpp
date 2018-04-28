#include "Strings.h"
#include <Windows.h>

std::wstring StringToWString(const std::string& str)
{
	const int slength = static_cast<int>(str.length()) + 1;
	const int length = MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, nullptr, 0);

	auto outWString = std::wstring(length, L'\0');
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, &outWString[0], length);
	return outWString;
}

std::string WStringToString(const std::wstring& wStr)
{
	const int slength = static_cast<int>(wStr.length()) + 1;
	const int length = WideCharToMultiByte(CP_ACP, 0, wStr.c_str(), slength, nullptr, 0, nullptr, nullptr);
	auto outString = std::string(length, '\0');
	WideCharToMultiByte(CP_ACP, 0, wStr.c_str(), slength, &outString[0], length, nullptr, nullptr);
	return outString;
}
