#include "MM_Note.h"

void MM_Note::init()
{
	//remove all of these??  We need at least an address
}

void MM_Note::init(uint8_t outAddr)
{
	this->init(MIDDLE_C, outAddr);
}

void MM_Note::init(uint8_t initialKey, uint8_t outAddr)
{
	this->dacAddr = outAddr;
}

int MM_Note::next()
{
	uint32_t val = 400ul + ((this->currentKey * DAC_CAL) / 100ul);

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

void MM_Note::noteOn(uint8_t key)
{
}

void MM_Note::noteOff()
{
}

void MM_Note::control1(int amt)
{
}

void MM_Note::control2(int amt)
{
}

void MM_Note::control3(int amt)
{
}

void MM_Note::control4(int amt)
{
}

void MM_Note::control5(int amt)
{
}

void MM_Note::control6(int amt)
{
}

void MM_Note::control7(int amt)
{
}

void MM_Note::control8(int amt)
{
}

int MM_Note::getAddr()
{
	return 0;
}

void MM_Note::setAddr(uint8_t addr)
{
	this->dacAddr = addr;
}
