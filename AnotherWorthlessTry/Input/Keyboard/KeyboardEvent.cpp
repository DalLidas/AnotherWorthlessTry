#include "KeyboardEvent.h"

KeyboardEvent::KeyboardEvent() :
	type(EventType::Invalide),
	key(0u)
{
}

KeyboardEvent::KeyboardEvent(const EventType type, const unsigned char key):
	type(type),
	key(key)
{
}

bool KeyboardEvent::IsPress() const
{
	return this->type == EventType::Press;
}

bool KeyboardEvent::IsRealize() const
{
	return this->type == EventType::Realize;
}

bool KeyboardEvent::IsValide() const
{
	return this->type == EventType::Invalide;
}

unsigned char KeyboardEvent::GetKeyCode() const
{
	return this->key;
}


