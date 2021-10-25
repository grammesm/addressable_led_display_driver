#pragma once

#include "LightingProgram.h"

class FastLedExample : public LightingProgram
{
private:
    uint8_t brightness = 64;

public:
    FastLedExample(CRGB *ledArray, int num) : LightingProgram(ledArray, num, "ScrollingBanner") {}
    ~FastLedExample() {}

    void FillLEDsFromPaletteColors(uint8_t colorIndex);
    void init();
    void servicePreShow();
};