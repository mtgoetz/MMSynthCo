#pragma once
#include <stdint.h>
#include <Arduino.h>
#include "Modulator.h"
#include "Constants.h"

#define MAX_PERIOD 10000000 //10 seconds
#define MIN_PERIOD 16666
#define LUT_SIZE 1000
#define ADJ_RESOLUTION 0.0000005
#define SHAPES_SIZE 4


//TODO *******
//fix upper overflow
//fix adjustment - to much range, too little at low, too much at high (slow)
//decrease shape change sensitivity - require 2 clicks


class MM_LFO : virtual public Modulator
{

private:
	enum LFOShapes { square, triangle, sawtooth, reverseSawtooth };
	uint8_t dacAddr = 0;
	LFOShapes currentShape = triangle;
	LFOShapes newShape = triangle;
	bool shapeTick = false;
	//uint16_t output = 0;
	uint16_t squareTable[LUT_SIZE];
	uint16_t triangleTable[LUT_SIZE];
	uint16_t sawtoothTable[LUT_SIZE];
	unsigned long periodLength = 1000000;
	double frequency = 1 / periodLength;
	unsigned long periodMicros = 0;
	
	int next(unsigned long micros);

public:
	void init(uint8_t outAddr);
	void init(uint8_t outAddr, unsigned long frequency, LFOShapes shape);
	String getShape();

	//Modulator method declarations
	int next();
	//todo fix volatile
	virtual volatile void noteOn(uint8_t channel, uint8_t pitch, uint8_t velocity);
	virtual void noteOff(uint8_t channel, uint8_t pitch, uint8_t velocity);
	void control1(int amt);
	void control2(int amt);
	void control3(int amt);
	void control4(int amt);
	void control8(int amt);
	void control5(int amt);
	void control6(int amt);
	void control7(int amt);
	int getAddr();
	void setAddr(uint8_t addr);
	ModulatorTypes getType();
};
