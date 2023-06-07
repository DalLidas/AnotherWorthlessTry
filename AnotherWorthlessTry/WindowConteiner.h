#pragma once

#include <memory>

#include "Window.h"
#include "Input/Keyboard/KeyboardClass.h"
#include "Input/Mouse/MouseClass.h"
#include "GFX/GFX.h"
#include "Physics/DotPhisics.h"
#include "Scene.h"

class WindowContainer {
public:
	WindowContainer();
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	Window window;
	KeyboardClass keyboard;
	MouseClass mouse;
	GFX gfx;
	DotPhysics physics;

	Scene scene;

private:
	
};
