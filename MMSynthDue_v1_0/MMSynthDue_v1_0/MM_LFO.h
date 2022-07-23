#pragma once
//still need these???     *****
#include <stdint.h>
#include <Arduino.h>
#include "Modulator.h"
#include "Constants.h"

#include "lfo.h"

//#define MAX_PERIOD 10000000 //10 seconds
//#define MIN_PERIOD 16666
//#define LUT_SIZE 1000
#define ADJ_RESOLUTION 0.5	//in bpm land
//#define SHAPES_SIZE 4


//TODO *******
//fix upper overflow
//fix adjustment - to much range, too little at low, too much at high (slow)
//decrease shape change sensitivity - require 2 clicks


class MM_LFO : virtual public Modulator
{

private:
	enum LFOShapes { off, triangle, sine, square, saw, size };
	enum SyncModes { free, sync };		//todo -> make this a control ***
	ClockSources clockSource = ClockSources::cs_free;

	lfo *thunderzLfo;
	//int frequency = 30;
	bool retriggerMode = false;
	uint8_t dacAddr = 0;
	LFOShapes currentShape = LFOShapes::triangle;
	//LFOShapes newShape = LFOShapes::triangle;
	//bool shapeTick = false;
	//uint16_t squareTable[LUT_SIZE];
	//uint16_t triangleTable[LUT_SIZE];
	//uint16_t sawtoothTable[LUT_SIZE];
	//unsigned long periodLength = 1000000;		//todo, not sure if we need this calculation anymore
	float bpm = 120;
	float frequency = bpm / 60;
	//unsigned long periodMicros = 0;
	void setFrequency(float frequency);

public:
	void init(uint8_t outAddr);
	void init(uint8_t outAddr, unsigned int frequency, LFOShapes shape);
	String getShape();
	float getBPM();
	void setClockSource(ClockSources source);
	int getClockSource();

	//Modulator method declarations
	int next(unsigned long micros);
	void noteOn(uint8_t channel, uint8_t pitch, uint8_t velocity);
	void noteOff(uint8_t channel, uint8_t pitch, uint8_t velocity);
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
	void setBPM(float bpm);
	ModulatorTypes getType();
};
