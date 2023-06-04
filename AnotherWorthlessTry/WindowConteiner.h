#pragma once

#include "Window.h"

class WindowContainer {
public:
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	Window window;

private:

};
