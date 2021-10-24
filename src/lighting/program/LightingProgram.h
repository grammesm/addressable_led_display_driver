#pragma once

#include <FastLED.h>
#include <ArduinoLog.h>

/**
 * Defines a lighting program interface
 */
class LightingProgram
{
protected:
    CRGB *leds;
    int numLeds;
    CRGBPalette16 currentPalette;
    TBlendType currentBlend;
    int delayMs = 0;

public:
    LightingProgram(CRGB *ledArray, int num)
    {
        leds = ledArray;
        numLeds = num;
    }
    ~LightingProgram() {}
    virtual const char *getName() = 0;
    virtual void init() = 0;
    virtual void service() = 0;
    virtual void executePostShow() {}
    void setDelayMs(int newDelay)
    {
        delayMs = newDelay;
    }
    int getDelayMs()
    {
        return delayMs;
    }
    virtual void setCurrentPalette(const TProgmemRGBPalette16 *palette)
    {
        Log.traceln("Lighting Program: setCurrentPalette:");
        currentPalette = *palette;
    }
    virtual void setBlendType(TBlendType blendType)
    {
        Log.traceln("Lighting Program: setBlendType:");
        currentBlend = blendType;
    }
};