#pragma once
#include "notemap.h"
#include "Modulator.h"

class MM_Note : virtual public Modulator {

private:

	uint8_t dacAddr = 0;

	// notetracker knows which note ons & offs we have seen.
	// We refer to it when it's time to generate CV and gate signals,
	notetracker themap;

	// Variables for arpeggiator clock.
	uint32_t tempo_delay = 10;
	volatile bool send_tick = false;

	// The last bend records the most recently seen bend message.
	// We need to keep track so we can update note CV when we get new notes,
	// or new bend messages - we need the other half in order to put them together.
	// bend is signed, 14-bit
	volatile int16_t last_bend = 0;

	// constants to describe the MIDI input.
	// NUM_KEYS is the number of keys we're interpreting
	// BASE_KEY is the offset of the lowest key number
	static const int8_t NUM_KEYS = 49;
	static const int8_t BASE_KEY = 36;

	// The tuning constant - representing the DAC conts per semitone
	//
	// Arrived at using: (((2^<DAC resolution>)/5)/12) * 100
	//
	// 2^12 = 4096 total DAC counts.
	// 4096/5 = 819.2 DAC counts per volt on a 5V supply
	// 819.2/12 = dac counts per semitone = 68.26
	// times 100 for some extra calculation precision = 6826
	static const uint32_t DAC_CAL = 109227;

	uint32_t fineTune;

public:

	void init(uint8_t outAddr);

	//Modulator method declarations
	int next();
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
