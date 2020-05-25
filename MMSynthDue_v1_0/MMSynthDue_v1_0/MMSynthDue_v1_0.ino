/*
 Name:		MMSynthDue_v1_0.ino
 Created:	10/17/2018 10:23:11 PM
 Author:	Matt Goetz & Matt Barton - MMSynthCo
*/

//analog read is 10 bit 1023
//dac out is 12 bit 4095
//65565 - 16 bit
#define main
//#define adsr_test //with main
//#define queue_test //used with main
//#define dactest
#define midi

#include "MM_ADSR.h"
#include "Modulator.h"
#include "dac_commands.h"

#include <SPI.h>
#include <DueTimer.h>
#include "MM_Queue.h"


#ifdef midi

//maybe this sort of stuff should go into the modulator???????????????????????????????????

#include <MIDI.h>
#include "notemap.h"

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

// global variables
//
// notetracker knows which note ons & offs we have seen.
// We refer to it when it's time to generate CV and gate signals,
static notetracker themap;

// Variables for arpeggiator clock.
static uint32_t tempo_delay = 10;
static bool     send_tick = false;



// constants to describe the MIDI input.
// NUM_KEYS is the number of keys we're interpreting
// BASE_KEY is the offset of the lowest key number
static const int8_t NUM_KEYS = 49;
static const int8_t BASE_KEY = 36;


void updateCV(uint8_t key)
{





	//TODO: do something with val....





}

/////////////////////////////////////////////////////////////////////////
// Callbacks for the MIDI parser
/////////////////////////////////////////////////////////////////////////

/* void handleNoteOn(byte channel, byte pitch, byte velocity)
*
*  Called by MIDI parser when note on messages arrive.
*/
void handleNoteOn(byte channel, byte pitch, byte velocity)
{
	// Do whatever you want when a note is pressed.
	// Try to keep your callbacks short (no delays ect)
	// otherwise it would slow down the loop() and have a bad impact
	// on real-time performance.

	themap.noteOn(pitch);

	uint8_t key;

	key = themap.whichKey();

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

	//what about pitch?? maybe change signature of noteon(note which??)

	for (byte i = OUT_1; i <= OUT_8; i++) {
		modulators[i]->noteOn(key);
	}

}

/* void handleNoteOff(byte channel, byte pitch, byte velocity)
*
*  Called by MIDI parser when note off messages arrive.
*/
void handleNoteOff(byte channel, byte pitch, byte velocity)
{
	// Do something when the note is released.
	// Note that NoteOn messages with 0 velocity are interpreted as NoteOffs.
	//Serial.print("off: ");
	//Serial.println(pitch, HEX);

	//if correct channel do this stuff, else send midi???

	themap.noteOff(pitch);

	for (byte i = OUT_1; i <= OUT_8; i++) {
		modulators[i]->noteOff();
	}

	//updateOutputs();
}

//other methods in this section...

void handlePitchBend(byte channel, int bend) {
	//do something


	//we might need a list of modulators that are notes

}

/////////////////////////////////////////////////////////////////////////
// millisecond timer related
//
// See documentation for MStimer2 (http://playground.arduino.cc/Main/MsTimer2).
////////////////////////////////////////////////////////////////////////

//Note: currently not used
void timer_callback()
{
	// Tell the mainline loop that time has elapsed
	send_tick = true;
}

/* void tick_func()
*
* Called by mainline loop when send_tick is true.
* Keeps track of rising/falling edges, and notifies notetracker
* of clock status.
*/
void tick_func()
{
	static uint8_t counter = 0;

	counter++;

	if (counter & 0x01)
	{
		themap.tickArp(false);

		//maybe not??
		updateOutputs();
	}
	else
	{
		themap.tickArp(true);

		//maybe not?
		updateOutputs();
	}
}

/////////////////////////////////////////////////////////////////////////
// Panel interface control routines
/////////////////////////////////////////////////////////////////////////

/*void check_pots()
*
* Read the analog input (tempo control)
*/
//void check_pots()
//{
//	uint32_t pot_val;
//	uint32_t calc;
//
//
//	//get this from rotary encoder on interface or from midi depending on mode.
//	pot_val = analogRead(PIN_TEMPO_POT);
//
//	// Result is 10 bits
//	//calc = (((0x3ff - pot_val) * 75)/1023) + 8;
//	calc = (((0x3ff - pot_val) * 1800) / 1023) + 25;
//
//	tempo_delay = calc;
//
//}



#endif // midi

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

const int UPDATE_PER_SEC = 20000;

#ifdef main

Modulator *modulators[NUM_OUTPUTS];
volatile MM_Queue *outputs = new MM_Queue(10);

#endif //main

#if defined(dactest) || defined(queue_test) || defined(adsr_test)
int testOutput;
int trigger_event;
const int MAX_DAC_OUTPUT = 65565;
#endif

#ifdef adsr_test
int button_pin = 4;
int button_state = 0;
bool note_on = false;
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

#ifdef main
void readInputs()
{
	//To test:
//1 get note on/ note off
//call env.next()
//send to dac using address

//get settings in interupt

#ifdef adsr_test
	button_state = digitalRead(button_pin);

	if (button_state == HIGH && !note_on) {
		for (byte i = OUT_1; i <= OUT_8; i++) {
			modulators[i]->noteOn();
		}
		note_on = true;
	}
	else if (button_state == LOW && note_on) {
		for (byte i = OUT_1; i <= OUT_8; i++) {
			modulators[i]->noteOff();
		}
		note_on = false;
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

#ifdef midi
	MIDI.read();
#endif
}

void updateOutputs()
{
	readInputs();

	//main output sequence
	//might just want to call no interupts, interrupts here instead?
#ifdef adsr_test
	button_state = digitalRead(button_pin);
	if (button_state == HIGH)
	{
		for (byte i = OUT_1; i <= OUT_7; i++) {
			writeTo(i, MAX_DAC_OUTPUT, false);
		}
		writeTo(OUT_8, MAX_DAC_OUTPUT, true);
	}
	else
	{
		for (byte i = OUT_1; i <= OUT_7; i++) {
			writeTo(i, 0, false);
		}
		writeTo(OUT_8, 0, true);
	}
#endif

#ifdef main

	for (byte i = OUT_1; i <= OUT_7; i++) {
		writeTo(i, modulators[i]->next(), false);
	}
	writeTo(OUT_8, modulators[OUT_8]->next(), true);
#endif

}

// the setup function runs once when you press reset or power the board
void setup() {

#ifdef midi
	// .begin sets the thru mode to on, so we'll have to turn it off if we don't want echo
	MIDI.begin(MIDI_CHANNEL_OMNI);

	MIDI.turnThruOff();

	// so it is called upon reception of a NoteOn.
	MIDI.setHandleNoteOn(handleNoteOn);  // Put only the name of the function
										 // Do the same for NoteOffs
	MIDI.setHandleNoteOff(handleNoteOff);

	MIDI.setHandlePitchBend(handlePitchBend);
#endif // midi

#ifdef queue_test
	testOutput = 0;
	trigger_event = 0;
#endif
#ifdef adsr_test
	MM_ADSR *a = new MM_ADSR();
	a->init(5, 5, 900, 5, OUT_1);
	MM_ADSR *b = new MM_ADSR();
	b->init(2000, 400, 500, 1023, OUT_2);
	MM_ADSR *c = new MM_ADSR();
	c->init(500, 100, 800, 600, OUT_3);
	MM_ADSR *d = new MM_ADSR();
	//square
	d->init(0, 0, 1023, 0, OUT_4);
	MM_ADSR *e = new MM_ADSR();
	//max phases
	e->init(1023, 1023, 500, 1023, OUT_5);
	MM_ADSR *f = new MM_ADSR();
	f->init(80, 80, 800, 80, OUT_6);
	MM_ADSR *g = new MM_ADSR();
	g->init(900, 800, 700, 600, OUT_7);
	MM_ADSR *h = new MM_ADSR();
	h->init(100, 100, 1000, 1023, OUT_8);

	modulators[a->getAddr()] = a;
	modulators[b->getAddr()] = b;
	modulators[c->getAddr()] = c;
	modulators[d->getAddr()] = d;
	modulators[e->getAddr()] = e;
	modulators[f->getAddr()] = f;
	modulators[g->getAddr()] = g;
	modulators[h->getAddr()] = h;

	pinMode(button_pin, INPUT);
#endif

	dacSetup();
	//outputs->enqueue(0);
	//50 microseconds = 20,000 times per second.
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