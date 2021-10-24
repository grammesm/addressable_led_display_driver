#pragma once

#include "LightingProgram.h"
#include <ArduinoLog.h>

class Cylon : public LightingProgram
{
private:
    const char *name = "Cylon";
    bool alternate = true;
    uint8_t hue = 0;

public:
    Cylon(CRGB *ledArray, int num) : LightingProgram(ledArray, num) {}
    ~Cylon() {}

    const char *getName()
    {
        return name;
    }

    void fadeall()
    {
        for (int i = 0; i < numLeds; i++)
        {
            leds[i].nscale8(250);
        }
    }

    void init()
    {
    }

    void service()
    {
        // First slide the led in one direction
        for (int i = 0; i < numLeds; i++)
        {
            // Set the i'th led to red
            leds[i] = CHSV(hue++, 255, 255);
            // Show the leds
            FastLED.show();
            // now that we've shown the leds, reset the i'th led to black
            // leds[i] = CRGB::Black;
            fadeall();
            // Wait a little bit before we loop around and do it again
            delay(10);
        }

        // Now go in the other direction.
        for (int i = (numLeds)-1; i >= 0; i--)
        {
            // Set the i'th led to red
            leds[i] = CHSV(hue++, 255, 255);
            // Show the leds
            FastLED.show();
            // now that we've shown the leds, reset the i'th led to black
            // leds[i] = CRGB::Black;S
            fadeall();
            // Wait a little bit before we loop around and do it again
            delay(10);
        }
    }
};