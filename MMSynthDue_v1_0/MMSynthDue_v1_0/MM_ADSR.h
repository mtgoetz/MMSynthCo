#pragma once
#include <stdint.h>
//reference https://gist.github.com/m0xpd/531c401926306ba839d9d5b12725ba6e

class MM_ADSR
{
private:
	//data members associated with each adsr
	double attack = 0.9;
	double decay = 0.9;
	int sustain;
	double release = 0.95;
	int max_level;
	int target_drive;
	uint8_t dacAddr;

	const uint8_t EPSILON = 100;		//////**********40

	float alpha = 0.7;	//time constant

	bool env_active = false;
	bool loop_mode = false;

	float output = 0.0;		//gain to be sent to dac after formating

	enum Phase
	{
		P_ATTACK,
		P_DECAY_SUSTAIN,
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