//
// Created by lazt on 4/23/21.
//

#ifndef BACKEND_FILESYSTEM_H
#define BACKEND_FILESYSTEM_H

#include "FS.h"
#include "SPIFFS.h"
#include "ArduinoJson.h"

extern DynamicJsonDocument globalConfiguration;


void initFileSystem();
void loadConf();
void saveConfig();


#endif //BACKEND_FILESYSTEM_H
