#pragma once
class Modulator
{
public:
	//int output;
	//uint8_t dacAddr;
	//virtual ~Modulator() {}
	virtual void setAddr(uint8_t addr) = 0;
	virtual int next() = 0;
	virtual void noteOn() = 0;
	virtual void noteOff() = 0;
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