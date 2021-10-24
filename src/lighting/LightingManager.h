#pragma once

#include "program/LightingProgram.h"

#define NUM_LEDS      300
#define LED_TYPE   WS2811
#define COLOR_ORDER   RGB
#define LED_PIN        4
#define VOLTS          12
#define MAX_MA       4000
#define PATTERN_LEN 4
#define BRIGHTNESS  64

class LightingManager {

private:
    LightingProgram* currentProgram;

public:
    CRGB* leds = new CRGB[NUM_LEDS];

    LightingManager();
    ~LightingManager();

    void setProgram(LightingProgram* program);
    void setPalette(const TProgmemRGBPalette16* pallete);
    void setDelayMs(int delay) {
        if (currentProgram != NULL) {
            currentProgram->setDelayMs(delay);
        }
    }

    void init();
    void service();
};