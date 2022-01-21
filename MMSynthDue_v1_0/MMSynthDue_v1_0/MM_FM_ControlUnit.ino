/*
 Name:		MMSynthDue_v1_0.ino
 Created:	10/17/2018 10:23:11 PM
 Author:	Matt Goetz & Matt Barton - MMSynthCo
*/



//todo
//midi tracking off
//static notemap******
//first midi note = 0 every time.
//time based lfo
//notes on demand or isr
//test rotary encs
//TODO: remove queue?
//TODO: move all constants and rename
//todo - rename MAX and put in const file


#define main
#define mod_test
//#define controls_test	//for now use with mod_test
//#define midi
//#define midi_test
//#define dactest

#include "Modulator.h"
#include "MM_ADSR.h"
#include "MM_Note.h"
#include <SPI.h>
#include "dac_commands.h"
#include "Encoder.h"
#include "Button.h"

#include <MIDI.h>
//#include "notemap.h"
#include <DueTimer.h>
//#include "MM_Queue.h"

const int slaveSelectPin = 9;

const uint8_t NUM_OUTPUTS = 8;
const uint8_t OUT_1 = 0;
const uint8_t OUT_2 = 1;
const uint8_t OUT_3 = 2;
const uint8_t OUT_4 = 3;
const uint8_t OUT_5 = 4;
const uint8_t OUT_6 = 5;
const uint8_t OUT_7 = 6;
const uint8_t OUT_8 = 7;

bool doUpdate = false;

bool inMenu = false;

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

#ifdef mod_test
// internal variables
bool trigger_on = false;                            // simple bool to switch trigger on and off
unsigned long   t = 0;                              // timestamp: current time
unsigned long   t_0 = 0;
unsigned long   trigger_duration = 5000000;          // time in 탎
unsigned long   space_between_triggers = 4500000;    // time in 탎
#endif

#ifdef midi
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
#endif

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

	} else if (!menuPressed && inMenu) {
		//leave menu

	} else if (buttonShift->pressed()) {
		//shift

		if (button1->pressed()) {
			inFocusModulator = modulators[OUT_5];
		}
		else if (button2->pressed()) {
			inFocusModulator = modulators[OUT_6];
		}
		else if (button3->pressed()) {
			inFocusModulator = modulators[OUT_7];
		}
		else if (button4->pressed()) {
			inFocusModulator = modulators[OUT_8];
		}

		inFocusModulator->control5(encoderOne->getUpdate());
		inFocusModulator->control6(encoderTwo->getUpdate());
		inFocusModulator->control7(encoderThree->getUpdate());
		inFocusModulator->control8(encoderFour->getUpdate());

	} else {
		//No shift

		if (button1->pressed()) {
			inFocusModulator = modulators[OUT_1];
		}
		else if (button2->pressed()) {
			inFocusModulator = modulators[OUT_2];
		}
		else if (button3->pressed()) {
			inFocusModulator = modulators[OUT_3];
		}
		else if (button4->pressed()) {
			inFocusModulator = modulators[OUT_4];
		}

		inFocusModulator->control1(encoderOne->getUpdate());
		inFocusModulator->control2(encoderTwo->getUpdate());
		inFocusModulator->control3(encoderThree->getUpdate());
		inFocusModulator->control4(encoderFour->getUpdate());
	}


#ifdef mod_test
	adsrReadInputs();
#endif


#ifdef controls_test
	//encOneOutput += testEncoder->getUpdate();
	//if (encOneOutput < 0) encOneOutput = 0;
	//else if (encOneOutput > MAX_DRIVE) encOneOutput = MAX_DRIVE;
#endif
}

//Is this still a thing?????
void update() {
	doUpdate = true;
}

void updateOutputs() {

	readInputs();

	for (uint8_t i = OUT_1; i < OUT_8; i++) {
		writeTo(i, modulators[i]->next(), false);
	}

	writeTo(OUT_8, modulators[OUT_8]->next(), true);

	/*writeTo(OUT_5, 0, false);
	writeTo(OUT_6, 13107, false);
	writeTo(OUT_7, 39321, false);
	writeTo(OUT_8, 65535, true);*/
}

void changeModulator() {

}

void defaultInitialization() {
	MM_Note *a = new MM_Note();
	a->init(OUT_1);			//Might need to limit these - as short as possible maybe 2500

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

	MM_ADSR *e = new MM_ADSR();
	e->init(7000000, 7500000, 10000, 50000, OUT_5);

	modulators[a->getAddr()] = a;
	modulators[b->getAddr()] = b;
	modulators[c->getAddr()] = c;
	modulators[d->getAddr()] = d;
	modulators[e->getAddr()] = e;
	modulators[f->getAddr()] = f;
	modulators[g->getAddr()] = g;
	modulators[h->getAddr()] = h;

	inFocusModulator = a;
}

// the setup function runs once when you press reset or power the board
void setup() {

	delay(100);

#ifdef mod_test
	adsrSetup();
#endif

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


#ifdef midi
	// Initiate MIDI communications, listen to all channels
// .begin sets the thru mode to on, so we'll have to turn it off if we don't want echo
	MIDI.begin(MIDI_CHANNEL_OMNI);

	MIDI.turnThruOff();

	MIDI.setHandleNoteOn(handleNoteOn);
	
	MIDI.setHandleNoteOff(handleNoteOff);

	pinMode(REDLEDPIN, OUTPUT);
	digitalWrite(REDLEDPIN, HIGH);
#endif // midi

#ifdef midi_test
	noteTestSetup();
#endif

	if (modulators[OUT_1] == NULL) defaultInitialization();

	dacSetup();
	Timer.getAvailable().attachInterrupt(update).start(50);
}

// Not used
void loop() {
#ifdef midi
	MIDI.read();
#endif
	updateOutputs();
}



#ifdef mod_test
void adsrReadInputs() {
	t = micros();

	if (trigger_on) {
		if (t > t_0 + trigger_duration) {               // check if trigger can be switched off
			//adsr_class.noteOff(t);                        // inform ADSR class that note/trigger is switched off

			for (int i = 0; i < NUM_OUTPUTS; i++) {
				modulators[i]->noteOff(0, 0, 0);
			}

			//digitalWrite(LED_BUILTIN, LOW);               // switch off LED
			t_0 = t;                                      // reset timestamp
			trigger_on = false;                           // set trigger_on to false
		}
	}
	else {
		if (t > t_0 + space_between_triggers) {            // check if trigger can be switched on
			for (int i = 0; i < NUM_OUTPUTS; i++) {
				modulators[i]->noteOn(0, 0, 0);
			}                        // inform ADSR class that note/trigger is switched on
			//digitalWrite(LED_BUILTIN, HIGH);              // switch on LED
			t_0 = t;                                      // reset timestamp
			trigger_on = true;                            // set trigger_on to false
		}
	}
}

void adsrSetup() {
	//TODO: make these just constructors? try one first
	MM_ADSR *a = new MM_ADSR();
	a->init(3000, 1000000, 30000, 1000000, OUT_1);		//Might need to limit these - as short as possible maybe 2500

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

	MM_ADSR *e = new MM_ADSR();
	e->init(7000000, 7500000, 10000, 50000, OUT_5);

	modulators[a->getAddr()] = a;
	modulators[b->getAddr()] = b;
	modulators[c->getAddr()] = c;
	modulators[d->getAddr()] = d;
	modulators[e->getAddr()] = e;
	modulators[f->getAddr()] = f;
	modulators[g->getAddr()] = g;
	modulators[h->getAddr()] = h;

	inFocusModulator = a;

	//update controls on in focus
	//change in focus
}
#endif

#ifdef midi_test

void noteTestSetup() {
	MM_Note *a = new MM_Note();
	a->init(OUT_1);		//Might need to limit these - as short as possible maybe 2500

	MM_ADSR *b = new MM_ADSR();
	b->init(MAX_DRIVE, MAX_DRIVE, 30000, MAX_DRIVE, OUT_2);

	MM_ADSR *c = new MM_ADSR();
	c->init(8000, 1000000, 40000, 50000, OUT_3);

	MM_ADSR *d = new MM_ADSR();
	d->init(500000, 1000000, 50000, 2500000, OUT_4);

	MM_ADSR *h = new MM_ADSR();
	h->init(5000000, 1000000, 55000, 8000000, OUT_8);

	MM_ADSR *g = new MM_ADSR();
	g->init(2000000, 700000, 25000, 2000000, OUT_7);

	MM_ADSR *f = new MM_ADSR();
	f->init(2000, 100000, 55000, 10000000, OUT_6);

	MM_ADSR *e = new MM_ADSR();
	e->init(3000000, 7500000, 10000, 50000, OUT_5);

	modulators[a->getAddr()] = a;
	modulators[b->getAddr()] = b;
	modulators[c->getAddr()] = c;
	modulators[d->getAddr()] = d;
	modulators[e->getAddr()] = e;
	modulators[f->getAddr()] = f;
	modulators[g->getAddr()] = g;
	modulators[h->getAddr()] = h;
}

#endif

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

//#endif // !adsr_example
//#ifdef adsr_example
//// --------------------------------------------------
////
//// ADSR - based on lookup table - Arduino Due
//// by mo_thunderz (mo-thunderz @github)
////
//// this example periodically triggers the adsr and has been written
//// for the Arduino Due. For use on other boards just reduce the
//// DACSIZE, remove the analogWriteResolution(12) statement and write
//// to a different port in this statement:
//// analogWrite(DAC0, adsr_class.getWave(t));
////
//// Parameters:
//// *) trigger_duration = duration of the adsr trigger (in 탎)
//// *) space_between_trigger = duration between triggers (in 탎)
//// *) adsr_attack = attack time (in 탎)
//// *) adsr_decay = decay time (in 탎)
//// *) adsr_sustain = level for sustain (0 to DACSIZE - 1)
//// *) adsr_release = release time (in 탎)
////
//// --------------------------------------------------
//
//#include "adsr2.h"                                  // import class
//#include <SPI.h>
//#include "dac_commands.h"
//
//#define DACSIZE 65536                               // vertical resolution of the DACs
//
//// variables
//unsigned long   adsr_attack = 900000;               // time in 탎
//unsigned long   adsr_decay = 800000;                // time in 탎
//int             adsr_sustain = 30000;                // sustain level -> from 0 to DACSIZE-1
//unsigned long   adsr_release = 2500000;              // time in 탎
//unsigned long   trigger_duration = 1000000;          // time in 탎
//unsigned long   space_between_triggers = 2500000;    // time in 탎
//
//// internal variables
//bool trigger_on = false;                            // simple bool to switch trigger on and off
//unsigned long   t = 0;                              // timestamp: current time
//unsigned long   t_0 = 0;                            // timestamp: last trigger on/off event
//
//// internal classes
//adsr            adsr_class(DACSIZE);                // ADSR class initialization
//
//
//
//const int slaveSelectPin = 9;
//
//void dacSetup() {
//
//	pinMode(slaveSelectPin, OUTPUT);
//	digitalWrite(slaveSelectPin, HIGH);
//
//	SPI.begin();
//	SPI.setBitOrder(MSBFIRST);
//	SPI.setDataMode(SPI_MODE0);
//
//	byte cmd = SELECT_EXT_REF;
//	noInterrupts();
//	digitalWrite(slaveSelectPin, LOW);
//	SPI.transfer(cmd);
//	SPI.transfer(0x00);
//	SPI.transfer(0x00);
//	digitalWrite(slaveSelectPin, HIGH);
//	interrupts();
//}
//
//void writeTo(byte n, volatile int val, bool do_final) {
//	//cmd = write and update
//	//address = n as byte
//	//input = val as 16 bits
//	byte command;
//	if (do_final) {
//		//writes to N and updates dac outputs on all pins
//		command = WRITE_N_UPDATE_ALL | n;
//	}
//	else {
//		command = WRITE_N | n;
//	}
//	byte msbyte = (val >> 8) & 0xff;
//	byte lsbyte = val & 0xff;
//
//	noInterrupts();
//	digitalWrite(slaveSelectPin, LOW);
//	SPI.transfer(command);
//	SPI.transfer(msbyte);
//	SPI.transfer(lsbyte);
//	digitalWrite(slaveSelectPin, HIGH);
//	interrupts();
//}
//
//
//
//void setup() {
//	delay(100);
//	//analogWriteResolution(12);                        // set the analog output resolution to 12 bit (4096 levels) -> ARDUINO DUE ONLY
//
//	//pinMode(LED_BUILTIN, OUTPUT);                     // initialize LED
//
//	adsr_class.setAttack(adsr_attack);                // initialize attack
//	adsr_class.setDecay(adsr_decay);                  // initialize decay
//	adsr_class.setSustain(adsr_sustain);              // initialize sustain
//	adsr_class.setRelease(adsr_release);              // initialize release
//
//	dacSetup();
//}
//
//void loop() {
//	/*delay(8000);
//	adsr            adsr_class(DACSIZE);
//	exit(0);*/
//	t = micros();                                     // take timestamp
//
//	writeTo(0, adsr_class.getWave(t), true);
//
//	//analogWrite(DAC0, adsr_class.getWave(t));         // update ADSR and write to DAC
//
//	if (trigger_on) {
//		if (t > t_0 + trigger_duration) {               // check if trigger can be switched off
//			adsr_class.noteOff(t);                        // inform ADSR class that note/trigger is switched off
//			//digitalWrite(LED_BUILTIN, LOW);               // switch off LED
//			t_0 = t;                                      // reset timestamp
//			trigger_on = false;                           // set trigger_on to false
//		}
//	}
//	else {
//		if (t > t_0 + space_between_triggers) {            // check if trigger can be switched on
//			adsr_class.noteOn(t);                         // inform ADSR class that note/trigger is switched on
//			//digitalWrite(LED_BUILTIN, HIGH);              // switch on LED
//			t_0 = t;                                      // reset timestamp
//			trigger_on = true;                            // set trigger_on to false
//		}
//	}
//}
//
//#endif