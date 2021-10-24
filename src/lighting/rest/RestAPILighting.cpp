#include "RestAPILighting.h"

static const char* MODULE_PREFIX = "RestAPILighting: ";

RestAPILighting::RestAPILighting(LightingManager& lightingManager) : 
        _lightingManager(lightingManager) {

        }

void RestAPILighting::setup(RestAPIEndpoints &endpoints)
{
    endpoints.addEndpoint("getLedBrightness", RestAPIEndpointDef::ENDPOINT_CALLBACK, RestAPIEndpointDef::ENDPOINT_GET, 
                    std::bind(&RestAPILighting::apiBrightnessGet, this, std::placeholders::_1, std::placeholders::_2),
                    "Get current LED brightness");
    endpoints.addEndpoint("setLedBrightness", RestAPIEndpointDef::ENDPOINT_CALLBACK, RestAPIEndpointDef::ENDPOINT_GET, 
                    std::bind(&RestAPILighting::apiBrightnessSet, this, std::placeholders::_1, std::placeholders::_2),
                    "Set current LED brightness");
    endpoints.addEndpoint("getLedPrograms", RestAPIEndpointDef::ENDPOINT_CALLBACK, RestAPIEndpointDef::ENDPOINT_GET, 
                    std::bind(&RestAPILighting::apiProgramsGet, this, std::placeholders::_1, std::placeholders::_2),
                    "Gets available LED programs");
    endpoints.addEndpoint("setLedProgram", RestAPIEndpointDef::ENDPOINT_CALLBACK, RestAPIEndpointDef::ENDPOINT_GET, 
                    std::bind(&RestAPILighting::apiProgramSet, this, std::placeholders::_1, std::placeholders::_2),
                    "Set current LED program");
    endpoints.addEndpoint("setLedColorPalette", RestAPIEndpointDef::ENDPOINT_CALLBACK, RestAPIEndpointDef::ENDPOINT_GET, 
                    std::bind(&RestAPILighting::apiPaletteSet, this, std::placeholders::_1, std::placeholders::_2),
                    "Set color palette for current program if supported");
    endpoints.addEndpoint("getColorPalettes", RestAPIEndpointDef::ENDPOINT_CALLBACK, RestAPIEndpointDef::ENDPOINT_GET, 
                    std::bind(&RestAPILighting::apiPalettesGet, this, std::placeholders::_1, std::placeholders::_2),
                    "Gets available color palettes");
}

void RestAPILighting::apiBrightnessGet(String &reqStr, String &respStr)
{
    // Get brightness
    uint8_t currentBrightness = _lightingManager.getBrightness();

    String brightnessStr;
    brightnessStr += "\"brightness\":";
    brightnessStr += currentBrightness;

    Log.trace("%sCurrent Brightness: %s\n", MODULE_PREFIX, brightnessStr.c_str());

    Utils::setJsonBoolResult(respStr, true, brightnessStr.c_str());
}

void RestAPILighting::apiBrightnessSet(String &reqStr, String &respStr)
{
    bool result = false;

    int newBrightness = RestAPIEndpoints::getNthArgStr(reqStr.c_str(), 1).toInt();

    Log.traceln("%sSet Brightness: %s = %d", MODULE_PREFIX, reqStr.c_str(), newBrightness);

    _lightingManager.setBrigthness(newBrightness);
    result = true;

    Utils::setJsonBoolResult(respStr, result);
}

void RestAPILighting::apiProgramSet(String &reqStr, String &respStr)
{
    bool result = false;

    String newProgram = RestAPIEndpoints::getNthArgStr(reqStr.c_str(), 1);

    Log.traceln("%sSet Program: %s", MODULE_PREFIX, reqStr.c_str(), newProgram);

    result = _lightingManager.setProgram(newProgram.c_str());

    Utils::setJsonBoolResult(respStr, result);
}

void RestAPILighting::apiProgramsGet(String &reqStr, String &respStr)
{
    bool result = false;

    String programJson = _lightingManager.getActiveProgramsJsonStr();

    Utils::setJsonBoolResult(respStr, true, programJson.c_str());
}

void RestAPILighting::apiPaletteSet(String &reqStr, String &respStr)
{
    bool result = false;

    String newPalette = RestAPIEndpoints::getNthArgStr(reqStr.c_str(), 1);

    Log.traceln("%sSet Palette: %s", MODULE_PREFIX, reqStr.c_str(), newPalette);

    result = _lightingManager.setPalette(newPalette.c_str());

    Utils::setJsonBoolResult(respStr, result);
}

void RestAPILighting::apiPalettesGet(String &reqStr, String &respStr)
{
    bool result = false;

    String paletteJson = _lightingManager.getActivePalettesJsonStr();

    Utils::setJsonBoolResult(respStr, true, paletteJson.c_str());
}
