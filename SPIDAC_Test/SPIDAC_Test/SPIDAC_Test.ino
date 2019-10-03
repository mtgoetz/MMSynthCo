/*
 Name:		SPIDAC_Test.ino
 Created:	5/5/2019 4:28:43 PM
 Author:	mthom
*/

//#include <stdint.h>
#include <SPI.h>
#include "dac_commands.h"

const int slaveSelectPin = 9; //???

int testOutput;

void write(int val) {
	//spisettings(speed, endianness, spimode)
	//SPI.beginTransaction(SPISettings(50000000, MSBFIRST, SPI_MODE0));

}

void dacSetup() {

	pinMode(slaveSelectPin, OUTPUT);
	digitalWrite(slaveSelectPin, HIGH);

	SPI.begin();

	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(SPI_MODE0);


	byte cmd = SELECT_EXT_REF;

	noInterrupts();
	digitalWrite(slaveSelectPin, LOW);
	SPI.transfer(cmd);
	SPI.transfer(0x00);
	SPI.transfer(0x00);
	digitalWrite(slaveSelectPin, HIGH);
	interrupts();
}

void writeTo(byte n, int val) {
	//cmd = write and update
	//address = n as byte
	//input = val as 16 bits

	byte cmd_address = WRITEUPDATE_N | n;
	byte msbyte = (val >> 8) & 0xff;
	byte lsbyte = val & 0xff;

	noInterrupts();
	digitalWrite(slaveSelectPin, LOW);
	SPI.transfer(cmd_address);
	SPI.transfer(msbyte);
	SPI.transfer(lsbyte);
	digitalWrite(slaveSelectPin, HIGH);
	interrupts();
}

// the setup function runs once when you press reset or power the board
void setup() {

	dacSetup();

	testOutput = 0;

	//write select external reference
	//...
}

// the loop function runs over and over again until power down or reset
void loop() {
  
	for (byte i = 0; i < 8; i++) {
		writeTo(i, testOutput);

		//???
		delayMicroseconds(1000);
	}

	testOutput++;
	if (testOutput > 4095) {
		testOutput = 0;
	}
}
