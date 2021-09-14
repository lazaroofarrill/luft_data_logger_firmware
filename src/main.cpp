#include <Wire.h>
#include <Battery/batterySaving.h>
#include "Arduino.h"
#include "WebServer/webServerProcess.h"
#include "Monitor/monitorProcess.h"


void setup() {
    Serial.begin(19200);
    if (!Wire.begin()) {
        Serial.println("Failed to init i2c interface");
    }

    Serial.println("update 0");
    initFileSystem();
    loadConf();
    serverSetup();
}

void loop() {
    delay(10000);
}