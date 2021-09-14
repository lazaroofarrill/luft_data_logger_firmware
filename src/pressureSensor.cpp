//
// Created by lazt on 4/23/21.
//

#include "Monitor/pressureSensor.h"

Adafruit_BMP085 bmp;

bool ok = false;

void pressureStart() {
    if (!bmp.begin()) {
        Serial.println("Could not find a valid BMP085 sensor, check wiring!");
        return;
    }
    ok = true;
}

String pressureRead() {
    String vars = "";
    if (ok) {
        auto temperature_c = bmp.readTemperature();
        Serial.print("Temperature = ");
        Serial.print(temperature_c);
        Serial.print(" *C ");

        vars += ",temperature_c=" + String(temperature_c);

        auto pressure = bmp.readPressure();
        vars += ",pressure=" + String(pressure);
        Serial.print("Pressure = ");
        Serial.print(pressure);
        Serial.print(" Pa ");

        // Calculate altitude assuming 'standard' barometric
        // pressure of 1013.25 millibar = 101325 Pascal
        auto altitude = bmp.readAltitude();
        vars += ",altitude=" + String(altitude);
        Serial.print("Altitude = ");
        Serial.print(altitude);
        Serial.print(" meters ");

        auto seaLevelPressure = bmp.readSealevelPressure();
        vars += ",pressure_sl=" + String(seaLevelPressure);
        Serial.print("Pressure at sealevel (calculated) = ");
        Serial.print(seaLevelPressure);
        Serial.println(" Pa ");

        // you can get a more precise measurement of altitude
        // if you know the current sea level pressure which will
        // vary with weather and such. If it is 1015 millibars
        // that is equal to 101500 Pascals.
//        Serial.print("Real altitude = ");
//        Serial.print(bmp.readAltitude(101500));
//        Serial.println(" meters");
    }
    return vars;
}
