/*
 Name:		MMSynthDue_v1_0.ino
 Created:	10/17/2018 10:23:11 PM
 Author:	Matt Goetz & Matt Barton - MMSynthCo
*/

//todo November 20201
//midi tracking off?
//static notemap******, static clock, priority?, note?
//first midi note = 0 every time.
//TODO: remove queue?

//1/23/2022
//todo find crash in switching mods or just lfo?
//todo: fix volatile usages
//Figure out screen pattern and get to point where you can see any/all settings whether or not they're implemented
//Fill out modulators


#include <SPI.h>
#include <MIDI.h>
#include <DueTimer.h>
#include <Arduino.h>

#include "dac_commands.h"
#include "Encoder.h"
#include "Button.h"
#include "Screen.h"
#include "MM_LFO.h"
#include "Modulator.h"
#include "MM_ADSR.h"
#include "MM_Note.h"
#include "Constants.h"
#include "MM_Utils.h"

bool doUpdate = false;
bool inMenu = false;
bool inChangeMod = false; //have pressed shift + turned control 8
bool changeModTickUp = false;	//require 2 ticks/update
bool changeModTickDown = false;	//require 2 ticks/update

ModulatorTypes newModulator;	//current selected modulator
Modulator *modulators[NUM_OUTPUTS];
Modulator *inFocusModulator;

Encoder *encoderOne;
Encoder *encoderTwo;
Encoder *encoderThree;
Encoder *encoderFour;

Button *buttonMenu;
Button *buttonShift;
Button *button1;
Button *button2;
Button *button3;
Button *button4;

Screen *screen;

MM_Utils* utils;

//region MIDI
MIDI_CREATE_INSTANCE(HardwareSerial, Serial, MIDI);
#define REDLEDPIN 7

void handleNoteOn(byte channel, byte pitch, byte velocity) {
	for (int i = 0; i < NUM_OUTPUTS; i++) {
		modulators[i]->noteOn(channel, pitch, velocity);
	}
	digitalWrite(REDLEDPIN, LOW);
}

void handleNoteOff(byte channel, byte pitch, byte velocity) {
	for (int i = 0; i < NUM_OUTPUTS; i++) {
		modulators[i]->noteOff(channel, pitch, velocity);
	}
	digitalWrite(REDLEDPIN, HIGH);
}


//other methods in this section...

/////////////////////////////////////////////////////////////////////////
// millisecond timer related
//
// See documentation for MStimer2 (http://playground.arduino.cc/Main/MsTimer2).
////////////////////////////////////////////////////////////////////////

void timer_callback()
{
	// Tell the mainline loop that time has elapsed
	//send_tick = true;
}

/* void tick_func()
*
* Called by mainline loop when send_tick is true.
* Keeps track of rising/falling edges, and notifies notetracker
* of clock status.
*/
void tick_func()
{
	int check = 0;
	//static uint8_t counter = 0;

	//counter++;

	//if (counter & 0x01)
	//{
		//digitalWrite(REDLEDPIN, HIGH);

		//themap.tickArp(false);
		//updateOutputs();
	//}
	//else
	//{
		//digitalWrite(REDLEDPIN, LOW);

		//themap.tickArp(true);
		//updateOutputs();
	//}
}


void sendTempoTick() {
	//maybe here send a timpo tick to all modulators?
}
//end region MIDI


void dacSetup() {

	pinMode(slaveSelectPin, OUTPUT);
	digitalWrite(slaveSelectPin, HIGH);

	SPI.begin();
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(SPI_MODE0);

	uint8_t cmd = SELECT_EXT_REF;
	noInterrupts();
	digitalWrite(slaveSelectPin, LOW);
	SPI.transfer(cmd);
	SPI.transfer(0x00);
	SPI.transfer(0x00);
	digitalWrite(slaveSelectPin, HIGH);
	interrupts();
}

void writeTo(uint8_t n, volatile int val, bool do_final) {
	//cmd = write and update
	//address = n as byte
	//input = val as 16 bits
	uint8_t command;
	if (do_final) {
		//writes to N and updates dac outputs on all pins
		command = WRITE_N_UPDATE_ALL | n;
	}
	else {
		command = WRITE_N | n;
	}
	uint8_t msbyte = (val >> 8) & 0xff;
	uint8_t lsbyte = val & 0xff;

	noInterrupts();
	digitalWrite(slaveSelectPin, LOW);
	SPI.transfer(command);
	SPI.transfer(msbyte);
	SPI.transfer(lsbyte);
	digitalWrite(slaveSelectPin, HIGH);
	interrupts();
}

void readInputs() {
	bool menuPressed = buttonMenu->pressed();

	if (menuPressed && !inMenu) {
		//go to menu
		inMenu = true;
	} else if (!menuPressed && inMenu) {
		//leave menu
		inMenu = false;
	} else if (buttonShift->pressed()) {
		//shift

		if (button1->pressed()) {
			changeFocus(modulators[OUT_5]);
		}
		else if (button2->pressed()) {
			changeFocus(modulators[OUT_6]);
		}
		else if (button3->pressed()) {
			changeFocus(modulators[OUT_7]);
		}
		else if (button4->pressed()) {
			changeFocus(modulators[OUT_8]);
		}

		inFocusModulator->control5(encoderOne->getUpdate());
		inFocusModulator->control6(encoderTwo->getUpdate());
		inFocusModulator->control7(encoderThree->getUpdate());

		changeModulator(encoderFour->getUpdate());
		//inFocusModulator->control8(encoderFour->getUpdate());

	} else {
		if (inChangeMod) {
			//do we need to update the array here????
			if (newModulator == Note) {
				MM_Note *note = new MM_Note();
				note->init(inFocusModulator->getAddr());
				changeFocus(note);
			}
			else if (newModulator == ADSR) {
				MM_ADSR *adsr = new MM_ADSR();
				adsr->init(inFocusModulator->getAddr());
				changeFocus(adsr);
			}
			else {
				MM_LFO *lfo = new MM_LFO();
				lfo->init(inFocusModulator->getAddr());
				changeFocus(lfo);
			}
			modulators[inFocusModulator->getAddr()] = inFocusModulator;
			inChangeMod = false;
			changeModTickDown = false;
			changeModTickUp = false;
		}
		//No shift

		if (button1->pressed()) {
			changeFocus(modulators[OUT_1]);
		}
		else if (button2->pressed()) {
			changeFocus(modulators[OUT_2]);
		}
		else if (button3->pressed()) {
			changeFocus(modulators[OUT_3]);
		}
		else if (button4->pressed()) {
			changeFocus(modulators[OUT_4]);
		}

		inFocusModulator->control1(encoderOne->getUpdate());
		inFocusModulator->control2(encoderTwo->getUpdate());
		inFocusModulator->control3(encoderThree->getUpdate());
		inFocusModulator->control4(encoderFour->getUpdate());
	}
}

void changeFocus(Modulator *modulator) {
	inFocusModulator = modulator;
	newModulator = inFocusModulator->getType();		//track starting point for change sequence
}

//Is this still a thing?????
void update() {
	doUpdate = true;
}

void updateOutputs() {

	readInputs();

	unsigned long microSeconds = micros();

	for (uint8_t i = OUT_1; i < OUT_8; i++) {
		writeTo(i, modulators[i]->next(microSeconds), false);
	}

	writeTo(OUT_8, modulators[OUT_8]->next(microSeconds), true);

	//screen->blank();
	screen->draw();
}

void changeModulator(int amt) {
	if (amt > 0) {
		if (changeModTickUp) {
			//do thing
			if (newModulator == Note) newModulator = ADSR;
			if (newModulator == ADSR) newModulator = LFO;

			changeModTickUp = false;

			inChangeMod = newModulator != inFocusModulator->getType();
		}
		else {
			changeModTickDown = false;
			changeModTickUp = true;
		}
	}
	else if (amt < 0) {
		if (changeModTickDown) {
			//do thing
			if (newModulator == ADSR) newModulator = Note;
			if (newModulator == LFO) newModulator = ADSR;

			changeModTickDown = false;

			inChangeMod = newModulator != inFocusModulator->getType();
		}
		else {
			changeModTickUp = false;
			changeModTickDown = true;
		}
	}
}

void defaultInitialization() {
	MM_Note *a = new MM_Note();
	a->init(OUT_8);			//Might need to limit these - as short as possible maybe 2500

	MM_ADSR *b = new MM_ADSR();
	b->init(MAX_DRIVE, MAX_DRIVE, 30000, MAX_DRIVE, OUT_2);

	MM_ADSR *c = new MM_ADSR();
	c->init(2000000, 1000000, 40000, 5000000, OUT_3);

	MM_ADSR *d = new MM_ADSR();
	d->init(500000, 1000000, 50000, 2500000, OUT_4);

	MM_ADSR *h = new MM_ADSR();
	h->init(1000000, 1000000, 55000, 8000000, OUT_8);

	MM_ADSR *g = new MM_ADSR();
	g->init(4000000, 700000, 25000, 200000, OUT_7);

	MM_ADSR *f = new MM_ADSR();
	f->init(100000, 100000, 55000, 10000000, OUT_6);

	//LFO on end
	MM_LFO *e = new MM_LFO();
	e->init(OUT_5);

	modulators[a->getAddr()] = a;
	modulators[b->getAddr()] = b;
	modulators[c->getAddr()] = c;
	modulators[d->getAddr()] = d;
	modulators[e->getAddr()] = e;
	modulators[f->getAddr()] = f;
	modulators[g->getAddr()] = g;
	modulators[h->getAddr()] = h;

	changeFocus(a);
}

// the setup function runs once when you press reset or power the board
void setup() {
	pinMode(TFT_BACKLIGHT_PIN, OUTPUT);
	digitalWrite(TFT_BACKLIGHT_PIN, LOW);

	delay(100);

	//initialize controls
	encoderOne = new Encoder(RE_1_A, RE_1_B, RE_1_BUTTON);
	encoderTwo = new Encoder(RE_2_A, RE_2_B, RE_2_BUTTON);
	encoderThree = new Encoder(RE_3_A, RE_3_B, RE_3_BUTTON);
	encoderFour = new Encoder(RE_4_A, RE_4_B, RE_4_BUTTON);
	buttonMenu = new Button(BUTTON_MENU);
	buttonShift = new Button(BUTTON_SHIFT);
	button1 = new Button(BUTTON_1);
	button2 = new Button(BUTTON_2);
	button3 = new Button(BUTTON_3);
	button4 = new Button(BUTTON_4);

	screen = new Screen();
	digitalWrite(TFT_BACKLIGHT_PIN, HIGH);
	screen->blank();
	delay(100);
	//screen->draw();

	if (modulators[OUT_1] == NULL) defaultInitialization();

	dacSetup();

	utils = new MM_Utils();

	//TODO test if this is even in use
	Timer.getAvailable().attachInterrupt(update).start(50);

	// Initiate MIDI communications, listen to all channels
	MIDI.begin(MIDI_CHANNEL_OMNI);
	MIDI.turnThruOff();
	MIDI.setHandleNoteOn(handleNoteOn);
	MIDI.setHandleNoteOff(handleNoteOff);

	pinMode(REDLEDPIN, OUTPUT);
	digitalWrite(REDLEDPIN, HIGH);
}

void loop() {
	MIDI.read();
	//if (doUpdate) {
		updateOutputs();
	//}
}