#include "Engine.h"

BOOL Engine::Initialize(HINSTANCE hInstance, std::string windowName, std::string windowClass, INT width, INT height)
{
    //keyboard.EnableAutoRepeatChars();
    keyboard.DisableAutoRepeatChars();

    //keyboard.EnableAutoRepeatKeys();
    keyboard.DisableAutoRepeatKeys();

    if (!this->window.Initialize(this, hInstance, windowName, windowClass, width, height)) {
        return false;
    }

    if (!gfx.Initialize(window.GetHWND(), width, height)) {
        return false;
    }

    return true;

}

BOOL Engine::ProcessMessages()
{
    return  this->window.ProcessMessages();
}

void Engine::Update()
{

#ifdef INPUT_DEBUG_MSG
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

    /*
    while (!mouse.EventBufferIsEmpty()) {
        MouseEvent me = mouse.ReadEvent();

        std::string outmsg = "";
        outmsg += "(X:" + std::to_string(me.GetPosX()) + "||";
        outmsg += "Y:" + std::to_string(me.GetPosX()) + ")\n";

        OutputDebugStringA(outmsg.c_str());
    }
    */

    while (!mouse.EventBufferIsEmpty()) {
        MouseEvent me = mouse.ReadEvent();

        if (me.GetType() == MouseEvent::EventType::RAW_MOVE) {
            std::string outmsg = "";
            outmsg += "(X:" + std::to_string(me.GetPosX()) + "||";
            outmsg += "Y:" + std::to_string(me.GetPosX()) + ")\n";

            OutputDebugStringA(outmsg.c_str());
        }

    }

#endif // DEBUG


    
    
}

void Engine::RenderFrame()
{
    gfx.RenderFrame();
}
