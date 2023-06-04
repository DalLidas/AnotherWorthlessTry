#pragma once
#include <windows.h>
#include "ExceptionLoger.h"

class WindowContainer;

class Window {
public:
    BOOL Initialize(
        WindowContainer* pWindowContainer,
        HINSTANCE hInstance,
        std::string windowName,
        std::string windowClass,
        INT width,
        INT height
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

    INT width = 0;
    INT height = 0;
};