//
// Created by lazt on 4/23/21.
//

#include "Monitor/lightSensor.h"

BH1750 lightMeter(LIGHT_SENSOR_ADDRESS);

bool lightReady = false;

void lightStart() {
    if ((lightReady = lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE))) {
        Serial.println(F("BH1750 Advanced begin"));
    } else {
        Serial.println(F("Error initializing BH1750"));
    }
}

String lightRead() {
    String vars = "";
    if (lightReady && lightMeter.measurementReady()) {
        float lux = lightMeter.readLightLevel();
        vars += ",light=" + String(lux);
        Serial.print("Light: ");
        Serial.print(lux);
        Serial.println(" lx");
    }
    return vars;
}
