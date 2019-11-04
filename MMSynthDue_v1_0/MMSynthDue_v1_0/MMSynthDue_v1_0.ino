/*
 Name:		MMSynthDue_v1_0.ino
 Created:	10/17/2018 10:23:11 PM
 Author:	Matt Goetz & Matt Barton - MMSynthCo
*/

//analog read is 10 bit 1023
//dac out is 12 bit 4095
//65565 - 16 bit

//#define main
#define dactest

#include "MM_ADSR.h"
#include "Modulator.h"
#include <SPI.h>
#include "dac_commands.h"

#include <MIDI.h>
#include "notemap.h"
#include <DueTimer.h>
#include "MM_Queue.h"

const int slaveSelectPin = 9;

#ifdef main

Modulator *modulators[8];
volatile MM_Queue *outputs[8];

#endif //main
#if defined(main) || defined(dactest)

void dacSetup() {

	pinMode(slaveSelectPin, OUTPUT);
	digitalWrite(slaveSelectPin, HIGH);

	SPI.begin();
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(SPI_MODE0);

	byte cmd = SELECT_EXT_REF;
	digitalWrite(slaveSelectPin, LOW);
	SPI.transfer(cmd);
	SPI.transfer(0x00);
	SPI.transfer(0x00);
	digitalWrite(slaveSelectPin, HIGH);
}

void writeTo(byte n, volatile int val) {
	//cmd = write and update
	//address = n as byte
	//input = val as 16 bits

	byte cmd_address = WRITEUPDATE_N | n;
	byte msbyte = (val >> 8) & 0xff;
	byte lsbyte = val & 0xff;

	//noInterrupts();
	digitalWrite(slaveSelectPin, LOW);
	SPI.transfer(cmd_address);
	SPI.transfer(msbyte);
	SPI.transfer(lsbyte);
	digitalWrite(slaveSelectPin, HIGH);
	//interrupts();
}

#endif //main, dactest
#ifdef main

void readInputs()
{
	//To test:
//1 get note on/ note off
//call env.next()
//send to dac using address

//get settings in interupt

}

void updateOutputs()
{
	readInputs();

	for (int i = 0; i < 8; i++)
	{
		//check for -1, if so do not send
	}
}

// the setup function runs once when you press reset or power the board
void setup() {

	dacSetup();

	for (int i = 0; i < 8; i++)
	{
		outputs[i] = new volatile MM_Queue();
		outputs[i]->enqueue(0);
	}

	Timer.getAvailable().attachInterrupt(updateOutputs).start(50);

}

// the loop function runs over and over again until power down or reset
void loop() {

}

#endif //main


#ifdef dactest
const int button = 2;
int buttonState = LOW;
const int MAX = 65565;
int testOutput;

void readInputs()
{

}

void updateOutputs()
{
	readInputs();

	if (testOutput > MAX) {
		testOutput = 0;
	}
	writeTo(1, testOutput);
	testOutput += 1;
}

// the setup function runs once when you press reset or power the board
void setup() {

	dacSetup();
	testOutput = 0;
	//MM_ADSR *test = new MM_ADSR();
	//test->init();
	//modulators[0] = test;

	//pinMode(button, INPUT);
}

// the loop function runs over and over again until power down or reset
void loop() {
	if (testOutput > MAX) {
		testOutput = 0;
	}
	writeTo(1, testOutput);
	testOutput += 1;
}

#endif