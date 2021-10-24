#pragma once

#include "LightingProgram.h"

class SingleColor : public LightingProgram
{

private:

public:
    SingleColor(CRGB *ledArray, int num) : LightingProgram(ledArray, num, "SingleColor") {}
    ~SingleColor() {}

    void init() {

    }
    void service() {
        for (int i = 0; i < numLeds; i++) {
            leds[i] = currentPalette[0];
        }
        FastLED.show();
    }
};