#ifndef STRINGS_H
#define STRINGS_H

#include <string>

// String converter -- MAX LENGTH IS FILE LENGTH
std::wstring StringToWString(const std::string& str);
std::string WStringToString(const std::wstring& wStr);

#endif