#pragma once
enum ModulatorTypes { Note, ADSR, LFO };

class Modulator
{
	
public:
	virtual void setAddr(uint8_t addr) = 0;
	virtual int getAddr() = 0;
	virtual ModulatorTypes getType();
	virtual void setBPM(float BPM);
	virtual float getBPM();
	virtual int next(unsigned long micros) = 0;
	virtual void noteOn(uint8_t channel, uint8_t pitch, uint8_t velocity) = 0;
	virtual void noteOff(uint8_t channel, uint8_t pitch, uint8_t velocity) = 0;
	virtual void control1(int amt) = 0;
	virtual void control2(int amt) = 0;
	virtual void control3(int amt) = 0;
	virtual void control4(int amt) = 0;
	virtual void control5(int amt) = 0;
	virtual void control6(int amt) = 0;
	virtual void control7(int amt) = 0;
	virtual void control8(int amt) = 0;

	//include control string name functions for display
	//+others?
};

//Modulator::~Modulator() {}