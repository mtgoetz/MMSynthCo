#pragma once
#define MAX_DRIVE 65535
#define DAC_SIZE 65536
#define MIN_FREQUENCY 0.125
#define MAX_FREQUENCY 6.0
#define MIN_BPM 7.5
#define MAX_BPM 360

const uint8_t NUM_OUTPUTS = 8;
const uint8_t OUT_1 = 0;
const uint8_t OUT_2 = 1;
const uint8_t OUT_3 = 2;
const uint8_t OUT_4 = 3;
const uint8_t OUT_5 = 4;
const uint8_t OUT_6 = 5;
const uint8_t OUT_7 = 6;
const uint8_t OUT_8 = 7;

const int slaveSelectPin = 9;

//TODO rename
enum ClockSources { cs_free, cs_internal, cs_external, size };
