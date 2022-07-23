#include "MM_LFO.h"

void MM_LFO::init(uint8_t outAddr) {
	init(outAddr, 30, square);
}

void MM_LFO::init(uint8_t outAddr, unsigned int frequency, LFOShapes shape)
{
	this->thunderzLfo = new lfo(DAC_SIZE);
	this->thunderzLfo->setWaveForm(shape);
	this->thunderzLfo->setAmpl(MAX_DRIVE);
	this->thunderzLfo->setAmplOffset(0);
	this->thunderzLfo->setMode(SyncModes::free);
	this->thunderzLfo->setMode0Freq(30);
	this->setClockSource(ClockSources::cs_free);
	//TODO: clean up period/frequency conversions
	//this->setAddr(outAddr);
	//this->periodLength = cycleTime;
	//this->frequency = 1.0 / this->periodLength;
	//this->currentShape = shape;
	//this->newShape = shape;
	//this->periodMicros = micros();

	//int half = LUT_SIZE / 2;
	//for (int i = 0; i < LUT_SIZE; i++) {
	//	if (i < half) {
	//		squareTable[i] = 0;
	//		triangleTable[i] = map(i, 0, half, 0, MAX_DRIVE);
	//	}
	//	else {
	//		squareTable[i] = MAX_DRIVE;
	//		triangleTable[i] = map(i, half, LUT_SIZE, MAX_DRIVE, 0);
	//	}
	//	sawtoothTable[i] = map(i, 0, LUT_SIZE, 0, MAX_DRIVE);
	//}
}

//Modulator method declarations

int MM_LFO::next(unsigned long micros) {

	return this->thunderzLfo->getWave(micros);

	//unsigned long delta = micros - this->periodMicros;

	//if (delta > this->periodLength) {		//figure out when a period is over and accept new shape at 0
	//	this->periodMicros = micros;
	//	delta = 0;
	//	this->currentShape = this->newShape;
	//}

	//int index = (int)floor(LUT_SIZE * (float)delta / (float)periodLength);
	//switch (this->currentShape) {
	//case square:
	//	return this->squareTable[index];
	//case triangle:
	//	return this->triangleTable[index];
	//case sawtooth:
	//	return this->sawtoothTable[index];
	//case reverseSawtooth:
	//	return this->sawtoothTable[LUT_SIZE - index];
	//default:
	//	//make it obvious something has gone wrong
	//	return 32500;
	//}
}

void MM_LFO::noteOn(uint8_t channel, uint8_t pitch, uint8_t velocity) {
	if (this->retriggerMode) {
		this->thunderzLfo->sync(micros());
	}
}

void MM_LFO::noteOff(uint8_t channel, uint8_t pitch, uint8_t velocity) {
	//nothing
}

void MM_LFO::setClockSource(ClockSources source) {
	this->clockSource = source;
}

int MM_LFO::getClockSource() {
	return this->clockSource;
}

ModulatorTypes MM_LFO::getType() {
	return ModulatorTypes::LFO;
}

//Frequency - make subdivistion enum with rate conversion
void MM_LFO::control1(int amt) {
	if (amt != 0) {
		//todo handle global clock and external -> if so this is always a subdivision of global clock
		//if midi do subdivision -> will have to change what is displayed

		if (this->clockSource == ClockSources::cs_free) {
			this->setBPM(this->getBPM() + ((float)amt * ADJ_RESOLUTION));
		}
		else if (this->clockSource == ClockSources::cs_internal) {
			//internal - do subdivision
		}
		else {
			//external - do subdivision
		}
	}
}

//Shape - does not wrap
void MM_LFO::control2(int amt) {
	//shouldn't need this if already debouncing - todo remove
	//if (!this->shapeTick && amt != 0) {
	//	this->shapeTick = true;
	//	return;
	//}
	if (amt > 0) {
		int current = (int)this->currentShape;
		if (current < (LFOShapes::size - 1)) {
			this->currentShape = (LFOShapes)current++;
		}
		
		//this->shapeTick = false;
	}
	else if (amt < 0) {
		int current = (int)this->currentShape;
		if (current > 0) {
			this->currentShape = (LFOShapes)current--;
		}
		
		//this->shapeTick = false;
	}
	this->thunderzLfo->setWaveForm(this->currentShape);
}

//Clock source - Make enum (free, internal, external)
void MM_LFO::control3(int amt) {
	if (amt > 0) {
		int current = (int)this->clockSource;
		if (current < (ClockSources::size - 1)) {
			this->clockSource = (ClockSources)current++;
		}
	}
	else if (amt < 0) {
		int current = (int)this->clockSource;
		if (current > 0) {
			this->clockSource = (ClockSources)current--;
		}
	}
}

//Retrigger Mode
void MM_LFO::control4(int amt) {
	//might want to hang on these types of controls before committing to new value ***
	//make these methods return bool whether or not they're a wait to trigger control...
	this->retriggerMode = !this->retriggerMode;
}

//amplitude
void MM_LFO::control5(int amt) {
	int newAmplitude = this->thunderzLfo->getAmpl() + amt;
	if (newAmplitude < 0) newAmplitude = 0;
	if (newAmplitude > MAX_DRIVE) newAmplitude = MAX_DRIVE;
	this->thunderzLfo->setAmpl(newAmplitude);
}

//ampl offset
void MM_LFO::control6(int amt) {
	int newOffset = this->thunderzLfo->getAmplOffset() + amt;
	if (newOffset < 0) newOffset = 0;
	if (newOffset > (this->thunderzLfo->getAmpl())) newOffset = this->thunderzLfo->getAmpl();
	this->thunderzLfo->setAmplOffset(newOffset);
}

//None
void MM_LFO::control7(int amt) {

}

//nix
void MM_LFO::control8(int amt) {

}

int MM_LFO::getAddr() {
	return this->dacAddr;
}

void MM_LFO::setAddr(uint8_t addr) {
	this->dacAddr = addr;
}

String MM_LFO::getShape()
{
	//off, triangle, sine, square, saw
	if (this->currentShape == LFOShapes::off) return String("Off");
	else if (this->currentShape == LFOShapes::triangle) return String("Triangle");
	else if (this->currentShape == LFOShapes::sine) return String("Sine");
	else if (this->currentShape == LFOShapes::square) return String("Square");
	else return String("Saw");
}

float MM_LFO::getBPM() {
	return this->bpm;
}

void MM_LFO::setBPM(float bpm) {
	if (bpm < MIN_BPM) {
		bpm = MIN_BPM;
	}
	else if (bpm > MAX_BPM) {
		bpm = MAX_BPM;
	}
	this->bpm = bpm;
	this->frequency = bpm / 60;
	this->thunderzLfo->setMode0Freq(this->frequency);
	this->thunderzLfo->setMode1Bpm(bpm);
}

void MM_LFO::setFrequency(float frequency)
{
	if (frequency < MIN_FREQUENCY) {
		frequency = MIN_FREQUENCY;
	}
	else if (frequency > MAX_FREQUENCY) {
		frequency = MAX_FREQUENCY;
	}
	float bpm = frequency * 60;
	this->bpm = bpm;
	this->frequency = frequency;
	this->thunderzLfo->setMode0Freq(frequency);
	this->thunderzLfo->setMode1Bpm(bpm);
}
