#include "MM_LFO.h"

void MM_LFO::init(uint8_t outAddr) {
	init(outAddr, 500000, square);
}

void MM_LFO::init(uint8_t outAddr, unsigned long cycleTime, LFOShapes shape)
{
	//TODO: clean up period/frequency conversions
	this->setAddr(outAddr);
	this->periodLength = cycleTime;
	this->frequency = 1.0 / this->periodLength;
	this->currentShape = shape;
	this->newShape = shape;
	this->periodMicros = micros();

	int half = LUT_SIZE / 2;
	for (int i = 0; i < LUT_SIZE; i++) {
		if (i < half) {
			squareTable[i] = 0;
			triangleTable[i] = map(i, 0, half, 0, MAX_DRIVE);
		}
		else {
			squareTable[i] = MAX_DRIVE;
			triangleTable[i] = map(i, half, LUT_SIZE, MAX_DRIVE, 0);
		}
		sawtoothTable[i] = map(i, 0, LUT_SIZE, 0, MAX_DRIVE);
	}
}

//Modulator method declarations

int MM_LFO::next(unsigned long micros) {

	unsigned long delta = micros - this->periodMicros;

	if (delta > this->periodLength) {		//figure out when a period is over and accept new shape at 0
		this->periodMicros = micros;
		delta = 0;
		this->currentShape = this->newShape;
	}

	int index = (int)floor(LUT_SIZE * (float)delta / (float)periodLength);
	switch (this->currentShape) {
	case square:
		return this->squareTable[index];
	case triangle:
		return this->triangleTable[index];
	case sawtooth:
		return this->sawtoothTable[index];
	case reverseSawtooth:
		return this->sawtoothTable[LUT_SIZE - index];
	default:
		//make it obvious something has gone wrong
		return 32500;
	}
}

volatile void MM_LFO::noteOn(uint8_t channel, uint8_t pitch, uint8_t velocity) {

}

void MM_LFO::noteOff(uint8_t channel, uint8_t pitch, uint8_t velocity) {

}

ModulatorTypes MM_LFO::getType() {
	return LFO;
}

//Frequency - make subdivistion enum with rate conversion
void MM_LFO::control1(int amt) {
	if (amt != 0) {
		this->frequency += ((double)amt * ADJ_RESOLUTION);
		this->periodLength = 1L / this->frequency;
		if (this->periodLength > MAX_PERIOD) {
			this->periodLength = MAX_PERIOD;
			this->frequency = 1.0 / this->periodLength;
		}
		else if (this->periodLength < MIN_PERIOD) {
			this->periodLength = MIN_PERIOD;
			this->frequency = 1.0 / this->periodLength;
		}
	}
}

//Shape - make shape enum
void MM_LFO::control2(int amt) {
	if (!this->shapeTick && amt != 0) {
		this->shapeTick = true;
		return;
	}
	if (amt > 0) {
		switch (this->currentShape) {
		case square:
			this->newShape = triangle;
			break;
		case triangle:
			this->newShape = sawtooth;
			break;
		case sawtooth:
			this->newShape = reverseSawtooth;
			break;
		}
		//this->periodMicros = micros();
		this->shapeTick = false;
	}
	else if (amt < 0) {
		switch (this->currentShape) {
		case triangle:
			this->newShape = square;
			break;
		case sawtooth:
			this->newShape = triangle;
			break;
		case reverseSawtooth:
			this->newShape = sawtooth;
			break;
		}
		this->shapeTick = false;
		//this->periodMicros = micros();
	}
}

//Clock source - Make enum (free, global clock, midi)
void MM_LFO::control3(int amt) {

}

//Retrigger
void MM_LFO::control4(int amt) {

}

//if possible level
void MM_LFO::control5(int amt) {

}

//None
void MM_LFO::control6(int amt) {

}

//None
void MM_LFO::control7(int amt) {

}

//nix
void MM_LFO::control8(int amt) {

}

int MM_LFO::getAddr() {
	return this->dacAddr;
}

void MM_LFO::setAddr(uint8_t addr) {
	this->dacAddr = addr;
}

String MM_LFO::getShape()
{
	if (this->currentShape == square) return String("Square");
	else if (this->currentShape == triangle) return String("Triangle");
	else if (this->currentShape == sawtooth) return String("Sawtooth");
	else return String("Reverse Sawtooth");
}
