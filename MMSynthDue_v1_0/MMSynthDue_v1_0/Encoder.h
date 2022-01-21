#pragma once
#include "Controls.h"
#include <stdint.h>
#include <Arduino.h>

static const uint8_t ENC_STATES[] = { 0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0 };

class Encoder {

private:
	uint8_t pinA;
	uint8_t pinB;
	uint8_t buttonPin;
	uint8_t buttonState = LOW;
	int currentAState;
	int lastAState;
	unsigned long lastButtonPressMillis = 0;
	
public:
	Encoder(uint8_t pinA, uint8_t pinB, uint8_t buttonPin);
	int getUpdate();
};