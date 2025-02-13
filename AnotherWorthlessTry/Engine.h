#pragma once

//for input debug msg 
//#define INPUT_DEBUG_MSG

#include "WindowConteiner.h"
#include "Input/Keyboard/KeyboardClass.h"

#include "BindImGui.h"

class Engine : WindowContainer{
public:
    BOOL Initialize(
        HINSTANCE hInstance,
        std::string windowName,
        std::string windowClass,
        INT width,
        INT height
    );

    BOOL ProcessMessages();
    void Update(BindMSG& imGuiMsg);
    void RenderFrame(BindMSG& imGuiMsg);

    BindMSG imGuiMsg{};

private:
    Timer timer;
};