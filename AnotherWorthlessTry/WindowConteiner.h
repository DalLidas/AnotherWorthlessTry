#pragma once

#include "Window.h"
#include "Input/Keyboard/KeyboardClass.h"
#include "Input/Keyboard/KeyboardEvent.h"

class WindowContainer {
public:
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	Window window;
	KeyboardClass keyboard;

private:

};
