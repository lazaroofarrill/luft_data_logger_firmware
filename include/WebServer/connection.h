//
// Created by lazt on 4/23/21.
//

#ifndef BACKEND_CONNECTION_H
#define BACKEND_CONNECTION_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include "FileSystem/sdcard.h"
#include "FileSystem/fileSystem.h"
#include "stdlib_noniso.h"
#include <Update.h>
#include "Monitor/monitorProcess.h"
#include "WebSocketsServer.h"


#ifndef USE_SDCARD
#define USE_SDCARD true
#endif
#define REFRESH_INTERVAL 5

extern WebServer server;
extern WebSocketsServer ws;

void passCors();

void sendCors();

bool checkAuth();

bool authControl();

void initWifi();

void handleRoot();

void handleNotFound();

void configNet();

void configUser();

void getWiFis();

void sessionWatch();

void genSessionID();

void authenticate();

void initWebServer();

void handleUpload();

void resetConfig();

void onFinishUpload();

void webSocketEvent(byte num, WStype_t type, uint8_t *payload, size_t length);

String getStringVal(JsonObject obj);

bool getBooleanVal(JsonObject obj);

double getNumberVal(JsonObject obj);

#endif //BACKEND_CONNECTION_H
