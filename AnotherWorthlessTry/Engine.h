#pragma once

#define DEBUG

#include "WindowConteiner.h"
#include "Input/Keyboard/KeyboardClass.h"

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
    void Update();

private:

};