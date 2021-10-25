#include "LightingManager.h"
#include <ArduinoLog.h>
#include "Palettes.h"

static const char *MODULE_PREFIX = "LightingManager: ";

LightingManager::LightingManager(ConfigBase &lightingConfig) : _lightingConfig(lightingConfig)
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
    // If there is no LED data stored, set to default
    String lightingConfigStr = _lightingConfig.getConfigString();
    if (lightingConfigStr.length() == 0 || lightingConfigStr.equals("{}"))
    {
        Log.trace("%sNo Lighting Data Found in NV Storge, Defaulting\n", MODULE_PREFIX);
        // Default to Twinkle, RedWhiteBlue
        currentProgram = twinkle;
        setPalette(Palettes::RedWhiteBlue->getName());
    } else {
        Log.traceln("%sLighting Config Found: %s", MODULE_PREFIX, lightingConfigStr.c_str());
        String currentProgramStr = _lightingConfig.getString("program", "Twinkle");
        currentPaletteName = _lightingConfig.getString("palette", "RedWhiteBlue");
        brightness = _lightingConfig.getLong("brightness", 128);
        setProgram(currentProgramStr.c_str());
        setPalette(currentPaletteName.c_str());
    }

    FastLED.clear();
    FastLED.setBrightness(brightness);
    FastLED.show();
    Log.traceln("%sLighting Manager Init Complete", MODULE_PREFIX);
}

void LightingManager::service()
{
    if (currentProgram == NULL) {
        return;
    }
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

void LightingManager::updateNvRam()
{
    String jsonStr;
    jsonStr += "{";
    jsonStr += "\"program\":";
    jsonStr += "\"" + currentProgram->getName() + "\"";
    jsonStr += ",";
    jsonStr += "\"palette\":";
    jsonStr += "\"" + currentPaletteName + "\"";
    jsonStr += ",";
    jsonStr += "\"brightness\":";
    jsonStr += brightness;
    jsonStr += "}";
    _lightingConfig.setConfigData(jsonStr.c_str());
    _lightingConfig.writeConfig();
    Log.trace("%supdateNvRam() : wrote %s\n", MODULE_PREFIX, _lightingConfig.getConfigCStrPtr());
    
}

bool LightingManager::setProgram(const char *programName)
{
    Log.traceln("%ssetProgram: %s", MODULE_PREFIX, programName);
    for (LightingProgram *p : ActivePrograms)
    {
        Log.traceln("%sLooking at program : %s", MODULE_PREFIX, p->getName().c_str());
        if (strcmp(p->getName().c_str(), programName) == 0)
        {
            currentProgram = p;
            currentProgram->init();
            setPalette(currentPaletteName.c_str());
            Log.traceln("%sSet Current Program: %s", MODULE_PREFIX, currentProgram->getName().c_str());
            updateNvRam();
            return true;
        }
    }
    return false;
}

bool LightingManager::setPalette(const char *paletteName)
{
    Log.traceln("%ssetPalette: %s", MODULE_PREFIX, paletteName);
    for (const Palette *p : Palettes::ActivePaletteList)
    {
        if (strcmp(p->getName(), paletteName) == 0)
        {
            currentPaletteName = paletteName;
            if (currentProgram != NULL)
            {
                Log.traceln("%sSwitching Color Palette: %s", MODULE_PREFIX, p->getName());
                currentProgram->setCurrentPalette(p->getPalette());
            }
            updateNvRam();
            return true;
        }
    }
    if (strcmp(paletteName, "working") == 0)
    {
        currentPaletteName = paletteName;
        updateNvRam();
        Log.traceln("%sUsing working palette", MODULE_PREFIX);
        currentProgram->setCurrentPalette((TProgmemRGBPalette16 *)&workingPalette);
    }
    return false;
}

void LightingManager::setBrigthness(uint8_t newBrightness)
{
    brightness = newBrightness;
    FastLED.setBrightness(brightness);
    updateNvRam();
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