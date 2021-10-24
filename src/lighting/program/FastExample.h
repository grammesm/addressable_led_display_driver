#pragma once

#include "LightingProgram.h"

class FastLedExample : public LightingProgram
{
private:
    const char *name = "ScrollingBanner";
    uint8_t brightness = 64;

public:
    FastLedExample(CRGB *ledArray, int num) : LightingProgram(ledArray, num, name) {}
    ~FastLedExample() {}

    void FillLEDsFromPaletteColors(uint8_t colorIndex);
    void init();
    void service();
};