#include "Encoder.h"

Encoder::Encoder(uint8_t pinA, uint8_t pinB, uint8_t buttonPin)
{
	this->pinA = pinA;
	this->pinB = pinB;
	this->buttonPin = buttonPin;
	pinMode(pinA, INPUT_PULLUP);
	pinMode(pinB, INPUT_PULLUP);
	pinMode(buttonPin, INPUT_PULLUP);

	this->lastAState = digitalRead(pinA);
}

int Encoder::getUpdate()
{
	int buttonState = digitalRead(this->buttonPin);

	this->currentAState = digitalRead(this->pinA);

	int updateAmount = 0;

	if (this->currentAState != this->lastAState && this->currentAState == 1) {

		if (buttonState == LOW) {
			if (millis() - this->lastButtonPressMillis > debounceDelay) {
				updateAmount = ENCODER_BASE_ADJ * ENCODER_PRESSED_MULT;
			}
			else {
				this->lastButtonPressMillis = millis();
			}
		}
		else {
			updateAmount = ENCODER_BASE_ADJ;
		}

		if (digitalRead(pinB) != this->currentAState) {
			updateAmount = -updateAmount;
		}
	}

	this->lastAState = this->currentAState;
	return updateAmount;
}
