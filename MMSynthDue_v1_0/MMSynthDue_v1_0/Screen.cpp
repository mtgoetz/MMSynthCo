#include "Screen.h"

Screen::Screen()
{
	tft.begin();
	tft.setRotation(3);
}

void Screen::draw()
{
	tft.drawLine(0, HEADER_SIZE, SCREEN_WIDTH, HEADER_SIZE, ILI9341_CYAN);
	tft.drawCircle(param1X, param1Y, KNOB_RADIUS, ILI9341_DARKGREEN);
	tft.drawCircle(param2X, param2Y, KNOB_RADIUS, ILI9341_DARKGREEN);
	tft.drawCircle(param3X, param3Y, KNOB_RADIUS, ILI9341_DARKGREEN);
	tft.drawCircle(param4X, param4Y, KNOB_RADIUS, ILI9341_DARKGREEN);
}

void Screen::blank() {
	tft.fillScreen(ILI9341_BLACK);
}

void Screen::green() {
	tft.fillScreen(ILI9341_CYAN);
}
