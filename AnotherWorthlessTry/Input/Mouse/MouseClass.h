#pragma once
#include "MouseEvent.h"
#include <queue>

class MouseClass{
public:
	void OnLeftPressed(int x_, int y_);
	void OnLeftReleased(int x_, int y_);
	void OnRightPressed(int x_, int y_);
	void OnRightReleased(int x_, int y_);
	void OnMiddlePressed(int x_, int y_);
	void OnMiddleReleased(int x_, int y_);
	void OnWheelUp(int x_, int y_);
	void OnWheelDown(int x_, int y_);
	void OnMouseMove(int x_, int y_);
	void OnMouseMoveRaw(int x_, int y_);

	bool IsLeftDown();
	bool IsMiddleDown();
	bool IsRightDown();

	int GetPosX();
	int GetPosY();
	MousePoint GetPos();

	bool EventBufferIsEmpty();
	MouseEvent ReadEvent();

private:
	std::queue<MouseEvent> eventBuffer;
	bool leftIsDown = false;
	bool rightIsDown = false;
	bool mbuttonDown = false;
	int x = 0;
	int y = 0;
};