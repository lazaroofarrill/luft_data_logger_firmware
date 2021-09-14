//
// Created by lazt on 4/23/21.
//

#include <FileSystem/fileSystem.h>
#include <WebServer/connection.h>
#include <Logger/logger.h>
#include "WebServer/webServerProcess.h"

TaskHandle_t serverTask;

void serverSetup() {
    initWifi();
    if (USE_SDCARD) {
        initSDCard();
    }
    initWebServer();
    startLogs();

    xTaskCreatePinnedToCore(
            serverLoop,
            "webServer",
            50000,
            NULL,
            1,
            &serverTask,
            1
    );
}

[[noreturn]] void serverLoop(void *param) {
    while (true) {
        server.handleClient();
        ws.loop();
        sessionWatch();
        delay(500);
    }
}
