#include "FastExample.h"

void FastLedExample::FillLEDsFromPaletteColors(uint8_t colorIndex)
{
    for (int i = 0; i < numLeds; i++)
    {
        leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlend);
        colorIndex += 3;
    }
}

void FastLedExample::init()
{
    setBlendType(LINEARBLEND);
    setDelayMs(10);
}

void FastLedExample::servicePreShow()
{
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    FillLEDsFromPaletteColors(startIndex);
}