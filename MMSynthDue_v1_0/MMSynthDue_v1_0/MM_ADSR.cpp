#include "MM_ADSR.h"
#include <math.h>
//#include <SoftwareSerial.h>
#include <Arduino.h>

//SoftwareSerial Serial(10, 11);

MM_ADSR::MM_ADSR()	//default
{
	MM_ADSR(900, 400, 200, 500, 0, 4095);
}

MM_ADSR::MM_ADSR(uint8_t outAddr)	//default with output address
{
	MM_ADSR(900, 400, 200, 500, outAddr, 4095);
}

MM_ADSR::MM_ADSR(int attack, int decay, int sustain, int release, uint8_t outAddr)
{
	MM_ADSR(attack, decay, sustain, release, outAddr, 4095);
}

MM_ADSR::MM_ADSR(int attack, int decay, int sustain, int release, uint8_t outAddr, int gain)
{
	//Serial.begin(115200);

	setAttack(attack);
	setDecay(decay);
	setSustain(sustain);
	setRelease(release);

	this->dacAddr = outAddr;
	if (gain > 4095) gain = 4095;
	this->max_level = gain;			//max output to a dac.
	this->phase = P_ATTACK;
}

int MM_ADSR::next()
{
	//if (!env_active) return 0;
	if (!env_active && !loop_mode) return 0;

	switch (phase)
	{
	case MM_ADSR::P_ATTACK:

		output += aInc;
		if (output >= max_level - EPSILON)
		{
			phase = P_DECAY;
		}

		break;
	case MM_ADSR::P_DECAY:

		output -= dInc;

		//if in loop mode reset to attack once hit 0 instead of sustain.
		if (loop_mode)
		{
			if (output < sustain + 1)
			{
				phase = P_ATTACK;
			}
		}
		else
		{
			if (output <= sustain)
			{
				phase = P_SUSTAIN;
			}
		}
		break;

	case MM_ADSR::P_SUSTAIN:
		//do nothing
		break;

	case MM_ADSR::P_RELEASE:

		output -= rInc;

		//if get to 0 switch env_active to false;
		if (output <= EPSILON)
		{
			env_active = false;
			output = 0;
		}

		break;
	default:
		return 0;
		break;
	}

	return output;
}

void MM_ADSR::noteOn()
{
	//trigger = true;
	env_active = true;
	//target_drive = max_level;	//drive to max during attack.
	phase = P_ATTACK;
	//alpha = attack;

}

void MM_ADSR::noteOff()
{
	//trigger = false;
	env_active = false;
	phase = P_RELEASE;
	//alpha = release;
	//target_drive = 0;

	//env_active = false;

}

void MM_ADSR::normalMode()
{
	this->loop_mode = false;
}

void MM_ADSR::loopMode()
{
	loop_mode = true;
}

void MM_ADSR::setGain(int gain)
{
	if (gain > 4095) gain = 4095;
	this->max_level = gain;
}

void MM_ADSR::setAttack(int attack)
{
	if (attack > 1023) attack = 1023;
	//this->attack = 0.999*cos((1023 - attack) / 795);
	//this->attack = sqrt(this->attack);
	this->aInc = map(attack, 0, 1023, 0, 255);
}

void MM_ADSR::setDecay(int decay)
{
	if (decay > 1023) decay = 1023;
	//this->decay = 0.999*cos((1023 - decay) / 795);
	//this->decay = sqrt(this->decay);
	this->dInc = map(decay, 0, 1023, 0, 255);
}

void MM_ADSR::setSustain(int sustain)
{
	if (sustain > 4095) sustain = 4095;
	this->sustain = sustain;
}

void MM_ADSR::setRelease(int release)
{
	if (release > 1023) release = 1023;
	//this->release = 0.999*cos((1023 - release) / 795);
	//this->release = sqrt(this->release);
	this->rInc = map(release, 0, 1023, 0, 255);
}

void MM_ADSR::setLevels(int attack, int decay, int sustain, int release)
{
	setAttack(attack);
	setDecay(decay);
	setRelease(release);
	setSustain(sustain);
}

uint8_t MM_ADSR::getOutputAddress()
{
	return dacAddr;
}


