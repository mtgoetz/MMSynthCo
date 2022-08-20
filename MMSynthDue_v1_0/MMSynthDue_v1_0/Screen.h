#pragma once
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Controls.h"

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
};