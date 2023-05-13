#pragma once
//still need these???     *****
#include <stdint.h>
#include <Arduino.h>
#include "Modulator.h"
#include "Constants.h"
#include <string>
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

//waveforms 0 -> off, 1 -> saw, 2 -> triangle, 3 -> sin, 4 -> square [0,4]
enum LFOShapes { off, saw, triangle, sine, square, size };


class MM_LFO : virtual public Modulator
{
private:
	//waveforms 0 -> off, 1 -> saw, 2 -> triangle, 3 -> sin, 4 -> square [0,4]
	enum SyncModes { free, sync };		//todo -> make this a control ***
	ClockSources clockSource = ClockSources::cs_free;

	lfo* thunderzLfo;
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
	void init(uint8_t outAddr, float frequency, LFOShapes shape);
	String getShape();
	float getBPM();
	void setClockSource(ClockSources source);
	String getClockSource();

	//Modulator method declarations
	int next(unsigned long micros);
	void noteOn(uint8_t channel, uint8_t pitch, uint8_t velocity);
	void noteOff(uint8_t channel, uint8_t pitch, uint8_t velocity);
	bool control1(int amt);
	bool control2(int amt);
	bool control3(int amt);
	bool control4(int amt);
	bool control8(int amt);
	bool control5(int amt);
	bool control6(int amt);
	bool control7(int amt);
	int getAddr();
	void setAddr(uint8_t addr);
	void setBPM(float bpm);
	ModulatorTypes getType();

	//string& getControl1Name();
	//std::string getControl2Name();
	//std::string getControl3Name();
	//std::string getControl4Name();
	//std::string getControl5Name();
	//std::string getControl6Name();
	//std::string getControl7Name();
	//String getControl8Name();

	void getControl1Val(char* buffer);
	void getControl2Val(char* buffer);
	void getControl3Val(char* buffer);
	void getControl4Val(char* buffer);
	void getControl5Val(char* buffer);
	void getControl6Val(char* buffer);
	void getControl7Val(char* buffer);
	void getControl8Val(char* buffer);

};
