#pragma once
#include <Arduino.h>

class MM_Utils {
public:
    //For some reason micros() required a function pointer...
    unsigned long getMircos();
};