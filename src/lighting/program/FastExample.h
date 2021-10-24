#pragma once

#include "LightingProgram.h"

class FastLedExample : public LightingProgram
{
private:
    const char *name = "ScrollingBanner";
    uint8_t brightness = 64;

public:
    FastLedExample(CRGB *ledArray, int num) : LightingProgram(ledArray, num) {}
    ~FastLedExample() {}

    const char *getName()
    {
        return name;
    }

    void FillLEDsFromPaletteColors(uint8_t colorIndex);
    void init();
    void service();
};