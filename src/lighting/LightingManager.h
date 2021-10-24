#pragma once

#include "program/LightingProgram.h"
#include "Palette.h"

#define NUM_LEDS      300
#define LED_TYPE   WS2811
#define COLOR_ORDER   RGB
#define LED_PIN        4
#define VOLTS          12
#define MAX_MA       4000
#define PATTERN_LEN 4

class LightingManager {
// TODO store brightness in NVRAM
private:
    LightingProgram* currentProgram;
    uint8_t brightness = 128;

public:
    CRGB* leds = new CRGB[NUM_LEDS];

    LightingManager();
    ~LightingManager();

    void setProgram(LightingProgram* program);
    void setPalette(const Palette* palette);
    void setBrigthness(uint8_t newBrightness);
    uint8_t getBrightness() {
        return brightness;
    }
    void setDelayMs(int delay) {
        if (currentProgram != NULL) {
            currentProgram->setDelayMs(delay);
        }
    }

    void init();
    void service();
};