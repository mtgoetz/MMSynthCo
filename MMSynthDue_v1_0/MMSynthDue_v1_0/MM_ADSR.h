#pragma once
#include <stdint.h>
//reference https://github.com/rmosquito/PolyEGg/blob/master/PolyEGg/PolyEGg.ino

//note: must be done at a control rate of your design.

class MM_ADSR
{
private:
	//data members associated with each adsr
	int sustain;
	int max_level;
	uint8_t dacAddr;
	const int MAX_DRIVE = 4095;

	int aInc = 1;
	int dInc = 1;
	int rInc = 1;

	const uint8_t EPSILON = 40;		//////**********40

	bool env_active = false;
	bool loop_mode = false;

	int output = 0;		//gain to be sent to dac after formating

	enum Phase
	{
		P_ATTACK,
		P_DECAY,
		P_SUSTAIN,
		P_RELEASE
	}phase;

	//public section method declarations
public:
	MM_ADSR();
	MM_ADSR(uint8_t outAddr);
	MM_ADSR(int attack, int decay, int sustain, int release, uint8_t outAddr);
	MM_ADSR(int attack, int decay, int sustain, int release, uint8_t outAddr, int initial_gain);

	int next();
	void noteOn();
	void noteOff();

	void normalMode();
	void loopMode();

	void setGain(int gain);
	void setAttack(int attack);
	void setDecay(int decay);
	void setSustain(int sustain);
	void setRelease(int release);
	void setLevels(int attack, int decay, int sustain, int release);

	uint8_t getOutputAddress();


	// MM_ADSR Init()
	//{
	//	MM_ADSR *adsr = (MM_ADSR*)malloc(sizeof(MM_ADSR));
	//	adsr->MM_ADSR::MM_ADSR();
	//	return *adsr;
	//}
};
