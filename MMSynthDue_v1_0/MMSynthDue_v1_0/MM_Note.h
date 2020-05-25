#pragma once

#include <stdint.h>
#include "Modulator.h"

class MM_Note : virtual public Modulator
{
private:

	uint8_t currentKey;
	uint8_t dacAddr;

	// The tuning constant - representing the DAC conts per semitone
	//
	// Arrived at using: (((2^<DAC resolution>)/5)/12) * 100
	//
	// 2^12 = 4096 total DAC counts.
	// 4096/5 = 819.2 DAC counts per volt on a 5V supply
	// 819.2/12 = dac counts per semitone = 68.26
	// times 100 for some extra calculation precision = 6826
	static const uint32_t DAC_CAL = 6826;

	// The last bend records the most recently seen bend message.
	// We need to keep track so we can update note CV when we get new notes,
	// or new bend messages - we need the other half in order to put them together.
	// bend is signed, 14-bit
	int last_bend = 0;

	//public section method declarations
public:
	void init();
	void init(uint8_t outAddr);
	void init(uint8_t initialKey, uint8_t outAddr);

	//Modulator method declarations
	int next();
	virtual void noteOn(uint8_t key);
	virtual void noteOff();
	void control1(int amt);
	void control2(int amt);
	void control3(int amt);
	void control4(int amt);
	void control5(int amt);
	void control6(int amt);
	void control7(int amt);
	void control8(int amt);
	int getAddr();
	void setAddr(uint8_t addr);
};
