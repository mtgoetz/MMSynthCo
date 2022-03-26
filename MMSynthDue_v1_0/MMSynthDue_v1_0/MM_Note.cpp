#include "MM_Note.h"

void MM_Note::init(uint8_t outAddr) {
	this->fineTune = 50;			//Should this be here??
	this->setAddr(outAddr);
}

//Modulator method declarations
int MM_Note::next(unsigned long micros) {	//todo: micros ignored for now...prob need for arp tho
	uint8_t key;

	key = this->themap.whichKey();

#if VERBOSE
	//Serial.print("key: ");
	//Serial.println(key, HEX);
#endif

	// key is in terms of MIDI note number.
	// Constraining the key number to 4 octave range
	// Soc we can do 4 Volts +/- ~0.5V bend range.
	if (key < BASE_KEY)
	{
		key = 0;
	}
	else if (key > BASE_KEY + NUM_KEYS)
	{
		key = NUM_KEYS;
	}
	else
	{
		key -= BASE_KEY;
	}

#if 0
	//Serial.print("KEY: ");
	//Serial.print(key);
#endif
	uint32_t val = ((key * DAC_CAL) / 100ul);
	//val += ((val / 100) * this->fineTune);

#if 0
	val = last_key * 6826ul;
	//Serial.print(" VALa: ");
	//Serial.print(val, HEX);

	val /= 100;
	//Serial.print(" VALb: ");
	//Serial.print(val, HEX);

	val += 400;
	//Serial.print(" VALc: ");
	//Serial.print(val, HEX);
#endif
	val += last_bend;
	return val;
}

volatile void MM_Note::noteOn(uint8_t channel, uint8_t pitch, uint8_t velocity) {
	this->themap.noteOn(pitch);
}

void MM_Note::noteOff(uint8_t channel, uint8_t pitch, uint8_t velocity) {
	this->themap.noteOff(pitch);
}

//Glide
void MM_Note::control1(int amt) {

}

//Arp on/off
void MM_Note::control2(int amt) {

}

//Arp type
void MM_Note::control3(int amt) {

}

//Arp rate
void MM_Note::control4(int amt) {

}

//Vibrato
void MM_Note::control5(int amt) {

}

//Priority (global)
void MM_Note::control6(int amt) {

}

//Fine tune (global) - use static volatile var
void MM_Note::control7(int amt) {

}

//output type (nix from interface)
void MM_Note::control8(int amt) {

}



int MM_Note::getAddr() {
	return this->dacAddr;
}

void MM_Note::setAddr(uint8_t addr) {
	this->dacAddr = addr;
}

ModulatorTypes MM_Note::getType()
{
	return Note;
}
