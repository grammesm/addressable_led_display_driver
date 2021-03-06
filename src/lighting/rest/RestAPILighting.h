#pragma once

#include "../LightingManager.h"
#include <Arduino.h>
#include "RestAPIEndpoints.h"
#include "Utils.h"

class RestAPILighting {

private:

    LightingManager& _lightingManager;

public:
    RestAPILighting(LightingManager& lightingManager);

    void setup(RestAPIEndpoints &endpoints);

    // Overall
    void apiStatus(String &reqStr, String &respStr);
    void apiBrightnessGet(String &reqStr, String &respStr);
    void apiBrightnessSet(String &reqStr, String &respStr);
    void apiProgramsGet(String &reqStr, String &respStr);
    void apiProgramSet(String &reqStr, String &respStr);
    void apiPaletteSet(String &reqStr, String &respStr);
    void apiAvailablePalettesGet(String &reqStr, String &respStr);
    void apiActivePalettesGet(String &reqStr, String &respStr);
    void apiSingleColorSet(String &reqStr, String &respStr);
};