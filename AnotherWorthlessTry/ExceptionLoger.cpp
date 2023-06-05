#include "ExceptionLoger.h"
#include <comdef.h>

void ExceptionLoger::ExceptionCall(std::string message)
{
	std::string exceptionMessage = "Exeption: " + message;
	MessageBoxA(NULL, exceptionMessage.c_str(), "Error", NULL);
}

void ExceptionLoger::ExceptionCall(std::wstring message)
{
	std::wstring exceptionMessage = L"Exeption: " + message;
	MessageBoxW(NULL, exceptionMessage.c_str(), L"Error", NULL);
}

void ExceptionLoger::ExceptionCall(HRESULT hr, std::string message)
{
	_com_error exception(hr);
	std::wstring exceptionMessage = L"Exeption: " + StringConverter::StringToWide(message) + L"\n\n" + exception.ErrorMessage();
	MessageBoxW(NULL, exceptionMessage.c_str(), L"Error", NULL);
}

void ExceptionLoger::ExceptionCall(HRESULT hr, std::wstring message)
{
	_com_error exception(hr);
	std::wstring exceptionMessage = L"Exeption: " + message + L"\n\n" + exception.ErrorMessage();
	MessageBoxW(NULL, exceptionMessage.c_str(), L"Error", NULL);
}
