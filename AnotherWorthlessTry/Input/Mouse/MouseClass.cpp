#include "MouseClass.h"

void MouseClass::OnLeftPressed(int x_, int y_)
{
	this->leftIsDown = true;
	MouseEvent me(MouseEvent::EventType::LPress, x_, y_);
	this->eventBuffer.push(me);
}

void MouseClass::OnLeftReleased(int x_, int y_)
{
	this->leftIsDown = false;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::LRelease, x_, y_));
}

void MouseClass::OnRightPressed(int x_, int y_)
{
	this->rightIsDown = true;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::RPress, x_, y_));
}

void MouseClass::OnRightReleased(int x_, int y_)
{
	this->rightIsDown = false;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::RRelease, x_, y_));
}

void MouseClass::OnMiddlePressed(int x_, int y_)
{
	this->mbuttonDown = true;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::MPress, x_, y_));
}

void MouseClass::OnMiddleReleased(int x_, int y_)
{
	this->mbuttonDown = false;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::MRelease, x_, y_));
}

void MouseClass::OnWheelUp(int x_, int y_)
{
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelUp, x_, y_));
}

void MouseClass::OnWheelDown(int x_, int y_)
{
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelDown, x_, y_));
}

void MouseClass::OnMouseMove(int x_, int y_)
{
	this->x = x_;
	this->y = y_;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::Move, x_, y_));
}

void MouseClass::OnMouseMoveRaw(int x_, int y_)
{
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::RAW_MOVE, x_, y_));
}

bool MouseClass::IsLeftDown()
{
	return this->leftIsDown;
}

bool MouseClass::IsMiddleDown()
{
	return this->mbuttonDown;
}

bool MouseClass::IsRightDown()
{
	return this->rightIsDown;
}

int MouseClass::GetPosX()
{
	return this->x;
}

int MouseClass::GetPosY()
{
	return this->y;
}

MousePoint MouseClass::GetPos()
{
	return{ this->x, this->y };
}

bool MouseClass::EventBufferIsEmpty()
{
	return this->eventBuffer.empty();
}

MouseEvent MouseClass::ReadEvent()
{
	if (this->eventBuffer.empty())
	{
		return MouseEvent();
	}
	else
	{
		MouseEvent e = this->eventBuffer.front(); //Get first event from buffer
		this->eventBuffer.pop(); //Remove first event from buffer
		return e;
	}
}