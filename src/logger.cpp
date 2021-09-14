//
// Created by lazt on 4/23/21.
//

#include "Logger/logger.h"

String dataToLog[LOG_SPLIT];
int logCounter = 0;
long loggedLast = 0;


void startLogs() {
    if (!cardMounted) return;
    if (!SD.mkdir("/logs")) {
        Serial.println("couldn't create logs directory");
    };
}


int lastLog() {
    File logRoot = SD.open("/logs");
    if (!logRoot) {
        Serial.println("couldn't open logs directory");
        return -1;
    }
    int count = 0;
    while (true) {
        File entry = logRoot.openNextFile();
        if (!entry) {
            break;
        }
        count++;
        // Serial.println(entry.name());
    }
    logRoot.close();

    return count;
}


void writeLog() {
    if (!cardMounted) {
        Serial.println("Card not mounted");
        return;
    }
    File log = SD.open("/logs/" + String(lastLog() + 1), "w");
    if (!log) {
        Serial.println("couldn't open file for writing");
        return;
    }

    for (int i = 0; i < logCounter; i++) {
        log.println(dataToLog[i]);
    }

    Serial.println("data logged succesfully");
    log.close();
}


void logData(String data) {
    ws.broadcastTXT(data);

    DateTime now = RTC.now();
    String path = "/logs/";
    path += now.year();
    path += "-";
    path += now.month();
    path += "-";
    path += now.day();

    File log = SD.open(path, FILE_APPEND);
    if (!log) {
        Serial.println("couldn't open file for writing");
        return;
    }
    log.println(data);
    Serial.println("data logged succesfully");
    log.close();
}

