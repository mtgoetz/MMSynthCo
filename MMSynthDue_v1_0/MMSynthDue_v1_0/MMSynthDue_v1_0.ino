/*
 Name:		MMSynthDue_v1_0.ino
 Created:	10/17/2018 10:23:11 PM
 Author:	Matt Goetz & Matt Barton - MMSynthCo
*/

//analog read is 10 bit 1023
//dac out is 12 bit 4095
//65565 - 16 bit

#define main
#define mod_test
//#define queue_test //used with main
//#define dactest

#include "MM_ADSR.h"
#include "Modulator.h"
#include <SPI.h>
#include "dac_commands.h"

#include <MIDI.h>
#include "notemap.h"
#include <DueTimer.h>
#include "MM_Queue.h"

const int slaveSelectPin = 9;

const byte NUM_OUTPUTS = 8;

const byte OUT_1 = 0;
const byte OUT_2 = 1;
const byte OUT_3 = 2;
const byte OUT_4 = 3;
const byte OUT_5 = 4;
const byte OUT_6 = 5;
const byte OUT_7 = 6;
const byte OUT_8 = 7;

#ifdef main

Modulator *modulators[NUM_OUTPUTS];
volatile MM_Queue *outputs = new MM_Queue(10);

#endif //main

#if defined(dactest) || defined(queue_test) || defined(mod_test)
int testOutput;
int trigger_event;
const int MAX = 65565;
#endif

#ifdef mod_test
int NOTE_ON_PERIOD = 20000 * 30;
int NOTE_OFF_PERIOD = 20000 * 18;
#endif

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

void writeTo(byte n, volatile int val, bool do_final) {
	//cmd = write and update
	//address = n as byte
	//input = val as 16 bits
	byte command;
	if (do_final) {
		//writes to N and updates dac outputs on all pins
		command = WRITE_N_UPDATE_ALL | n;
	}
	else {
		command = WRITE_N | n;
	}
	byte msbyte = (val >> 8) & 0xff;
	byte lsbyte = val & 0xff;

	noInterrupts();
	digitalWrite(slaveSelectPin, LOW);
	SPI.transfer(command);
	SPI.transfer(msbyte);
	SPI.transfer(lsbyte);
	digitalWrite(slaveSelectPin, HIGH);
	interrupts();
}

#ifdef main

void readInputs()
{
	//To test:
//1 get note on/ note off
//call env.next()
//send to dac using address

//get settings in interupt

#ifdef mod_test
	testOutput++;
	if (testOutput == NOTE_ON_PERIOD) {
		for (int i = OUT_1; i <= OUT_8; i++) {
			modulators[i]->noteOn();
			testOutput = 0;
		}
	}
	else if (testOutput == NOTE_OFF_PERIOD) {
		for (int i = OUT_1; i <= OUT_8; i++) {
			modulators[i]->noteOff();
		}
	}
#endif

#ifdef queue_test

	int loop = 1;
	//if (trigger_event == 15000) {
	//	loop = 9;
	//	trigger_event = 0;
	//}

	for (; loop > 0; loop--) {
		outputs->enqueue(testOutput);
		testOutput++;
		if (testOutput > MAX) {
			testOutput = 0;
		}
	}
	//trigger_event++;
	//if (trigger_event > MAX) {
	//	trigger_event = 0;
	//}

#endif // !queue_test
}

void updateOutputs()
{
	readInputs();

#if defined(main) || defined(mod_test)
	for (byte i = OUT_1; i <= OUT_7; i++) {
		writeTo(i, modulators[i]->next(), false);
	}
	writeTo(OUT_8, modulators[OUT_8]->next(), true);
#endif


	//while (outputs->next()) {
	//	int val = outputs->dequeue();
	//	if (val != -1) {
	//		for (int i = 0; i < 7; i++) {
	//			writeTo(i, val, false);
	//		}
	//		writeTo(7, val, true);
	//	}

	//}

}

// the setup function runs once when you press reset or power the board
void setup() {

#ifdef queue_test
	testOutput = 0;
	trigger_event = 0;
#endif
#ifdef mod_test
	MM_ADSR *a = new MM_ADSR();
	a->init(5, 5, 900, 5, OUT_1);
	MM_ADSR *b = new MM_ADSR();
	b->init(5, 5, 900, 5, OUT_2);
	MM_ADSR *c = new MM_ADSR();
	c->init(5, 5, 900, 5, OUT_3);
	MM_ADSR *d = new MM_ADSR();
	d->init(5, 5, 900, 5, OUT_4);
	MM_ADSR *e = new MM_ADSR();
	e->init(5, 5, 900, 5, OUT_5);
	MM_ADSR *f = new MM_ADSR();
	f->init(5, 5, 900, 5, OUT_6);
	MM_ADSR *g = new MM_ADSR();
	g->init(5, 5, 900, 5, OUT_7);
	MM_ADSR *h = new MM_ADSR();
	h->init(5, 5, 900, 5, OUT_8);

	modulators[a->getAddr()] = a;
	modulators[b->getAddr()] = b;
	modulators[c->getAddr()] = c;
	modulators[d->getAddr()] = d;
	modulators[e->getAddr()] = e;
	modulators[f->getAddr()] = f;
	modulators[g->getAddr()] = g;
	modulators[h->getAddr()] = h;

	for (int i = OUT_1; i <= OUT_8; i++) {
		modulators[i]->noteOn();
	}

	testOutput = 0;
	trigger_event = 0;
#endif

	dacSetup();
	//outputs->enqueue(0);
	Timer.getAvailable().attachInterrupt(updateOutputs).start(50);
}

// Not used
void loop() {
}

#endif //main


#ifdef dactest

const int button = 2;
int buttonState = LOW;

void readInputs()
{

}

void updateOutputs()
{
	readInputs();

	if (testOutput > MAX) {
		testOutput = 0;
	}
	for (int i = 0; i < 7; i++) {
		writeTo(i, testOutput, false);
	}
	writeTo(7, testOutput, true);
	testOutput += 1;

}

// the setup function runs once when you press reset or power the board
void setup() {

	dacSetup();
	testOutput = 0;

	Timer.getAvailable().attachInterrupt(updateOutputs).start(50);
}

// the loop function runs over and over again until power down or reset
void loop() {
}

#endif