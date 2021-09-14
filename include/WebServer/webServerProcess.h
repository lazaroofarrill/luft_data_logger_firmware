//
// Created by lazt on 4/23/21.
//

#ifndef BACKEND_WEBSERVERPROCESS_H
#define BACKEND_WEBSERVERPROCESS_H

#include "Arduino.h"
#include "WiFi.h"
#include "WebServer.h"
#include "WiFiClient.h"
#include "WiFiAP.h"
#include "WebSocketsServer.h"


void serverSetup();

[[noreturn]] void serverLoop(void *);


#endif //BACKEND_WEBSERVERPROCESS_H
