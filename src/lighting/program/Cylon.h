#pragma once

#include "LightingProgram.h"
#include <ArduinoLog.h>

class Cylon : public LightingProgram
{
private:
    bool alternate = true;
    uint8_t hue = 0;
    int index = 0;

public:
    Cylon(CRGB *ledArray, int num) : LightingProgram(ledArray, num, "Cylon") {
        delayMs = 5;
    }
    ~Cylon() {}

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

    void servicePreShow()
    {
        
        leds[index] = ColorFromPalette(currentPalette, hue);//, brightness, currentBlend);
        hue += 3;
        if (alternate) {
            index++;
          if (index >= numLeds) {
              index = numLeds - 1;
              alternate = false;
          }
        } else {
            index--;
          if (index <= 0) {
              index = 0;
              alternate = true;
          }
        }
    }

    void servicePostShow()
    {
        fadeall();
    }
};