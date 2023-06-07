#include "Engine.h"

//C4324 - Normal warnin padding 
//

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    Engine engine;
    engine.Initialize(hInstance, "MyEngine", "MyEngineClass", 800, 600);

    while (engine.ProcessMessages()) {
        engine.Update();
        engine.RenderFrame();
    }
    
	return 0;
}