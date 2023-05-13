#pragma once
#include <Arduino.h>
//#define protoboard

//assume 100 ticks
#define ENCODER_BASE_ADJ 10
#define ENCODER_PRESSED_MULT 3

#ifdef protoboard
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

#define BUTTON_SHIFT 49   //on prototype
//#define BUTTON_SHIFT 48
#define BUTTON_1 50
#define BUTTON_2 51
#define BUTTON_3 52
#define BUTTON_4 53
#define BUTTON_MENU 35    //on prototype
//#define BUTTON_MENU 22

#define TFT_DC_PIN 13
#define TFT_SD_CS_PIN 12
#define TFT_LCD_CS_PIN 11
#define TFT_BACKLIGHT_PIN 10

#else

//prod board
#define TFT_DC_PIN 10
#define TFT_SD_CS_PIN 11
#define TFT_LCD_CS_PIN 12
#define TFT_BACKLIGHT_PIN 13

#define RE_1_A 24
#define RE_1_BUTTON 22
#define RE_1_B 26

#define RE_2_A 32
#define RE_2_B 34
#define RE_2_BUTTON 30

#define RE_3_A 40
#define RE_3_B 42
#define RE_3_BUTTON 38

#define RE_4_A 48
#define RE_4_B 50
#define RE_4_BUTTON 46

#define BUTTON_SHIFT 15   //on prototype
//#define BUTTON_SHIFT 48
#define BUTTON_1 28
#define BUTTON_2 36
#define BUTTON_3 44
#define BUTTON_4 52
#define BUTTON_MENU 14    //on prototype

//************* IMPORTANT!!!!!!!!
//d5-d12 pull up pins
//d2, 3 pull down
#define D2_R 8
#define D2_B 7
#define D2_G 6

#define D3_R 5
#define D3_B 4
#define D3_G 3

#define D5_R 23
#define D5_G 27

#define D6_R 25
#define D6_G 29

#define D7_R 31
#define D7_G 35

#define D8_R 33
#define D8_G 37

#define D9_R 39
#define D9_G 43

#define D10_R 41
#define D10_G 45

#define D11_R 47
#define D11_G 51

#define D12_R 49
#define D12_G 53

#endif

//? STILL USED?
const unsigned long debounceDelay = 40;