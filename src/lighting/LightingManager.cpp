#include "LightingManager.h"
#include <ArduinoLog.h>

LightingManager::LightingManager() {
    FastLED.setMaxPowerInVoltsAndMilliamps( VOLTS, MAX_MA);
    FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds, NUM_LEDS)
        .setCorrection(TypicalLEDStrip);
}

LightingManager::~LightingManager() {
}

void LightingManager::setProgram(LightingProgram* program) {
    currentProgram = program;
    currentProgram->init();
    Log.traceln("Set Current Program: %s", currentProgram->getName());
}

void LightingManager::setPalette(const Palette* palette) {
    if (currentProgram != NULL) {
        Log.traceln("Switching Color Palette: %s", palette->getName());
        currentProgram->setCurrentPalette(palette->getPalette());
    }
}

void LightingManager::setBrigthness(uint8_t newBrightness) {
    brightness = newBrightness;
    FastLED.setBrightness(brightness);
}

void LightingManager::init() {
    FastLED.clear();
    FastLED.setBrightness(brightness);
    FastLED.show();
    Log.traceln("Lighting Manager Init Complete");
}

void LightingManager::service() {
    currentProgram->service();
    if (currentProgram->getDelayMs() > 0) {
        delay(currentProgram->getDelayMs());
    }
}