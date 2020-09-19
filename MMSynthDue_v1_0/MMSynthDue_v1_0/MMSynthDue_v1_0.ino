/*
 Name:		MMSynthDue_v1_0.ino
 Created:	10/17/2018 10:23:11 PM
 Author:	Matt Goetz & Matt Barton - MMSynthCo
*/

#include "MM_ADSR.h"
#include "Modulator.h"
#include "dac_commands.h"

#include <SPI.h>
#include <DueTimer.h>


/////////////////////////////////////////////////////////////////////////
// Panel interface control routines
/////////////////////////////////////////////////////////////////////////

Modulator *modulators[8];

MM_ADSR *env1 = new MM_ADSR();
MM_ADSR *env2 = new MM_ADSR();
MM_ADSR *env3 = new MM_ADSR();
MM_ADSR *env4 = new MM_ADSR();
MM_ADSR *env5 = new MM_ADSR();
MM_ADSR *env6 = new MM_ADSR();
MM_ADSR *env7 = new MM_ADSR();
MM_ADSR *env8 = new MM_ADSR();



const int slaveSelectPin = 9;
const int ldcaPin = 8;

const byte NUM_OUTPUTS = 8;

const byte OUT_1 = 0;
const byte OUT_2 = 1;
const byte OUT_3 = 2;
const byte OUT_4 = 3;
const byte OUT_5 = 4;
const byte OUT_6 = 5;
const byte OUT_7 = 6;
const byte OUT_8 = 7;


enum inputs {
	TEST_BUTTON,
	INPUTS_SIZE
};

bool changedInputStates[INPUTS_SIZE];


volatile byte testButtonState = LOW;

const int UPDATE_PER_SEC = 20000;

//Modulator *modulators[NUM_OUTPUTS];

int testOutput;
int trigger_event;
const int MAX_DAC_OUTPUT = 65565;

const int TEST_BUTTON_PIN = 4;
int button_state = LOW;
bool note_on = false;

void dacSetup() {

	pinMode(ldcaPin, OUTPUT);
	digitalWrite(ldcaPin, LOW);

	pinMode(slaveSelectPin, OUTPUT);
	digitalWrite(slaveSelectPin, HIGH);

	SPI.begin();
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(SPI_MODE0);
	//SPI.setClockDivider();

	byte cmd = SELECT_EXT_REF;
	noInterrupts();
	digitalWrite(slaveSelectPin, LOW);
	SPI.transfer(cmd);
	SPI.transfer(0x00);
	SPI.transfer(0x00);
	digitalWrite(slaveSelectPin, HIGH);
	interrupts();
}

void writeTo(byte which, volatile int val, bool do_final) {
	//cmd = write and update
	//address = which as byte
	//input = val as 16 bits
	byte command;
	if (do_final) {
		//writes to N and updates dac outputs on all pins
		command = WRITE_N_UPDATE_ALL | which;
	}
	else {
		command = WRITE_N | which;
	}

	//is msb & 0xff necessary??
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

void readInputs()
{

}

/*void controlChanged(volatile int which, volatile int value) {
	switch (which) {
		case TEST_BUTTON :
			//do update based on state of buttonState
			testButtonState = value;
			changedInputStates[TEST_BUTTON] = true;
			break;
		default :
			break;
	}
		
}*/

void testButtonChanged() {
	testButtonState = !testButtonState;
	changedInputStates[TEST_BUTTON] = true;
}

void updateOutputs()
{

	/*for (byte i = OUT_1; i <= OUT_7; i++) {
		writeTo(i, modulators[i]->next(), false);
	}
	writeTo(OUT_8, modulators[OUT_8]->next(), true);*/

	/*for (int i = 0; i < INPUTS_SIZE; i++) {
		if (changedInputStates[i]) {
			switch (i) {
				case TEST_BUTTON:
					for (byte i = OUT_1; i <= OUT_7; i++) {
						writeTo(i, button_state, false);
					}
					writeTo(OUT_8, button_state, true);
					changedInputStates[i] = false;
					break;
				default:
					break;
			}
		}
	}*/

	if (testOutput > MAX_DAC_OUTPUT) {
		testOutput = 0;
	}

	
	switch (testOutput) {
	case 0:
		env1->noteOn(0);
		env2->noteOn(0);
		env3->noteOn(0);
		env4->noteOn(0);
		env5->noteOn(0);
		env6->noteOn(0);
		env7->noteOn(0);
		env8->noteOn(0);
		break;
	case 40000:
		env1->noteOff();
		env2->noteOff();
		env3->noteOff();
		env4->noteOff();
		env5->noteOff();
		env6->noteOff();
		env7->noteOff();
		env8->noteOff();
	default:
		break;
	}
	/*for (int i = 0; i < 7; i++) {
		writeTo(i, testOutput, false);
	}*/

	Serial.println("Updating...");
	writeTo(0, env1->next(), false);
	writeTo(1, env2->next(), false);
	writeTo(2, env3->next(), false);
	writeTo(3, env4->next(), false);
	writeTo(4, env5->next(), false);
	writeTo(5, env6->next(), false);
	writeTo(6, env7->next(), false);
	writeTo(7, env8->next(), true);

	testOutput += 1;


	/*if (testOutput > MAX_DAC_OUTPUT) {
		testOutput = 0;
	}
	writeTo(0, 0, false);
	writeTo(1, 13113, false);
	writeTo(2, 26226, false);
	writeTo(3, 39339, false);
	writeTo(4, 52452, false);
	testOutput += 1;
	writeTo(5, testOutput, false);
	writeTo(6, (MAX_DAC_OUTPUT - testOutput), false);
	writeTo(7, MAX_DAC_OUTPUT, true);*/

}

// the setup function runs once when you press reset or power the board
void setup() {

	//pinMode(button_pin, INPUT);
	//attachInterrupt(digitalPinToInterrupt(TEST_BUTTON_PIN), testButtonChanged, CHANGE);

	dacSetup();

	testOutput = 0;

	env1->init();
	env2->init();
	env3->init();
	env4->init();
	env5->init();
	env6->init();
	env7->init();
	env8->init();

	//50 microseconds = 20,000 times per second.
	Timer.getAvailable().attachInterrupt(updateOutputs).start(150);

}

// Not used
void loop() {
}





#ifdef dactest

const int button = 2;
int buttonState = LOW;

void readInputs()
{

}

void updateOutputs()
{
	readInputs();

	if (testOutput > MAX_DAC_OUTPUT) {
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