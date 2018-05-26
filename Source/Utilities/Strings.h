#ifndef STRINGS_H
#define STRINGS_H

#include <string>
#include <string_view>
#include <vector>

// String converter -- MAX LENGTH IS FILE LENGTH
std::wstring StringToWString(const std::string& str);
std::string WStringToString(const std::wstring& wStr);

std::vector<std::string_view> SplitString(std::string_view text, char separator);

#endif