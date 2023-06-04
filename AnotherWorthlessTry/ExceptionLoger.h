#pragma once
#include <Windows.h>
#include "StringConverter.h"

class ExceptionLoger {
public:
	static void ExceptionCall(std::string message);
	static void ExceptionCall(HRESULT hr, std::string message);
	static void ExceptionCall(HRESULT hr, std::wstring message);
};