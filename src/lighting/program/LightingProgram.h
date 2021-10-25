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
    String _name;
    CRGBPalette16 currentPalette;
    TBlendType currentBlend = LINEARBLEND;
    int delayMs = 0;

public:
    LightingProgram(CRGB *ledArray, int num, String name) {
        leds = ledArray;
        numLeds = num;
        _name = name;
    }
    ~LightingProgram() {}
    virtual void init() = 0;
    virtual void servicePreShow() = 0;
    const String getName() const
    {
        return _name;
    }
    virtual void servicePostShow() {}
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
        currentPalette = *palette;
    }
    virtual void setBlendType(TBlendType blendType)
    {
        Log.traceln("Lighting Program: setBlendType:");
        currentBlend = blendType;
    }
    String getProgramJson()
    {
        String jsonStr;
        jsonStr += "{";
        jsonStr += "\"name\":";
        jsonStr += "\"";
        jsonStr += _name;
        jsonStr += "\"";
        jsonStr += "}";
        return jsonStr;
    }
};