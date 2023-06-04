#pragma once

class KeyboardEvent {
public:
	enum EventType {
		Press,
		Realize,
		Invalide
	};

	KeyboardEvent();
	KeyboardEvent(const EventType type, const unsigned char key);
	bool IsPress() const;
	bool IsRealize() const;
	bool IsValide() const;
	unsigned char GetKeyCode() const;

private:
	EventType type;
	unsigned char key;
};