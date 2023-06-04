#include "Engine.h"

BOOL Engine::Initialize(HINSTANCE hInstance, std::string windowName, std::string windowClass, INT width, INT height)
{
    //keyboard.EnableAutoRepeatChars();
    keyboard.DisableAutoRepeatChars();

    //keyboard.EnableAutoRepeatKeys();
    keyboard.DisableAutoRepeatKeys();

    return this->window.Initialize(this, hInstance, windowName, windowClass, width, height);
}

BOOL Engine::ProcessMessages()
{
    return  this->window.ProcessMessages();
}

void Engine::Update()
{

#ifdef DEBUG
    while (!keyboard.CharBufferIsEmpty()) {
        unsigned char ch = keyboard.ReadChar();
        std::string outmsg = "Char: ";
        outmsg += ch;
        outmsg += "\n";
        OutputDebugStringA(outmsg.c_str());
}

    while (!keyboard.KeyBufferIsEmpty()) {

        KeyboardEvent kbe = keyboard.ReadKey();

        unsigned char keyCode = kbe.GetKeyCode();

        std::string outmsg = "";
        if (kbe.IsPress()) {
            outmsg += "Key pressed: ";
        }
        if (kbe.IsRealize()) {
            outmsg += "Key realized: ";
        }

        outmsg += keyCode;
        outmsg += "\n";
        OutputDebugStringA(outmsg.c_str());
    }
#endif // DEBUG

    
}
