#include "Engine.h"

BOOL Engine::Initialize(HINSTANCE hInstance, std::string windowName, std::string windowClass, INT width, INT height)
{
    return this->window.Initialize(this, hInstance, windowName, windowClass, width, height);
}

BOOL Engine::ProcessMessages()
{
    return  this->window.ProcessMessages();
}
