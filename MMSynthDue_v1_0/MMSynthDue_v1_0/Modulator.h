#pragma once
enum ModulatorTypes { Note, ADSR, LFO };

class Modulator
{
	
public:
	virtual void setAddr(uint8_t addr) = 0;
	virtual int getAddr() = 0;
	virtual ModulatorTypes getType();
	virtual int next(unsigned long micros) = 0;
	virtual void noteOn(uint8_t channel, uint8_t pitch, uint8_t velocity) = 0;
	virtual void noteOff(uint8_t channel, uint8_t pitch, uint8_t velocity) = 0;
	virtual bool control1(int amt) = 0;
	virtual bool control2(int amt) = 0;
	virtual bool control3(int amt) = 0;
	virtual bool control4(int amt) = 0;
	virtual bool control5(int amt) = 0;
	virtual bool control6(int amt) = 0;
	virtual bool control7(int amt) = 0;
	virtual bool control8(int amt) = 0;

	//virtual void getControl1Name(char* buffer) = 0;
	//virtual void getControl2Name(char* buffer) = 0;
	//virtual void getControl3Name(char* buffer) = 0;
	//virtual void getControl4Name(char* buffer) = 0;
	//virtual void getControl5Name(char* buffer) = 0;
	//virtual void getControl6Name(char* buffer) = 0;
	//virtual void getControl1Name(char* buffer) = 0;
	//virtual void getControl7Name(char* buffer) = 0;
	//void getControl8Name(char* buffer) {
	//	return "Type";
	//}

	virtual void getControl1Val(char* buffer) = 0;
	virtual void getControl2Val(char* buffer) = 0;
	virtual void getControl3Val(char* buffer) = 0;
	virtual void getControl4Val(char* buffer) = 0;
	virtual void getControl5Val(char* buffer) = 0;
	virtual void getControl6Val(char* buffer) = 0;
	virtual void getControl7Val(char* buffer) = 0;
	virtual void getControl8Val(char* buffer) = 0;

	//include control string name functions for display
	//+others?
};

//Modulator::~Modulator() {}