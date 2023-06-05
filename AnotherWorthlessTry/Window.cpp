#include "WindowConteiner.h"

BOOL Window::Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, std::string windowName, std::string windowClass, INT width, INT height){
    this->hInstance = hInstance;
    this->windowName = windowName;
    this->wWindowName = StringConverter::StringToWide(windowName);
    this->windowClass = windowClass;
    this->wWindowClass = StringConverter::StringToWide(windowClass);
    this->width = width;
    this->height = height;

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
        pWindowContainer
     );

    if (this->handle == NULL){
        ExceptionLoger::ExceptionCall(GetLastError(), "CreateWindowEx dont create windows class");
        return false;
    }

    ShowWindow(this->handle, SW_SHOW);
    SetForegroundWindow(this->handle);
    SetFocus(this->handle);

    return true;
}

Window::~Window() {
    if (this->handle != NULL) {
        UnregisterClass(this->wWindowClass.c_str(), this->hInstance);
        DestroyWindow(this->handle);
    }
}

HWND Window::GetHWND() const{
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

LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        // All other messages
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;

    default:
    {
        // retrieve ptr to window class
        WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        // forward message to window class handler
        return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
    }
    }
}

LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
     switch (uMsg)
    {
    case WM_NCCREATE:
    {
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        WindowContainer* pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);
        if (pWindow == nullptr) //Sanity check
        {
            ExceptionLoger::ExceptionCall("Critical Error: Pointer to window container is null during WM_NCCREATE");
            exit(-1);
        }
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
        SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
        return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}


void Window::RegisterWindowClass(){
    WNDCLASSEX wc{0};
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = HandleMessageSetup;
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