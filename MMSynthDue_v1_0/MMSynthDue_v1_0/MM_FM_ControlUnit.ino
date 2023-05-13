//#define main	//no longer used?
//#define mod_test
#define controls_test //!!!!Must be used with diableControls
//#define midi
//#define midi_test
//#define dactest
#define disableControls
//#define debugInitOnScreen
//#define disableSreen
//#define focusTest;	//Not working yet, or buttons aren't working + no menu action


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

bool doUpdate = false;	//todo not used?
bool inMenu = false;
bool inChangeMod = false; //have pressed shift + turned control 8
bool changeModTickUp = false;	//require 2 ticks/update
bool changeModTickDown = false;	//require 2 ticks/update

ModulatorTypes newModulator;		//new selection to be initiated
Modulator *modulators[NUM_OUTPUTS];
Modulator *inFocusModulator;		//current selected modulator

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

#ifdef mod_test
// internal variables
bool trigger_on = false;                            // simple bool to switch trigger on and off
unsigned long   t = 0;                              // timestamp: current time
unsigned long   t_0 = 0;
unsigned long   trigger_duration = 5000000;          // time in µs
unsigned long   space_between_triggers = 4500000;    // time in µs
uint8_t screenCount = 0;
#endif

#ifdef controls_test
int buttonOutput = 0;
int encoderOneOutput = 0;
int encoderTwoOutput = 0;
int encoderThreeOutput = 0;
int encoderFourOutput = 0;
bool menuPressed = false;
bool shiftPressed = false;
bool onePressed = false;
bool twoPressed = false;
bool threePressed = false;
bool fourPressed = false;
#endif

#ifdef dactest
int testOutput = 0;
#endif


void dacSetup() {

	pinMode(slaveSelectPin, OUTPUT);
	digitalWrite(slaveSelectPin, HIGH);

	SPI.begin();
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(SPI_MODE0);

	uint8_t cmd = SELECT_EXT_REF;

	//todo: make sure this doesn't cause problems
	//noInterrupts();
	digitalWrite(slaveSelectPin, LOW);
	SPI.transfer(cmd);
	SPI.transfer(0x00);
	SPI.transfer(0x00);
	digitalWrite(slaveSelectPin, HIGH);
	//interrupts();
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
#ifndef disableControls

	bool menuPressed = buttonMenu->pressed();

	if (menuPressed && !inMenu) {
		//go to menu
		inMenu = true;
	} else if (!menuPressed && inMenu) {
		//leave menu
		inMenu = false;
	} else if (buttonShift->pressed()) {
		//shift
#ifdef focusTest
		screen->changeFocusTest(true);
#endif

		screen->toggleSwitch(true);

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
		else {	//only try to do one thing at a time

			//todo: fix this now
			if (inFocusModulator->control5(encoderOne->getUpdate())) {
				screen->updateDataOneTxt();
			}
			if (inFocusModulator->control6(encoderTwo->getUpdate())) {
				screen->updateDataTwoTxt();
			}
			if (inFocusModulator->control7(encoderThree->getUpdate())) {
				screen->updateDataThreeTxt();
			}

			//TODO: make the display show what the selection will be and do the update when shift is released
			changeModulator(encoderFour->getUpdate());
			//inFocusModulator->control8(encoderFour->getUpdate());
		}

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
#ifdef focusTest
		screen->changeFocusTest(false);
#endif
		screen->toggleSwitch(false);

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
		else {
			//todo: consider only allowing one update here...maybe using a boolean for each control to return?
			if (inFocusModulator->control1(encoderOne->getUpdate())) {
				screen->updateDataOneTxt();
			}
			if (inFocusModulator->control2(encoderTwo->getUpdate())) {
				screen->updateDataTwoTxt();
			}
			if (inFocusModulator->control3(encoderThree->getUpdate())) {
				screen->updateDataThreeTxt();
			}
			if (inFocusModulator->control4(encoderFour->getUpdate())) {
				screen->updateDataFourTxt();
			}
		}
	}

#endif // !disableControls

#ifdef mod_test
	simulateNoteOnOff();
#endif

}

void changeFocus(Modulator *modulator) {
	inFocusModulator = modulator;
	newModulator = inFocusModulator->getType();		//track starting point for change sequence
	screen->setAndDrawBasicData(inFocusModulator);

#ifdef focusTest
	screen->changeFocusTest(inFocusModulator->getAddr());
#endif
}

//Is this still a thing?????
void update() {
	doUpdate = true;
}

#ifndef dactest
void updateOutputs() {
#ifndef controls_test

	unsigned long microSeconds = micros();

	for (uint8_t i = OUT_1; i < OUT_8; i++) {
		writeTo(i, modulators[i]->next(microSeconds), false);
	}

	writeTo(OUT_8, modulators[OUT_8]->next(microSeconds), true);

	/*writeTo(OUT_5, 0, false);
	writeTo(OUT_6, 13107, false);
	writeTo(OUT_7, 39321, false);
	writeTo(OUT_8, 65535, true);*/

#if !defined(disableSreen) && !defined(focusTest)
	//screen->blank();
	//screen->draw();
#endif // !disableSreen
#else
	doControlsTest();
#endif // !controls_test
}
#endif //dactest

void changeModulator(int amt) {
	if (amt > 0) {
		if (changeModTickUp) {
			//do thing
			if (newModulator == Note) newModulator = ADSR;
			if (newModulator == ADSR) newModulator = LFO;

			changeModTickUp = false;

			//Needed because we are not wrapping around and do not update every time this happens.
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
	a->init(OUT_1);			//Might need to limit these - as short as possible maybe 2500

	MM_ADSR *b = new MM_ADSR();
	b->init(MAX_DRIVE, MAX_DRIVE, 30000, MAX_DRIVE, OUT_2);

	MM_ADSR *c = new MM_ADSR();
	c->init(2000000, 1000000, 40000, 5000000, OUT_3);

	MM_ADSR *d = new MM_ADSR();
	d->init(500000, 1000000, 50000, 2500000, OUT_4);

	MM_ADSR *e = new MM_ADSR();
	e->init(1000000, 1000000, 55000, 8000000, OUT_5);

	MM_ADSR *g = new MM_ADSR();
	g->init(4000000, 700000, 25000, 200000, OUT_7);

	MM_ADSR *f = new MM_ADSR();
	f->init(100000, 100000, 55000, 10000000, OUT_6);

	//LFO on end
	MM_LFO *h = new MM_LFO();
	h->init(OUT_8);

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
#ifndef dactest
void setup() {
	pinMode(TFT_BACKLIGHT_PIN, OUTPUT);
	digitalWrite(TFT_BACKLIGHT_PIN, LOW);

	delay(100);

	//initialize controls
	//todo - put in separate function
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

#ifdef debugInitOnScreen
	screen->green();
	delay(2000);
#endif

#ifdef mod_test
	modTestSetup();
#endif

#ifdef debugInitOnScreen
	screen->blue();
	delay(3000);
#endif

	//loads every time for now until load from storage
	if (modulators[OUT_1] == NULL) defaultInitialization();

	screen->setAndDrawBasicData(inFocusModulator);

#ifdef debugInitOnScreen
	screen->green();
	delay(3000);
#endif

	dacSetup();

#ifdef debugInitOnScreen
	screen->blue();
	delay(3000);
#endif

//#ifdef controls_test
//	screen->controlsTestLabels();
//#endif

	//utils = new MM_Utils();

	//TODO test if this is even in use
	//TODO -> use instead of loop to lighten load for midi???
	//Timer.getAvailable().attachInterrupt(update).start(50);
	Timer.getAvailable().attachInterrupt(update).start(100000);	//10/second for screen update, consider queuing up screen updates?


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
}

void loop() {
#ifdef midi
	MIDI.read();
#endif
	if (doUpdate) {
		readInputs();
		updateOutputs();

#ifndef controls_test
		//screen->draw();
#endif
		doUpdate = false;
	}
}
#endif //not dactest



//Testing functions
#ifdef mod_test
void simulateNoteOnOff() {
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
				modulators[i]->noteOn(0, 80, 100);
			}                        // inform ADSR class that note/trigger is switched on
			//digitalWrite(LED_BUILTIN, HIGH);              // switch on LED
			t_0 = t;                                      // reset timestamp
			trigger_on = true;                            // set trigger_on to false
		}
	}

	//if (screenCount == 2) {
	//	screen->draw();
	//	screenCount = 0;
	//}
	//screenCount++;
}

void modTestSetup() {
	
#ifdef debugInitOnScreen
	screen->red();
	screen->drawText("in adsr setup\n");
#endif

	MM_ADSR *e = new MM_ADSR();
	e->init(MAX_DRIVE, MAX_DRIVE, 30000, MAX_DRIVE, OUT_5);

	MM_ADSR *f= new MM_ADSR();
	f->init(2000000, 1000000, 40000, 5000000, OUT_6);

	MM_ADSR *g = new MM_ADSR();
	g->init(4000000, 700000, 25000, 200000, OUT_7);

	MM_ADSR *h = new MM_ADSR();
	h->init(100000, 100000, 55000, 10000000, OUT_8);

#ifdef debugInitOnScreen
	screen->drawText("before lfo\n");
	delay(3000);
#endif

	MM_LFO* a = new MM_LFO();
	a->init(OUT_1, 2, LFOShapes::sine);

	MM_LFO* b = new MM_LFO();
	b->init(OUT_2, 1, LFOShapes::square);

	MM_LFO* c = new MM_LFO();
	c->init(OUT_3, 3, LFOShapes::saw);

	MM_LFO* d = new MM_LFO();
	d->init(OUT_4, 5, LFOShapes::triangle);

#ifdef debugInitOnScreen
	screen->drawText("after lfo\n");
	delay(3000);

	if (a == NULL || h == NULL) {
		screen->drawText("problem -----------\n");
	}
#endif

	modulators[a->getAddr()] = a;
	modulators[b->getAddr()] = b;
	modulators[c->getAddr()] = c;
	modulators[d->getAddr()] = d;
	modulators[e->getAddr()] = e;
	modulators[f->getAddr()] = f;
	modulators[g->getAddr()] = g;
	modulators[h->getAddr()] = h;

#ifdef debugInitOnScreen
	screen->drawText("after assignments\n");
	delay(3000);
#endif

	changeFocus(a);

#ifdef debugInitOnScreen
	screen->drawText("after change focus\n");
	delay(3000);
#endif

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

void updateOutputs()
{
	//readInputs();

	if (testOutput > MAX_DRIVE) {
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

//not working?
#ifdef controls_test
void doControlsTest() {

	bool menu = buttonMenu->pressed();
	bool shift = buttonShift->pressed();
	bool one = button1->pressed();
	bool two = button2->pressed();
	bool three = button3->pressed();
	bool four = button4->pressed();

	encoderOneOutput += encoderOne->getUpdate();
	if (encoderOneOutput < 0) encoderOneOutput = 0;
	else if (encoderOneOutput > MAX_DRIVE) encoderOneOutput = MAX_DRIVE;

	encoderTwoOutput += encoderTwo->getUpdate();
	if (encoderTwoOutput < 0) encoderTwoOutput = 0;
	else if (encoderTwoOutput > MAX_DRIVE);

	encoderThreeOutput += encoderThree->getUpdate();
	if (encoderThreeOutput < 0) encoderThreeOutput = 0;
	else if (encoderThreeOutput > MAX_DRIVE) encoderThreeOutput = MAX_DRIVE;

	encoderFourOutput += encoderFour->getUpdate();
	if (encoderFourOutput < 0) encoderFourOutput = 0;
	else if (encoderFourOutput > MAX_DRIVE) encoderFourOutput = MAX_DRIVE;

	if (doUpdate) {
		screen->controlsTestUpdates(menu, shift, one, two, three, four,
			encoderOneOutput, encoderTwoOutput, encoderThreeOutput, encoderFourOutput);
		
		doUpdate = false;
	}

	//writeTo(OUT_2, encoderOneOutput, false);
	//writeTo(OUT_3, encoderTwoOutput, false);
	//writeTo(OUT_4, encoderThreeOutput, false);
	//writeTo(OUT_5, encoderFourOutput, true);
}
#endif