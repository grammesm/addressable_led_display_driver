#pragma once

#include "program/LightingProgram.h"
#include "program/FastExample.h"
#include "program/Twinkle.h"
#include "program/Breathe.h"
#include "program/Cylon.h"
#include "program/SingleColor.h"

#include "Palette.h"

#define NUM_LEDS      300
#define LED_TYPE   WS2811
#define COLOR_ORDER   RGB
#define LED_PIN        4
#define VOLTS          12
#define MAX_MA       4000

class LightingManager {
// TODO store config/brightness in NVRAM
private:
    LightingProgram* currentProgram;
    String currentPaletteName;
    uint8_t brightness = 128;
    CRGB* leds = new CRGB[NUM_LEDS];
    CRGBPalette16 workingPalette;

    bool autoChangePalette = false;
    int autoChangeSeconds = 30;


    LightingProgram *fastLedExample = new FastLedExample(leds, NUM_LEDS);
    LightingProgram *twinkle = new Twinkle(leds, NUM_LEDS);
    LightingProgram *breathe = new Breathe(leds, NUM_LEDS);
    LightingProgram *cylon = new Cylon(leds, NUM_LEDS);
    LightingProgram *singleColor = new SingleColor(leds, NUM_LEDS);

    LightingProgram* ActivePrograms[5] = {
        fastLedExample,
        twinkle,
        breathe,
        cylon,
        singleColor
    };

public:

    LightingManager();
    ~LightingManager();

    bool setProgram(const char* programName);
    bool setPalette(const char* paletteName);
    void setBrigthness(uint8_t newBrightness);
    uint8_t getBrightness() {
        return brightness;
    }
    void setDelayMs(int delay) {
        if (currentProgram != NULL) {
            currentProgram->setDelayMs(delay);
        }
    }

    String getPaletteListJson();
    void init();
    void service();

    void chooseNextColorPalette();

    void setSingleColor(int color);

    String getAvailablePalettesJsonStr();
    String getActivePalettesJsonStr();
    String getActiveProgramsJsonStr();
};