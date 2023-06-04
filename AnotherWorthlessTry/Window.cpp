#include "Window.h"

BOOL Window::Initialize(
    HINSTANCE hInstance,
    std::string windowName,
    std::string windowClass, 
    INT height, INT width
)
{
    this->hInstance = hInstance;
    this->windowName = windowName;
    this->wWindowName = StringConverter::StringToWide(windowName);
    this->windowClass = windowClass;
    this->wWindowClass = StringConverter::StringToWide(windowClass);
    this->height = height;
    this->width = width;

    RegisterWindowClass();

    this->handle = CreateWindowEx(
        0,
        this->wWindowClass.c_str(),
        this->wWindowName.c_str(),
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        100,
        100,
        this->width,
        this->height,
        NULL,
        NULL,
        this->hInstance,
        nullptr
     );

    if (FAILED(this->handle)){
        ExceptionLoger::ExceptionCall(GetLastError(), "CreateWindowEx dont create windows class");
        return false;
    }

    ShowWindow(this->handle, SW_SHOW);
    SetForegroundWindow(this->handle);
    SetFocus(this->handle);

    return true;
}

Window::~Window() {
    if (SUCCEEDED(this->handle)) {
        UnregisterClass(this->wWindowClass.c_str(), this->hInstance);
        DestroyWindow(this->handle);
    }
}

void Window::RegisterWindowClass()
{
    WNDCLASSEX wc;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = DefWindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = this->hInstance;
    wc.hIcon = NULL;
    wc.hIconSm = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = this->wWindowClass.c_str();
    wc.cbSize = sizeof(WNDCLASSEX);

    RegisterClassEx(&wc);
}

HWND Window::GetHWND() const
{
    return handle;
}

BOOL Window::ProcessMessages() {
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG)); 

    while (PeekMessage(&msg, this->handle, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    }

    // Check if the window was closed
    if (msg.message == WM_NULL)
    {
        if (!IsWindow(this->handle))
        {
            this->handle = NULL; //Message processing loop takes care of destroying this window
            UnregisterClass(this->wWindowClass.c_str(), this->hInstance);
            return false;
        }
    }

    return true;
}