#include "MM_LFO.h"

void MM_LFO::init() {

}

void MM_LFO::init(uint8_t outAddr) {
	this->setAddr(outAddr);
}

//Modulator method declarations
int MM_LFO::next() {

}

volatile void MM_LFO::noteOn(uint8_t channel, uint8_t pitch, uint8_t velocity) {

}

void MM_LFO::noteOff(uint8_t channel, uint8_t pitch, uint8_t velocity) {

}

//Frequency - make subdivistion enum with rate conversion
void MM_LFO::control1(int amt) {

}

//Shape - make shape enum
void MM_LFO::control2(int amt) {

}

//Clock source - Make enum (free, global clock, midi
void MM_LFO::control3(int amt) {

}

//Retrigger
void MM_LFO::control4(int amt) {

}

//if possible level
void MM_LFO::control5(int amt) {

}

//None
void MM_LFO::control6(int amt) {

}

//None
void MM_LFO::control7(int amt) {

}

//nix
void MM_LFO::control8(int amt) {

}

int MM_LFO::getAddr() {
	return this->dacAddr;
}

void MM_LFO::setAddr(uint8_t addr) {
	this->dacAddr = addr;
}