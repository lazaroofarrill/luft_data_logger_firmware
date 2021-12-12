#include <sys/cdefs.h>
#include <Battery/batterySaving.h>
#include "Monitor/monitorProcess.h"
#include "Arduino.h"
#include "FreeRTOS.h"
#include "FileSystem/fileSystem.h"

TaskHandle_t monitorTask = NULL;
u_long GENERAL_READ_INTERVAL = 0;
u_long PRECIPITATION_READ_INTERVAL = 0;

Station station;

void monitorSetup() {
    GENERAL_READ_INTERVAL = globalConfiguration["Settings"]["generalReadDelay"].as<long>() * 60 * 1000;
    PRECIPITATION_READ_INTERVAL = globalConfiguration["Settings"]["precipitationReadDelay"].as<long>() * 60 * 1000;
    Serial.print("General read freq is ");
    Serial.println(GENERAL_READ_INTERVAL);
    humidityStart();
    lightStart();
    pressureStart();
    timeStart();
    xTaskCreatePinnedToCore(monitorLoop, "MONITOR_TSK", 50000,
                            NULL, 1, &monitorTask, 1);
}

[[noreturn]] void monitorLoop(void *param) {
    ulong lastGeneral = -GENERAL_READ_INTERVAL, lastPrecipitation = -PRECIPITATION_READ_INTERVAL;
    for (;;) {
        String data = "";
        if (abs((long) millis() - (long) lastGeneral) > GENERAL_READ_INTERVAL) {
            data += ";windSpeed=" + String(station.velocidad_viento_prueba());
            data += ";windDirection=" + String(station.dir_viento_prueba());
            data += ";windDirectionDegrees=" + String(station.dir_viento_degrees());
            data += ";humidity=" + String(station.humedad_prueba());
            data += ";temperature=" + String(station.temp_prueba());
            data += ";radiation=" + String(station.pira_prueba());
            data += ";pressure=" + String(station.presion_prueba());
            data += ";evapoTranspiration=" + String(station.evapo_prueba());
            data += ";rain=" + String(station.precipitationMM_prueba());
            data += timeRead();

            lastGeneral = millis();
        }
        if (abs((long) millis() - (long) lastPrecipitation) > PRECIPITATION_READ_INTERVAL) {
            data += timeRead();
            data += ";precipitation=" + String(station.precipitationMM_prueba());
            lastPrecipitation = millis();
        }

        if (data.length()) {
            logData(data);
        }
        delay(100);
    }
}
