#include "LightingManager.h"
#include <ArduinoLog.h>

LightingManager::LightingManager() {
    FastLED.setMaxPowerInVoltsAndMilliamps( VOLTS, MAX_MA);
    FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds, NUM_LEDS)
        .setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}

LightingManager::~LightingManager() {
}

void LightingManager::setProgram(LightingProgram* program) {
    currentProgram = program;
    currentProgram->init();
    Log.traceln("Set Current Program: %s", currentProgram->getName());
}

void LightingManager::setPalette(const TProgmemRGBPalette16* pallete) {
    if (currentProgram != NULL) {
        currentProgram->setCurrentPalette(pallete);
    }
}

void LightingManager::init() {
    FastLED.clear();
    FastLED.show();
    Log.traceln("Lighting Manager Init Complete");
}

void LightingManager::service() {
    currentProgram->service();
    if (currentProgram->getDelayMs() > 0) {
        delay(currentProgram->getDelayMs());
    }
}