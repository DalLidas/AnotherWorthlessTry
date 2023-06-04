#include "Engine.h"

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    Engine engine;
    engine.Initialize(hInstance, "MyEngine", "MyEngineClass", 800, 600);

    while (engine.ProcessMessages()) {
        engine.Update();
    }
    
	return 0;
}