#pragma once
//#include <Adafruit_GFX.h>
//#include <Fonts/FreeSans12pt7b.h>
//#include <Fonts/FreeSans12pt7b.h>
//#include <Fonts/FreeSans18pt7b.h>
//#include <gfxfont.h>
//#include <Fonts/FreeMono12pt7b.h>
//#include "ILI9341_t3.h"
#include <Adafruit_ILI9341.h>
#include "Controls.h"
#include <Arduino.h>
#include "Modulator.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define HEADER_SIZE 25
#define KNOB_RADIUS 25

class Screen {

private:
	Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_LCD_CS_PIN, TFT_DC_PIN); 
	uint16_t buffer[SCREEN_WIDTH * SCREEN_HEIGHT];
	int param1X = floor(SCREEN_WIDTH * 0.15);
	int param1Y = floor(SCREEN_HEIGHT * 0.75);
	
	int param2X = floor(SCREEN_WIDTH * 0.38);
	int param2Y = floor(SCREEN_HEIGHT * 0.75);

	int param3X = floor(SCREEN_WIDTH * 0.61);
	int param3Y = floor(SCREEN_HEIGHT * 0.75);

	int param4X = floor(SCREEN_WIDTH * 0.85);
	int param4Y = floor(SCREEN_HEIGHT * 0.75);


	//for basic screen
	Modulator *modulator;

	bool shiftPressed = false;
	bool holdForUpdare = false;

	int16_t x1, y1;
	uint16_t w, h;

	int titleX = 140;
	int titleY = 10;

	int textOneX = 5;
	int textOneY = 80;

	int dataOneX = 10;
	int dataOneY = 130;

	int textTwoX = 80;
	int textTwoY = 80;

	int dataTwoX = 85;
	int dataTwoY = 130;

	int textThreeX = 155;
	int textThreeY = 80;

	int dataThreeX = 160;
	int dataThreeY = 130;

	int textFourX = 230;
	int textFourY = 80;

	int dataFourX = 235;
	int dataFourY = 130;

	String paramNames[3][8]{
		{ "Glide", "Arp On/Off", "Arp Type", "Arp Rate", "Vibrato", "Priority", "Fine Tune", "Type" },
		{ "Attack", "Decay", "Sustain", "Release", "Loop Mode", "Inverse", "", "Type" },
		{ "Frequency", "Shape", "Clock Src", "Retrig Mode", "Amplitude", "Amp Offset", "", "Type" }
	};

	String modNames[3] { "Note", "ADSR", "LFO" };

public:
	Screen();
	//splash();
	
	//updateParam1();
	void draw();
	void blank();
	void green();
	void blue();
	void red();
	void drawText(const char* text);

	void controlsTestLabels();
	void controlsTestUpdates(
		bool menu, bool shift, bool bOne, bool bTwo, bool bThree, bool bFour, 
		int eOne, int eTwo, int eThree, int eFour);

	void changeFocusTest(bool shiftOn);
	void changeFocusTest(String changedTo);

	void setAndDrawBasicData(Modulator *focus);

	void setTitle();
	void setParams();
	void setData();

	void updateDataOneTxt();
	void updateDataTwoTxt();
	void updateDataThreeTxt();
	void updateDataFourTxt();

	void writeCanvas();

	void toggleSwitch(bool pressed);
};