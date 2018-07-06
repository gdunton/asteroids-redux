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

std::vector<std::string_view> SplitString(std::string_view text, char separator)
{
	std::vector<std::string_view> results;

	while(!text.empty())
	{
		const auto until = text.find(separator);

		results.emplace_back(text.substr(0, until));

		if(until == std::string_view::npos)
		{
			break;
		}

		text.remove_prefix(until + 1);
	}

	return results;
}
