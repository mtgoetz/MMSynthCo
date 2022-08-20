#pragma once
#include <stdint.h>
#include "Modulator.h"
#include <Arduino.h>
#include "Controls.h"
#include "Constants.h"

#define ADSR_TABLE_SIZE 1024			// number of time points
#define ATTACK_ALPHA 0.995				// varies between 0.9 (steep curve) and 0.9995 (straight line)
#define ATTACK_DECAY_RELEASE 0.997		// fits to ARRAY_SIZE 1024
#define MAX_LENGTH 10000000				//10 seconds from microseconds
#define TIME_CONTROL_MULT (MAX_LENGTH / 100)
#define DAC_CONTROL_MULT (MAX_DRIVE / 100)

class MM_ADSR : virtual public Modulator
{
private:
	static int attackTable[ADSR_TABLE_SIZE];
	static int decayReleaseTable[ADSR_TABLE_SIZE];
	static bool initialized;

	bool loopMode = false;
	bool inverted = false;
	uint8_t dacAddr = 0;

	//default values
	unsigned long attack = 500000;		// 0 to 20 sec
	unsigned long decay = 500000;		// 1ms to 60 sec
	int sustain = 40000;				// 0 to 65535
	unsigned long release = 500000;		// 1ms to 60 sec

	// time stamp for note on and note off
	unsigned long noteOnMicros = 0;
	unsigned long noteOffMicros = 0;

	// internal values needed to transition to new pulse (attack) and to release at any point in time
	uint16_t output;
	uint16_t decayStart;
	uint16_t releaseStart;
	uint16_t attackStart;
	uint16_t notesPressedCount = 0;
	uint16_t gain = MAX_DRIVE;

	void setAttack(unsigned long attack);
	void setDecay(unsigned long decay);
	void setSustain(int sustain);
	void setRelease(unsigned long release);
	//void setGain(int gain);

	void noteOn(unsigned long micros);
	void noteOff(unsigned long micros);

	//todo: remove?
	//int mapRange(int val);

	//public section method declarations
public:
	void init(uint8_t outAddr);
	void init(unsigned long attack, unsigned long decay, int sustain, unsigned long release, uint8_t outAddr);
	void init(unsigned long attack, unsigned long decay, int sustain, unsigned long release, uint8_t outAddr, int initial_gain, bool loopMode, bool invert);

	//ADSR unique operations
	void loopModeOff();
	void loopModeOn();
	bool toggleInversion();

	//Modulator method declarations
	int next(unsigned long micros);
	virtual void noteOn(uint8_t channel, uint8_t pitch, uint8_t velocity);
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
