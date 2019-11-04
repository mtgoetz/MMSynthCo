#include "MM_ADSR.h"
#include <math.h>
//#include <SoftwareSerial.h>
#include <Arduino.h>

//SoftwareSerial Serial(10, 11);

void MM_ADSR::init()	//default
{
	init(900, 400, 200, 500, 0, MAX_DRIVE);
}

void MM_ADSR::init(uint8_t outAddr)	//default with output address
{
	init(900, 400, 200, 500, outAddr, MAX_DRIVE);
}

void MM_ADSR::init(int attack, int decay, int sustain, int release, uint8_t outAddr) //default to max level for gain
{
	init(attack, decay, sustain, release, outAddr, MAX_DRIVE);
}

//going to need a bigger function for loading from memory..
void MM_ADSR::init(int attack, int decay, int sustain, int release, uint8_t outAddr, int gain) //full cnstr
{
	//Serial.begin(115200);

	setAttack(attack);
	setDecay(decay);
	setSustain(sustain);
	setRelease(release);
	setGain(gain);

	this->dacAddr = outAddr;			//max output to a dac.
	this->phase = P_ATTACK;
	this->output = 0;
	this->aInc = 1;
	this->dInc = 1;
	this->rInc = 1;
	this->env_active = false;
	this->loop_mode = false;
	this->inverted = false;
}

int MM_ADSR::next()
{
	if (!env_active) return 0;

	switch (phase)
	{
	case MM_ADSR::P_ATTACK:

		output += aInc;
		if (output >= max_level)
		{
			output = max_level;
			phase = P_DECAY;
		}

		break;

	case MM_ADSR::P_DECAY:

		output -= dInc;

		if (output <= sustain)
		{
			output = sustain;
			if (loop_mode)
			{
				phase = P_ATTACK;
			}
			else
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
		if (output <= 0)
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

volatile void MM_ADSR::noteOn()
{
	output = 0;
	env_active = true;
	phase = P_ATTACK;
}

void MM_ADSR::noteOff()
{
	//env_active = false;
	phase = P_RELEASE;
}

void MM_ADSR::normalMode()
{
	this->loop_mode = false;
}

void MM_ADSR::loopMode()
{
	this->loop_mode = true;
}

//fill out
bool MM_ADSR::toggleInversion()
{
	return false;
}

void MM_ADSR::setGain(int gain)
{
	if (gain > MAX_DRIVE) gain = MAX_DRIVE;
	this->max_level = gain;
}

void MM_ADSR::setAttack(int attack)
{
	if (attack > 1023) attack = 1023;
	//this->attack = 0.999*cos((1023 - attack) / 795);
	//this->attack = sqrt(this->attack);
	this->aInc = map(attack, 0, 1023, 0, MAX_DRIVE);
}

void MM_ADSR::setDecay(int decay)
{
	if (decay > 1023) decay = 1023;
	//this->decay = 0.999*cos((1023 - decay) / 795);
	//this->decay = sqrt(this->decay);
	this->dInc = map(decay, 0, 1023, 0, MAX_DRIVE);
}

void MM_ADSR::setSustain(int sustain)
{
	if (sustain > MAX_DRIVE) sustain = MAX_DRIVE;
	this->sustain = sustain;
}

void MM_ADSR::setRelease(int release)
{
	if (release > 1023) release = 1023;
	//this->release = 0.999*cos((1023 - release) / 795);
	//this->release = sqrt(this->release);
	this->rInc = map(release, 0, 1023, 0, MAX_DRIVE);
}

//probably not used
void MM_ADSR::setLevels(int attack, int decay, int sustain, int release)
{
	setAttack(attack);
	setDecay(decay);
	setRelease(release);
	setSustain(sustain);
}

void MM_ADSR::setAddr(uint8_t addr)
{
	this->dacAddr = addr;
}

//Attack
void MM_ADSR::control1(int amt)
{
	//update aInc
	//convert to 0-127 value
	//cast to string
}

//Decay
void MM_ADSR::control2(int amt)
{

}

//Sustain
void MM_ADSR::control3(int amt)
{

}

//Release
void MM_ADSR::control4(int amt)
{

}

//Gain
void MM_ADSR::control5(int amt)
{

}

//loop mode
void MM_ADSR::control6(int amt)
{

}

//Inverted
void MM_ADSR::control7(int amt)
{

}

//Module type --- this will be more complex: need to show on screen, need to accept only when shift
//is released, need main code to do the work so maybe delete this control and manage from main
void MM_ADSR::control8(int amt)
{

}

