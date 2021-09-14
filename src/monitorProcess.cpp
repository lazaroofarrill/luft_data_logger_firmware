#include <sys/cdefs.h>
#include <Battery/batterySaving.h>
#include "Monitor/monitorProcess.h"
#include "Arduino.h"
#include "FreeRTOS.h"
#include "FileSystem/fileSystem.h"

TaskHandle_t monitorTask = NULL;
u_long LOG_INTERVAL = 0;

void monitorSetup() {
    LOG_INTERVAL = (long) getNumberVal(doc["sensors"]["readFreq"]) * 60 * 1000;
    Serial.print("Read freq is");
    Serial.println(LOG_INTERVAL);
    humidityStart();
    lightStart();
    pressureStart();
    timeStart();
    xTaskCreatePinnedToCore(monitorLoop, "MONITOR_TSK", 50000,
                            NULL, 1, &monitorTask, 1);
}

[[noreturn]] void monitorLoop(void *param) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
        String data = "";
        data += humidityRead();
        data += lightRead();
        data += pressureRead();
        data += timeRead();
        logData(data);
        if (batterySavingActivated && !digitalRead(POWER_PIN)) {
            esp_deep_sleep(LOG_INTERVAL * 1000);
        } else {
            vTaskDelayUntil(&xLastWakeTime, LOG_INTERVAL / portTICK_RATE_MS);
            xLastWakeTime = xTaskGetTickCount();
        }
    }
}
