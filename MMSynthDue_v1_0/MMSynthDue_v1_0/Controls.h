#pragma once
#include <Arduino.h>

//assume 100 ticks
#define ENCODER_BASE_ADJ 1
#define ENCODER_PRESSED_MULT 5

#define RE_1_A 36
#define RE_1_BUTTON 37
#define RE_1_B 38

#define RE_2_A 39
#define RE_2_B 41
#define RE_2_BUTTON 40

#define RE_3_A 42
#define RE_3_B 44
#define RE_3_BUTTON 43

#define RE_4_A 45
#define RE_4_B 47
#define RE_4_BUTTON 46

#define BUTTON_SHIFT 49
#define BUTTON_1 50
#define BUTTON_2 51
#define BUTTON_3 52
#define BUTTON_4 53
#define BUTTON_MENU 35

#define TFT_DC_PIN 13
#define TFT_SD_CS_PIN 12
#define TFT_LCD_CS_PIN 11
#define TFT_BACKLIGHT_PIN 10

static int menuButtonState = HIGH;
static int shiftButtonState = HIGH;
static int button1State = HIGH;
static int button2State = HIGH;
static int button3State = HIGH;
static int button4State = HIGH;

const unsigned long debounceDelay = 50;