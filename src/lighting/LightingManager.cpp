#include "LightingManager.h"
#include <ArduinoLog.h>
#include "Palettes.h"

LightingManager::LightingManager()
{
    FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_MA);
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)
        .setCorrection(TypicalLEDStrip);
}

LightingManager::~LightingManager()
{
}

void LightingManager::init()
{
    FastLED.clear();
    FastLED.setBrightness(brightness);
    FastLED.show();
    Log.traceln("Lighting Manager Init Complete");
}

void LightingManager::service()
{
    currentProgram->servicePreShow();
    FastLED.show();
    currentProgram->servicePostShow();
    if (currentProgram->getDelayMs() > 0)
    {
        delay(currentProgram->getDelayMs());
    }
    if (autoChangePalette)
    {
        EVERY_N_SECONDS(autoChangeSeconds)
        {
            chooseNextColorPalette();
        }
    }
}

bool LightingManager::setProgram(const char *programName)
{
    Log.traceln("setProgram: %s", programName);
    for (LightingProgram *p : ActivePrograms)
    {
        Log.traceln("Looking at program : %s", p->getName());
        if (strcmp(p->getName().c_str(), programName) == 0)
        {
            currentProgram = p;
            currentProgram->init();
            setPalette(currentPaletteName.c_str());
            Log.traceln("Set Current Program: %s", currentProgram->getName());
            return true;
        }
    }
    return false;
}

bool LightingManager::setPalette(const char *paletteName)
{
    Log.traceln("setPalette: %s", paletteName);
    for (const Palette *p : Palettes::ActivePaletteList)
    {
        if (strcmp(p->getName(), paletteName) == 0)
        {
            currentPaletteName = paletteName;
            if (currentProgram != NULL)
            {
                Log.traceln("Switching Color Palette: %s", p->getName());
                currentProgram->setCurrentPalette(p->getPalette());
            }
            return true;
        }
    }
    if (strcmp(paletteName, "working") == 0)
    {
        currentPaletteName = paletteName;
        Log.traceln("Using working palette");
        currentProgram->setCurrentPalette((TProgmemRGBPalette16 *)&workingPalette);
    }
    return false;
}

void LightingManager::setBrigthness(uint8_t newBrightness)
{
    brightness = newBrightness;
    FastLED.setBrightness(brightness);
}

void LightingManager::chooseNextColorPalette()
{
    const uint8_t numberOfPalettes = sizeof(Palettes::ActivePaletteList) / sizeof(Palettes::ActivePaletteList[0]);
    static uint8_t whichPalette = -1;
    whichPalette = addmod8(whichPalette, 1, numberOfPalettes);
    const Palette *palette = Palettes::ActivePaletteList[whichPalette];
    setPalette(palette->getName());
}

String LightingManager::getActivePalettesJsonStr()
{
    const uint8_t numberOfPalettes = sizeof(Palettes::ActivePaletteList) / sizeof(Palettes::ActivePaletteList[0]);
    String jsonStr;
    jsonStr += "\"palettes\":[";
    for (int i = 0; i < numberOfPalettes; i++)
    {
        const Palette *p = Palettes::ActivePaletteList[i];
        jsonStr += p->getPaletteJson();
        if (i < (numberOfPalettes - 1))
        {
            jsonStr += ",";
        }
    }
    jsonStr += "]";
    return jsonStr;
}

String LightingManager::getAvailablePalettesJsonStr()
{
    const uint8_t numberOfPalettes = sizeof(Palettes::AvailablePaletteList) / sizeof(Palettes::AvailablePaletteList[0]);
    String jsonStr;
    jsonStr += "\"palettes\":[";
    for (int i = 0; i < numberOfPalettes; i++)
    {
        const Palette *p = Palettes::AvailablePaletteList[i];
        jsonStr += p->getPaletteJson();
        if (i < (numberOfPalettes - 1))
        {
            jsonStr += ",";
        }
    }
    jsonStr += "]";
    return jsonStr;
}

String LightingManager::getActiveProgramsJsonStr()
{
    const uint8_t numberOfPrograms = sizeof(ActivePrograms) / sizeof(ActivePrograms[0]);
    String jsonStr;
    jsonStr += "\"programs\":[";
    for (int i = 0; i < numberOfPrograms; i++)
    {
        LightingProgram *p = ActivePrograms[i];
        jsonStr += p->getProgramJson();
        if (i < (numberOfPrograms - 1))
        {
            jsonStr += ",";
        }
    }
    jsonStr += "]";
    return jsonStr;
}

void LightingManager::setSingleColor(int color)
{
    for (int i = 0; i < 16; i++)
    {
        workingPalette[i] = color;
        setPalette("working");
    }
}