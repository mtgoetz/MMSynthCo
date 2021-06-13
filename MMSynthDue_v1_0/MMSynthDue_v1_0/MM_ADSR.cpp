#include "MM_ADSR.h"
//#include <math.h>
//#include <SoftwareSerial.h>

//SoftwareSerial Serial(10, 11);

void MM_ADSR::init()	//default
{
	init(900, 400, 200, 500, 0, MAX_DRIVE - 1, false, false);
}

void MM_ADSR::init(uint8_t outAddr)	//default with output address
{
	init(900, 400, 200, 500, outAddr, MAX_DRIVE - 1, false, false);
}

void MM_ADSR::init(unsigned long attack, unsigned long decay, int sustain, unsigned long release, uint8_t outAddr) //default to max level for gain
{
	init(attack, decay, sustain, release, outAddr, MAX_DRIVE - 1, false, false);
}

//going to need a bigger function for loading from memory..
void MM_ADSR::init(unsigned long attack, unsigned long decay, int sustain, unsigned long release, uint8_t outAddr, int gain, bool loopMode, bool invert) //full cnstr
{
	setAttack(attack);
	setDecay(decay);
	setSustain(sustain);
	setRelease(release);

	//if remove this also do constructos
	//setGain(gain);

	this->dacAddr = outAddr;			//max output to a dac.
	this->output = 0;
	this->loopMode = loopMode;
	this->inverted = invert;																// take 100ms as initial value for Release

	for (int i = 0; i < ADSR_TABLE_SIZE; i++) {												// Create look-up table for Attack
		attackTable[i] = i;
		decayReleaseTable[i] = MAX_DRIVE - 1 - i;
	}

	for (int i = 0; i < ADSR_TABLE_SIZE - 1; i++) {											// Create look-up table for Decay
		attackTable[i + 1] = (1.0 - ATTACK_ALPHA) * (MAX_DRIVE - 1) + ATTACK_ALPHA * attackTable[i];
		decayReleaseTable[i + 1] = ATTACK_DECAY_RELEASE * decayReleaseTable[i];
	}

	for (int i = 0; i < ADSR_TABLE_SIZE; i++) {												// normalize table to min and max
		attackTable[i] = map(
			attackTable[i] / 10,
			0, 
			attackTable[ADSR_TABLE_SIZE - 1] / 10,
			0, 
			MAX_DRIVE - 1);

		decayReleaseTable[i] = map(
			decayReleaseTable[i] / 10,
			decayReleaseTable[ADSR_TABLE_SIZE - 1] / 10,
			decayReleaseTable[0] / 10,
			0,
			MAX_DRIVE - 1);
	}
}

int MM_ADSR::next()
{
	getWave(micros());
}

int MM_ADSR::getWave(unsigned long micros)
{
	unsigned long delta = 0;
	if (this->noteOffMicros < this->noteOnMicros) {			// if note is pressed
		delta = micros - this->noteOnMicros;
		if (delta < attack)									// Attack
			output = map(attackTable[(int)floor(ADSR_TABLE_SIZE * (float)delta / (float)attack)], 0, MAX_DRIVE - 1, attackStart, MAX_DRIVE - 1);
		else if (delta < attack + decay) { // Decay
			delta = micros - this->noteOnMicros - attack;
			output = map(decayReleaseTable[(int)floor(ADSR_TABLE_SIZE * (float)delta / (float)decay)], 0, MAX_DRIVE - 1, sustain, this->output);
			int test = output;
		}
		else
			output = sustain;
	}
	if (this->noteOffMicros > this->noteOnMicros) {			// if note not pressed
		delta = micros - this->noteOffMicros;
		if (delta < release)								// release
			output = map(decayReleaseTable[(int)floor(ADSR_TABLE_SIZE * (float)delta / (float)release)], 0, MAX_DRIVE - 1, 0, releaseStart);
		else
			output = 0;									// note off
	}
	//if (output > MAX_DRIVE - 1) {
	//	output = MAX_DRIVE - 1;
	//}
	return output;
}

volatile void MM_ADSR::noteOn(uint8_t channel, uint8_t pitch, uint8_t velocity)
{
	//could do something with velocity here
	this->noteOn(micros());
}

void MM_ADSR::noteOff(uint8_t channel, uint8_t pitch, uint8_t velocity)
{
	this->noteOff(micros());
}

void MM_ADSR::noteOn(unsigned long micros)
{
	this->noteOnMicros = micros;			// set new timestamp for note_on
	this->attackStart = output;				// set start value new Attack
	this->notesPressedCount++;
}

void MM_ADSR::noteOff(unsigned long micros)
{
	this->notesPressedCount--;
	if (this->notesPressedCount <= 0) {						// if all notes are depressed - start release
		this->noteOffMicros = micros;						// set timestamp for note off
		this->releaseStart = output;		// set start value for release
		this->notesPressedCount = 0;
	}
}

//TODO fill out
bool MM_ADSR::toggleInversion()
{
	return false;
}

//int MM_ADSR::mapRange(int val) {
	//todo fix this
//	return map(val, 0, 1023, MIN_INCREMENT, MAX_DRIVE);
//}

//void MM_ADSR::setGain(int gain)
//{
//	if (gain > MAX_DRIVE) gain = MAX_DRIVE;
//	this->max_level = gain;
//}


void MM_ADSR::setAttack(unsigned long attack)
{
	if (attack < 5000) attack = 5000;
	this->attack = attack;
}

void MM_ADSR::setDecay(unsigned long decay)
{
	this->decay = decay;
}

void MM_ADSR::setSustain(int sustain)
{
	if (sustain < 0) sustain = 0;
	if (sustain >= MAX_DRIVE) sustain = MAX_DRIVE - 1;
	this->sustain = sustain;
}

void MM_ADSR::setRelease(unsigned long release)
{
	this->release = release;
}

void MM_ADSR::setAddr(uint8_t addr)
{
	this->dacAddr = addr;
}

int MM_ADSR::getAddr()
{
	return this->dacAddr;
}

void MM_ADSR::loopModeOff()
{
	this->loopMode = false;
}

void MM_ADSR::loopModeOn()
{
	this->loopMode = true;
}

//Attack
void MM_ADSR::control1(int amt)
{
	//because increasing the phase means going slower, a smaller increment amount
	//means a longer phase (increasing the attack time).
	//aInc -= amt;
	//if (aInc < 0) aInc = 0;
	//if (aInc > MAX_DRIVE) aInc = MAX_DRIVE;
}

//Decay
void MM_ADSR::control2(int amt)
{
	//dInc -= amt;
	//if (dInc < 0) dInc = 0;
	//if (dInc > MAX_DRIVE) dInc = MAX_DRIVE;
}

//Sustain
void MM_ADSR::control3(int amt)
{
	//sustain += amt;
	//if (sustain < 0) sustain = 0;
	//if (sustain > MAX_DRIVE) sustain = MAX_DRIVE;
}

//Release
void MM_ADSR::control4(int amt)
{
	//rInc -= amt;
	//if (rInc < 0) rInc = 0;
	//if (rInc > MAX_DRIVE) rInc = MAX_DRIVE;
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

