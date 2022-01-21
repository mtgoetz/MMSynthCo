#include "Button.h"

Button::Button(uint8_t pin)
{
	this->pin = pin;
	pinMode(pin, INPUT_PULLUP);
}

bool Button::pressed()
{
	bool pressed = false;
	int reading = digitalRead(this->pin);

	if (reading != this->lastReading) {
		this->lastDebounceTime = millis();
	}

	if ((millis() - this->lastDebounceTime) > debounceDelay && reading != this->buttonState) {
		this->buttonState = reading;
	}
	this->lastReading = reading;
	return this->buttonState == LOW;
}
