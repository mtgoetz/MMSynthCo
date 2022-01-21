#pragma once
#include "Controls.h"

class Button {

private:
	uint8_t pin;
	unsigned long lastDebounceTime = 0;
	int lastReading;
	int buttonState = HIGH;
public:
	Button(uint8_t pin);
	bool pressed();
};
