#pragma once
#include "notemap.h"
#include "Modulator.h"
#include <string>

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
	int next(unsigned long micros);
	virtual void noteOn(uint8_t channel, uint8_t pitch, uint8_t velocity);
	virtual void noteOff(uint8_t channel, uint8_t pitch, uint8_t velocity);
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

	//std::string getControl1Val();
	//std::string getControl2Val();
	//std::string getControl3Val();
	//std::string getControl4Val();
	//std::string getControl5Val();
	//std::string getControl6Val();
	//std::string getControl7Val();
	//std::string getControl8Val();
};
