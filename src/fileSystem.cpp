//
// Created by lazt on 4/23/21.
//

#include "FileSystem/fileSystem.h"

DynamicJsonDocument doc = DynamicJsonDocument(1024);


void initFileSystem() {
    if (!SPIFFS.begin()) {
        Serial.println("Couldn't init filesystem");
    }
//    double toMB = 1 / (1024 * 1024);
    ulong total = SPIFFS.totalBytes();
    ulong used = SPIFFS.usedBytes();
    double free = (double) used / (double) total * 100.0;

    Serial.printf("Loading SPIFFS filesystem: \n"
                  "Capacity: %lu\n"
                  "Used: %lu\n"
                  "%.2f%% used\n",
                  total, used, free);
}

void saveConfig() {
    File file = SPIFFS.open("/config.json", "w");
    if (!file) {
        Serial.println("couldn't open file");
        return;
    }

    serializeJsonPretty(doc, file);
    file.close();
}

void loadConf() {
    File file = SPIFFS.open("/config.json", "r");
    if (!file) {
        Serial.println("couldn't open file");
        return;
    }

    DeserializationError error = deserializeJson(doc, file);
    file.close();
    if (error) {
        Serial.println("error loading json file");
        Serial.println(error.c_str());
        return;
    }
}
