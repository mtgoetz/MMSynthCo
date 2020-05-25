#pragma once
#include <stdint.h>
#include "Modulator.h"
//reference https://github.com/rmosquito/PolyEGg/blob/master/PolyEGg/PolyEGg.ino

//note: must be done at a control rate of your design.

class MM_ADSR : virtual public Modulator
{
private:
	const int MAX_DRIVE = 65565;
	const int MIN_INCREMENT = 20; //phase completes in 5 seconds

	int sustain;
	int max_level;
	int output;
	uint8_t dacAddr;
	bool env_active;
	bool loop_mode;
	bool inverted;

	int aInc;
	int dInc;
	int rInc;

	//const uint8_t EPSILON = 100;		//////**********40

	enum Phase
	{
		P_ATTACK,
		P_DECAY,
		P_SUSTAIN,
		P_RELEASE
	}phase;

	void setGain(int gain);
	void setAttack(int attack);
	void setDecay(int decay);
	void setSustain(int sustain);
	void setRelease(int release);
	void setLevels(int attack, int decay, int sustain, int release);
	int mapRange(int val);

	//public section method declarations
public:
	void init();
	void init(uint8_t outAddr);
	void init(int attack, int decay, int sustain, int release, uint8_t outAddr);
	void init(int attack, int decay, int sustain, int release, uint8_t outAddr, int initial_gain);

	//ADSR unique operations
	void normalMode();
	void loopMode();
	bool toggleInversion();

	//Modulator method declarations
	int next();
	virtual void noteOn(uint8_t key);
	virtual void noteOff();
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
