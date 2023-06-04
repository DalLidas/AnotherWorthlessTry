#pragma once
#include <windows.h>
#include "StringConverter.h"
#include "ExceptionLoger.h"

class Window {
public:
    BOOL Initialize(
        HINSTANCE hInstance,
        std::string windowName,
        std::string windowClass,
        INT height,
        INT width 
    );

    ~Window();

    BOOL ProcessMessages();
    HWND GetHWND() const;

private:
    void RegisterWindowClass();

    HWND handle = NULL;
    HINSTANCE hInstance = NULL;

    std::string windowName = "";
    std::wstring wWindowName = L"";

    std::string windowClass = "";
    std::wstring wWindowClass = L"";

    INT height = 0;
    INT width = 0;
};