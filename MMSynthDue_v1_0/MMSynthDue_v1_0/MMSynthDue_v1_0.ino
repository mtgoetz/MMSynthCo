/*
 Name:		MMSynthDue_v1_0.ino
 Created:	10/17/2018 10:23:11 PM
 Author:	Matt Goetz & Matt Barton - MMSynthCo
*/

//analog read is 10 bit 1023
//dac out is 12 bit 4095

#include "MM_ADSR.h"

#include <MIDI.h>
#include "notemap.h"

MM_ADSR *amp_envelope;

// the setup function runs once when you press reset or power the board
void setup() {
	amp_envelope = new MM_ADSR();
}

// the loop function runs over and over again until power down or reset
void loop() {
  
	//To test:
	//1 get note on/ note off
	//call env.next()
	//send to dac using address
}
