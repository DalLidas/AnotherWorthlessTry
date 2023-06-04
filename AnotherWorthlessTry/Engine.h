#pragma once

#include "WindowConteiner.h"

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

private:

};