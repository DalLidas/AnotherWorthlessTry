#include <Windows.h>
#include "Window.h"
#include "ExceptionLoger.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")


int CALLBACK wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPWSTR lpCmdLine,
    int nShowCmd)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    Window window;
    window.Initialize(hInstance, "MyWindow", "MyWindowClass", 600, 800);

    while (window.ProcessMessages());
    
	return 0;
}