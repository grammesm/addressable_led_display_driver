#include "Breathe.h"

void Breathe::init()
{
}

void Breathe::service()
{
    float dV = ((exp(sin(pulseSpeed * millis() / 2000.0 * PI)) - 0.36787944) * delta);
    val = valueMin + dV;
    hue = map(val, valueMin, valueMax, rgb2hsv_approximate(currentPalette[0]).hue, rgb2hsv_approximate(currentPalette[15]).hue); // Map hue based on current val
    //hue = map(val, valueMin, valueMax, hueA, hueB); // Map hue based on current val
    sat = map(val, valueMin, valueMax, satA, satB); // Map sat based on current val

    for (int i = 0; i < numLeds; i++)
    {
        leds[i] = CHSV(hue, sat, val);

        // You can experiment with commenting out these dim8_video linesS
        // to get a different sort of look.
        leds[i].r = dim8_video(leds[i].r);
        leds[i].g = dim8_video(leds[i].g);
        leds[i].b = dim8_video(leds[i].b);
    }
    FastLED.show();
}