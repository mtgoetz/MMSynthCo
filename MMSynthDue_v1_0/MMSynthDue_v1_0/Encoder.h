#pragma once
#include "Controls.h"
#include <stdint.h>
#include <Arduino.h>

class Encoder {
private:
	uint8_t pinA;
	uint8_t pinB;
	uint8_t buttonPin;

	uint8_t buttonState = LOW;
	int currentAState;
	int lastAState;
	unsigned long lastButtonPressMillis = 0;
	//buttonPressed = false;
public:
	Encoder(uint8_t pinA, uint8_t pinB, uint8_t buttonPin);
	int getUpdate();
};
