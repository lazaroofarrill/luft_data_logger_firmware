//
// Created by lazt on 4/23/21.
//

#ifndef BACKEND_LOGGER_H
#define BACKEND_LOGGER_H

#include "FileSystem/sdcard.h"
#include "WebServer/connection.h"
#include <ArduinoJson.h>

#define MIN_TO_MILLS 1000*60
//#define LOG_INTERVAL 5*1000
#define LOG_SPLIT 288
#define MAX_FAILED_READS 5

void startLogs();

int lastLog();

void writeLog();

void logData(String data);


#endif //BACKEND_LOGGER_H
