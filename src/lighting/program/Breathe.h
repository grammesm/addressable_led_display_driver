#pragma once

#include "LightingProgram.h"

class Breathe : public LightingProgram
{
private:
    const char *name = "Breathe";
    uint8_t brightness = 128;

    float pulseSpeed = 0.5; // Larger value gives faster pulse.

    uint8_t hueA = 15;      // Start hue at valueMin.
    uint8_t satA = 230;     // Start saturation at valueMin.
    float valueMin = 120.0; // Pulse minimum value (Should be less then valueMax).

    uint8_t hueB = 95;      // End hue at valueMax.
    uint8_t satB = 255;     // End saturation at valueMax.
    float valueMax = 255.0; // Pulse maximum value (Should be larger then valueMSin).

    uint8_t hue = hueA;                               // Do Not Edit
    uint8_t sat = satA;                               // Do Not Edit
    float val = valueMin;                             // Do Not Edit
    uint8_t hueDelta = hueA - hueB;                   // Do Not Edit
    float delta = (valueMax - valueMin) / 2.35040238; // Do Not Edit

public:
    Breathe(CRGB *ledArray, int num) : LightingProgram(ledArray, num, name) {}
    ~Breathe() {}
    void init();
    void service();
};