#include "StringConverter.h"

std::wstring StringConverter::StringToWide(std::string str)
{
    std::wstring wStr(str.begin(), str.end());
    return wStr;
}
