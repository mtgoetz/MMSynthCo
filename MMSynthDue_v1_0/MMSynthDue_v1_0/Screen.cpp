#include "Screen.h"
//#define controls_test

Screen::Screen()
{
	if (&tft == nullptr) {
		tft = Adafruit_ILI9341(TFT_LCD_CS_PIN, TFT_DC_PIN);
	}
	tft.begin();
	tft.setRotation(3);
	tft.setFont(&FreeSans12pt7b);
	//this->canvas.setFont(&FreeSans12pt7b);
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
	tft.fillScreen(ILI9341_GREEN);
}

void Screen::blue() {
	tft.fillScreen(ILI9341_BLUE);
}

void Screen::red()
{
	tft.fillScreen(ILI9341_RED);
}

void Screen::drawText(const char* text)
{
	tft.write(text, strlen(text));
}

void Screen::controlsTestLabels()
{
	//this->blank();
	tft.setTextColor(ILI9341_WHITE);
	tft.setCursor(10, 10);
	tft.println("Buttons:");
	tft.println();
	tft.print("| Menu  | Shift | One   | Two   | Three | Four  |");
	tft.setCursor(10, 100);
	tft.println("Encoders:");
	tft.println();
	tft.print("| One   | Two   | Three | Four  |");
}

void Screen::controlsTestUpdates(
	bool menu, bool shift, bool bOne, bool bTwo, bool bThree, bool bFour,
	int eOne, int eTwo, int eThree, int eFour)
{
	this->controlsTestLabels();

	//tft.fillRect(0, 50, SCREEN_WIDTH, 55, ILI9341_BLACK);
	tft.setCursor(0, 50);
	tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
	//String buttonValues = 
	tft.print("| ");
	tft.print(menu ? "ON    | " : "OFF   | ");
	tft.print(shift ? "ON    | " : "OFF   | ");
	tft.print(bOne ? "ON    | " : "OFF   | ");
	tft.print(bTwo ? "ON    | " : "OFF   | ");
	tft.print(bThree ? "ON    | " : "OFF   | ");
	tft.print(bFour ? "ON    | " : "OFF   | ");

	
	tft.setCursor(13, 150);
	tft.print("     ");
	tft.setCursor(13, 150);
	tft.print(eOne);

	tft.setCursor(58, 150);
	tft.print("     ");
	tft.setCursor(58, 150);
	tft.print(eTwo);
	tft.setCursor(103, 150);
	tft.print("     ");
	tft.setCursor(103, 150);
	tft.print(eThree);
	tft.setCursor(148, 150);
	tft.print("     ");
	tft.setCursor(148, 150);
	tft.print(eFour);
}

void Screen::changeFocusTest(bool shiftOn)
{
	tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
	tft.setTextSize(2);
	tft.setCursor(floor(SCREEN_WIDTH / 4), floor(SCREEN_HEIGHT / 2));
	if (shiftOn) {
		tft.print("Shift");
	}
	else {
		tft.print("     ");
	}
}

void Screen::changeFocusTest(String changedTo)
{
	tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
	tft.setTextSize(2);
	tft.setCursor(floor(SCREEN_WIDTH / 4) * 3, floor(SCREEN_HEIGHT / 2));
	tft.print(changedTo);
}

void Screen::setAndDrawBasicData(Modulator *focus)
{
#ifndef controls_test | disableControls
	this->holdForUpdare = true;
	this->modulator = focus;
	this->shiftPressed = false;
	//this->blank();
	this->canvas.fillScreen(ILI9341_BLACK);
	this->setTitle();
	//this->canvas.setFont(&FreeSans12pt7b);
	this->setParams();
	this->updateDataOneTxt();
	this->updateDataTwoTxt();
	this->updateDataThreeTxt();
	this->updateDataThreeTxt();
	this->updateDataFourTxt();
	this->writeCanvas();
	this->holdForUpdare = false;
#endif
}

void Screen::setTitle()
{
#ifndef controls_test | disableControls
	this->canvas.setCursor(this->titleX, this->titleY);
	//this->canvas.setFont(&FreeSans18pt7b);
	this->canvas.print(this->modNames[this->modulator->getType()]);
	//tft.println(this->modNames[this->modulator->getType()]);
#endif
}

void Screen::setParams()
{
#ifndef controls_test | disableControls
	int paramStart = this->shiftPressed ? 4 : 0;

	this->canvas.setCursor(this->textOneX, this->textOneY);
	canvas.print(this->paramNames[this->modulator->getType()][paramStart++]);

	this->canvas.setCursor(this->textTwoX, this->textTwoY);
	canvas.print(this->paramNames[this->modulator->getType()][paramStart++]);

	this->canvas.setCursor(this->textThreeX, this->textThreeY);
	canvas.print(this->paramNames[this->modulator->getType()][paramStart++]);

	this->canvas.setCursor(this->textFourX, this->textFourY);
	canvas.print(this->paramNames[this->modulator->getType()][paramStart]);
	//canvas.print("hello");
	if (!this->holdForUpdare) this->writeCanvas();
#endif
}

void Screen::setData()
{
#ifndef controls_test | disableControls
	this->updateDataOneTxt();
	this->updateDataTwoTxt();
	this->updateDataThreeTxt();
	this->updateDataFourTxt();
	if (!this->holdForUpdare) this->writeCanvas();
#endif
}

void Screen::updateDataOneTxt() {
#ifndef controls_test | disableControls
	//tft.setCursor(this->dataOneX, this->dataOneY);
	char* buf = new char[16];
	this->shiftPressed ? this->modulator->getControl5Val(buf) : this->modulator->getControl1Val(buf);
	this->canvas.setCursor(this->dataOneX, this->dataOneY);
	this->canvas.getTextBounds(buf, this->dataOneX, this->dataOneY, &this->x1, &this->y1, &this->w, &this->h);
	this->canvas.fillRect(this->x1, this->y1, this->w, this->h, ILI9341_BLACK);
	this->canvas.print(buf);
	if (!this->holdForUpdare) this->writeCanvas();
#endif
}

void Screen::updateDataTwoTxt() {
#ifndef controls_test | disableControls
	//tft.setCursor(this->dataTwoX, this->dataTwoY);
	char* buf = new char[16];
	this->shiftPressed ? this->modulator->getControl6Val(buf) : this->modulator->getControl2Val(buf);
	this->canvas.setCursor(this->dataTwoX, this->dataTwoY);
	this->canvas.getTextBounds(buf, this->dataTwoX, this->dataTwoY, &this->x1, &this->y1, &this->w, &this->h);
	this->canvas.fillRect(this->x1, this->y1, this->w, this->h, ILI9341_BLACK);
	this->canvas.print(buf);
	//this->canvas.print(buf);
	if (!this->holdForUpdare) this->writeCanvas();
#endif
}

void Screen::updateDataThreeTxt() {
#ifndef controls_test | disableControls
	this->canvas.setCursor(this->dataThreeX, this->dataThreeY);
	char* buf = new char[16];
	this->shiftPressed ? this->modulator->getControl7Val(buf) : this->modulator->getControl3Val(buf);
	this->canvas.getTextBounds(buf, this->dataThreeX, this->dataThreeY, &this->x1, &this->y1, &this->w, &this->h);
	this->canvas.fillRect(this->x1, this->y1, this->w, this->h, ILI9341_BLACK);
	this->canvas.print(buf);
	//this->canvas.print(buf);
	if (!this->holdForUpdare) this->writeCanvas();
#endif
}

void Screen::updateDataFourTxt() {
#ifndef controls_test | disableControls
	this->canvas.setCursor(this->dataFourX, this->dataFourY);
	char* buf = new char[16];
	this->shiftPressed ? this->modulator->getControl8Val(buf) : this->modulator->getControl4Val(buf);
	this->canvas.getTextBounds(buf, this->dataFourX, this->dataFourY, &this->x1, &this->y1, &this->w, &this->h);
	this->canvas.fillRect(this->x1, this->y1, this->w, this->h, ILI9341_BLACK);
	this->canvas.print(buf);
	//this->canvas.print(buf);
	if (!this->holdForUpdare) this->writeCanvas();
#endif
}

void Screen::writeCanvas()
{
#ifndef controls_test | disableControls
	this->tft.drawBitmap(0, 0, this->canvas.getBuffer(), 320, 240, ILI9341_WHITE, ILI9341_BLACK);
#endif
}

void Screen::toggleSwitch(bool pressed)
{
#ifndef controls_test | disableControls
	bool needsUpdate = false;
	if (pressed != this->shiftPressed) {
		needsUpdate = true;
	}

	shiftPressed = pressed;

	if (needsUpdate) {
		this->holdForUpdare = true;
		//todo -> move to blank?
		this->canvas.fillScreen(ILI9341_BLACK);
		this->setTitle();
		this->setParams();
		this->setData();
		this->writeCanvas();
		this->holdForUpdare = false;
	}
#endif
}
