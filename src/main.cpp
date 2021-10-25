
#define SYSTEM_TYPE_NAME "LEDDriver"
const char *systemType = "LEDDriver";

// System version
const char *systemVersion = "0.000.001";

// Build date
const char *buildDate = __DATE__;

// Build date
const char *buildTime = __TIME__;

// Arduino
#include <Arduino.h>

// Logging
#include <ArduinoLog.h>

// WiFi
#include <WiFi.h>

// Utils
#include <Utils.h>

// Status LED
#include "StatusIndicator.h"
StatusIndicator wifiStatusLed;

// Config
#include "ConfigNVS.h"
#include "ConfigFile.h"

// WiFi Manager
#include "WiFiManager.h"
WiFiManager wifiManager;

// NTP Client
#include "NTPClient.h"
NTPClient ntpClient;

// File manager
#include "FileManager.h"
FileManager fileManager;

// API Endpoints
#include "RestAPIEndpoints.h"
RestAPIEndpoints restAPIEndpoints;

// Web server
#include "WebServer.h"
WebServer webServer;

// Firmware update
#include <RdOTAUpdate.h>
RdOTAUpdate otaUpdate;

// Hardware config
static const char *hwConfigJSON = {
    "{"
    "\"unitName\":" SYSTEM_TYPE_NAME ","
    "\"wifiEnabled\":1,"
    "\"webServerEnabled\":1,"
    "\"webServerPort\":80,"
    "\"OTAUpdate\":{\"enabled\":1,\"directOk\":1},"
    "\"serialConsole\":{\"portNum\":0},"
    "\"commandSerial\":{\"portNum\":-1,\"baudRate\":115200},"
    "\"ntpConfig\":{\"ntpServer\":\"pool.ntp.org\", \"gmtOffsetSecs\":0, \"dstOffsetSecs\":0},"
    "}"};

// Config for hardware
ConfigBase hwConfig(hwConfigJSON);

// Config for robot control
ConfigNVS robotConfig("robot", 2000);

// Config for WiFi
ConfigNVS wifiConfig("wifi", 100);

// Config for NTP
ConfigNVS ntpConfig("ntp", 200);

// Config for network logging
ConfigNVS netLogConfig("netLog", 200);

// Config for CommandScheduler
ConfigNVS cmdSchedulerConfig("cmdSched", 500);

// Config for LightingManager
ConfigNVS lightingConfig("lightMgr", 250);

// CommandScheduler - time-based commands
#include "CommandScheduler.h"
CommandScheduler commandScheduler;

// CommandSerial port - used to monitor activity remotely and send commands
#include "CommandSerial.h"
CommandSerial commandSerial(fileManager);

// Serial console - for configuration
#include "SerialConsole.h"
SerialConsole serialConsole;

// NetLog
#include "NetLog.h"
NetLog netLog(Serial, commandSerial);

// REST API System
#include "RestAPISystem.h"
RestAPISystem restAPISystem(wifiManager,
                            otaUpdate, netLog, fileManager, ntpClient,
                            commandScheduler,
                            systemType, systemVersion);
// Debug loop used to time main loop
#include "DebugLoopTimer.h"

#include "lighting/LightingManager.h"
LightingManager lightingManager(lightingConfig);

#include "lighting/program/LightingProgram.h"

// REST API Lighting
#include "lighting/rest/RestAPILighting.h"
RestAPILighting restAPILighting(lightingManager);


// Debug loop timer and callback function
void debugLoopInfoCallback(String &infoStr)
{
    if (wifiManager.isEnabled())
        infoStr = wifiManager.getHostname() + " V" + String(systemVersion) + " SSID " + WiFi.SSID() + " IP " + WiFi.localIP().toString() + " Heap " + String(ESP.getFreeHeap());
    else
        infoStr = "WiFi Disabled, Heap " + String(ESP.getFreeHeap());
}
DebugLoopTimer debugLoopTimer(10000, debugLoopInfoCallback);

void setup()
{
    // Logging
    Serial.begin(115200);
    Log.begin(LOG_LEVEL_TRACE, &netLog);

    // Message
    Log.notice("%s %s (built %s %s)\n", systemType, systemVersion, buildDate, buildTime);

    // Robot config
    robotConfig.setup();

    // Status Led
    wifiStatusLed.setup(&robotConfig, "robotConfig/wifiLed");

    // File system
    fileManager.setup(robotConfig, "robotConfig/fileManager");

    // WiFi Config
    wifiConfig.setup();

    // NTP Config
    ntpConfig.setup();

    // NetLog Config
    netLogConfig.setup();

    // Command scheduler
    commandScheduler.setup(&robotConfig, "cmdSched", &cmdSchedulerConfig, restAPIEndpoints);

    // Serial console
    serialConsole.setup(hwConfig, restAPIEndpoints);

    // WiFi Manager
    wifiManager.setup(hwConfig, &wifiConfig, systemType, &wifiStatusLed);

    // NTP Client
    ntpClient.setup(&hwConfig, "ntpConfig", &ntpConfig);

    // Firmware update
    otaUpdate.setup(hwConfig, systemType, systemVersion);

    // Add API endpoints
    restAPISystem.setup(restAPIEndpoints);
    //restAPIRobot.setup(restAPIEndpoints);
    restAPILighting.setup(restAPIEndpoints);

    // Web server
    webServer.setup(hwConfig);
    //webServer.addStaticResources(__webAutogenResources, __webAutogenResourcesCount);
    webServer.addEndpoints(restAPIEndpoints);
    //webServer.serveStaticFiles("/files/spiffs", "/spiffs/");
    //webServer.serveStaticFiles("/files/sd", "/sd/");
    webServer.enableAsyncEvents("/events");

    // Network logging
    netLog.setup(&netLogConfig, wifiManager.getHostname().c_str());

    lightingConfig.setup();
    lightingManager.init();
    delay(3000);
    // lightingManager.setProgram("Twinkle");
    // lightingManager.setPalette("Haloween");



    // Add debug blocks
    debugLoopTimer.blockAdd(0, "LoopTimer");
    debugLoopTimer.blockAdd(1, "WiFi");
    debugLoopTimer.blockAdd(2, "Web");
    debugLoopTimer.blockAdd(3, "SysAPI");
    debugLoopTimer.blockAdd(4, "Console");
    debugLoopTimer.blockAdd(6, "OTA");
    debugLoopTimer.blockAdd(7, "NetLog");
    debugLoopTimer.blockAdd(8, "NTP");
    debugLoopTimer.blockAdd(9, "Sched");
    debugLoopTimer.blockAdd(10, "WifiLed");
    debugLoopTimer.blockAdd(15, "LightingManager");
}

uint8_t brightness = 0;
void loop()
{

    // Debug loop Timing
    debugLoopTimer.blockStart(0);
    debugLoopTimer.service();
    debugLoopTimer.blockEnd(0);

    // Service WiFi
    debugLoopTimer.blockStart(1);
    wifiManager.service();
    debugLoopTimer.blockEnd(1);

    // Service the web server
    if (wifiManager.isConnected())
    {
        // Begin the web server
        debugLoopTimer.blockStart(2);
        webServer.begin(true);
        debugLoopTimer.blockEnd(2);
    }

    // Service the system API (restart)
    debugLoopTimer.blockStart(3);
    restAPISystem.service();
    debugLoopTimer.blockEnd(3);

    // Serial console
    debugLoopTimer.blockStart(4);
    serialConsole.service();
    debugLoopTimer.blockEnd(4);

    // Service OTA Update
    debugLoopTimer.blockStart(6);
    otaUpdate.service();
    debugLoopTimer.blockEnd(6);

    // Service NetLog
    debugLoopTimer.blockStart(7);
    netLog.service(serialConsole.getXonXoff());
    debugLoopTimer.blockStart(7);

    // Service NTP
    debugLoopTimer.blockStart(8);
    ntpClient.service();
    debugLoopTimer.blockEnd(8);

    // Service command scheduler
    debugLoopTimer.blockStart(9);
    commandScheduler.service();
    debugLoopTimer.blockEnd(9);

    // Service the status LED
    debugLoopTimer.blockStart(10);
    wifiStatusLed.service();
    debugLoopTimer.blockEnd(10);

    debugLoopTimer.blockStart(15);
    // EVERY_N_SECONDS(SECONDS_PER_PALETTE)
    // {
    //     lightingManager.chooseNextColorPalette();
    // }
    // EVERY_N_MILLISECONDS(10) {
    //     lightingManager.setBrigthness(brightness++);
    // }
    lightingManager.service();
    debugLoopTimer.blockEnd(15);
}
