#include "Engine.h"

BOOL Engine::Initialize(HINSTANCE hInstance, std::string windowName, std::string windowClass, INT width, INT height)
{
    timer.Start();

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

    float dt = static_cast<float>(timer.GetMilisecondsElapsed());
    timer.Restart();

    while (!keyboard.CharBufferIsEmpty())
    {
        unsigned char ch = keyboard.ReadChar();
    }

    while (!keyboard.KeyBufferIsEmpty())
    {
        KeyboardEvent kbe = keyboard.ReadKey();
        unsigned char keycode = kbe.GetKeyCode();
    }

    while (!mouse.EventBufferIsEmpty())
    {
        MouseEvent me = mouse.ReadEvent();
        if (mouse.IsRightDown())
        {
            if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
            {
                this->gfx.camera.AdjustRotation((float)me.GetPosY() * 0.01f, (float)me.GetPosX() * 0.01f, 0);
            }
        }
    }

    const float cameraSpeed = 0.006f;

    if (keyboard.KeyIsPressed('W'))
    {
        this->gfx.camera.AdjustPosition(this->gfx.camera.GetForwardVector() * cameraSpeed * dt);
    }
    if (keyboard.KeyIsPressed('S'))
    {
        this->gfx.camera.AdjustPosition(this->gfx.camera.GetBackwardVector() * cameraSpeed * dt);
    }
    if (keyboard.KeyIsPressed('A'))
    {
        this->gfx.camera.AdjustPosition(this->gfx.camera.GetLeftVector() * cameraSpeed * dt);
    }
    if (keyboard.KeyIsPressed('D'))
    {
        this->gfx.camera.AdjustPosition(this->gfx.camera.GetRightVector() * cameraSpeed * dt);
    }
    if (keyboard.KeyIsPressed(VK_SPACE))
    {
        this->gfx.camera.AdjustPosition(0.0f, cameraSpeed * dt, 0.0f);
    }
    if (keyboard.KeyIsPressed(VK_SHIFT))
    {
        this->gfx.camera.AdjustPosition(0.0f, -cameraSpeed * dt, 0.0f);
    }

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

#endif // INPUT_DEBUG_MSG
}

void Engine::RenderFrame()
{
    gfx.RenderFrame();
}
