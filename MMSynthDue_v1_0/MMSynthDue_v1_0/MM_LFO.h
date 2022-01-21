#pragma once
#include <stdint.h>
#include "Modulator.h"

class MM_LFO : virtual public Modulator
{
private:
	uint8_t dacAddr = 0;

	//public section method declarations
public:
	void init();
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
};
