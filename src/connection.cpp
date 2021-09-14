//
// Created by lazt on 4/23/21.
//

#include <Battery/batterySaving.h>
#include "WebServer/connection.h"

WebServer server(80);
WebSocketsServer ws(81);

String networks = "[";

String sessionId = "";

unsigned long refreshSession = 0;


void passCors() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Max-Age", "600");
    server.sendHeader("Access-Control-Allow-Methods", "*");
    server.sendHeader("Access-Control-Allow-Headers", "*");
}


bool checkAuth() {
    if (server.hasHeader("Authorization")) {
        String token = "Bearer ";
        if (sessionId.length() == 0)
            return false;

        token += sessionId;
        if (server.header("Authorization").indexOf(token) != -1) {
            Serial.println("correct auth");
            refreshSession = millis();
            return true;
        }
    }
    return false;
}


bool authControl() {
    if (!checkAuth()) {
        passCors();
        server.send(401, "text/plain", "user not authorized");
        return false;
    }
    return true;
}


void initWifi() {
    String ssid = "";
    String password = "";

    ssid = globalConfiguration["ssid"].as<String>();
    password = globalConfiguration["password"].as<String>();

    Serial.print("SSID is ");
    Serial.println(ssid);
    Serial.print("Password is ");
    Serial.println(password);

    if (globalConfiguration["connectionMode"].as<String>().indexOf("AP") == -1) {
        Serial.print("Connecting to ");
        Serial.println(ssid);

        Serial.print("\nStation IP Address:");
        if (globalConfiguration["dhcp"]) {
            IPAddress address = IPAddress();
            IPAddress gateway = IPAddress();
            if (address.fromString(globalConfiguration["ip"].as<String>()) &&
                gateway.fromString(globalConfiguration["gateway"].as<String>())) {

                WiFi.config(address, gateway, IPAddress(255, 255, 255, 0));
            }
        }

        WiFi.begin(ssid.c_str(), password.c_str());
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.println();
        delay(500);
        WiFi.setHostname(globalConfiguration["hostname"]);
        Serial.println(WiFi.getHostname());
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("Setting up Hotspot");
        IPAddress address = IPAddress();
        IPAddress gateway = IPAddress();
        if (address.fromString(globalConfiguration["ip"].as<String>()) &&
            gateway.fromString(globalConfiguration["gateway"].as<String>())) {
            WiFi.softAPConfig(address, gateway, IPAddress(255, 255, 255, 0));
        }

        WiFi.softAP(ssid.c_str(), password.c_str());
        IPAddress myIP = WiFi.softAPIP();
        Serial.print("AP IP address: ");
        Serial.println(myIP);
    }
}


void sessionWatch() {
    unsigned long lapse = millis() - refreshSession;
    if (lapse / 1000 / 60 > REFRESH_INTERVAL && !sessionId.isEmpty()) {
        Serial.println("Resetting token. Closing current connections");
        sessionId = "";
    }
}


void genSessionID() {
    if (sessionId.isEmpty()) {
        for (int i = 0; i < 40; i++) {
            sessionId += char(random(65, 90));
        }
        sessionId.replace("\n", "");
        sessionId.replace("\t", "");
        sessionId.replace(" ", "");
        Serial.print("session id: ");
        Serial.println(sessionId);
    }
    refreshSession = millis();

    String response = "{ \"access_token\": \"";
    response += sessionId;
    response += "\",\n\"token_type\":\"bearer\"}";

    passCors();
    server.send(200, "application/json", response);
}


void authenticate() {
    DynamicJsonDocument request(512);

    DeserializationError error = deserializeJson(request, server.arg("plain"));
    if (error) {
        Serial.println(error.c_str());
    }

    String user = globalConfiguration["Settings"]["username"];
    String pswd = globalConfiguration["Settings"]["password"];


    if (user.equals(request["name"].as<String>()) &&
        pswd.equals(request["password"].as<String>())) {
        genSessionID();
        return;
    }

    passCors();
    server.send(401, "text/plain", "auth error");
}


void initWebServer() {
    server.on("/", []() {
        passCors();
        server.send(200, "text/plain", "espoleta says hello");
    });
    server.on("/authenticate", HTTP_OPTIONS, sendCors);
    server.on("/authenticate", HTTP_POST, authenticate);
    server.on("/update", HTTP_OPTIONS, sendCors);
    server.on("/update", HTTP_POST, onFinishUpload, handleUpload);
    server.on("/reset", resetConfig);
    server.on("/storage", HTTP_GET, []() {
        if (!authControl()) return;
        String space = "{";
        space += "\"used\": " + String((double) (SD.usedBytes())) + ",";
        space += "\"total\": " + String((double) (SD.cardSize())) + "}";
        passCors();
        server.send(200, "application/json", space.c_str());
    });
    server.on("/frequency", HTTP_OPTIONS, sendCors);
    server.on("/logs", []() {
        if (!authControl()) return;
        DateTime now = RTC.now();
        String path = "/logs/";
        path += now.year();
        path += "-";
        path += now.month();
        path += "-";
        path += now.day();

        File log = SD.open(path, FILE_READ);
        if (!log) {
            Serial.println("couldn't open log for reading");
            return;
        }
        passCors();
        server.streamFile(log, "text/plain");
        log.close();
    });
    server.on("/format", []() {
        if (!authControl()) return;
        File root;
        root = SD.open("/logs");
        delay(2000);

        while (true) {
            File entry = root.openNextFile();
            String localPath;

            Serial.println();
            if (!entry) {
                break;
            }
            if (!entry.isDirectory()) {
                SD.remove(String("/logs/") + String(entry.name()));
            }
            delay(1);
        }
        server.send(200, "Todos los logs han sido borrados");
    });
    server.onNotFound([]() {
        passCors();
        server.send(404);
    });

    server.begin();
    ws.begin();
    ws.onEvent(webSocketEvent);
}


void resetConfig() {
    if (!authControl()) return;
    File clean = SPIFFS.open("/clean_config.json", "r");
    if (!clean) {
        Serial.println("couldn't open 1");
        return;
    }
    if (!SPIFFS.remove("/config.json")) {
        Serial.println("couldn't clean confs");
    }
    File conf = SPIFFS.open("/config.json", "w");
    if (!conf) {
        Serial.println("Couldn't open 2");
        return;
    }
    size_t n;
    uint8_t data[64];
    Serial.println("start copying");
    while ((n = clean.read(data, sizeof(data))) > 0) {
        conf.write(data, n);
    }
    Serial.println("done copying");
    clean.close();
    conf.close();
    server.send(200, "text/plain", "configuration set to default");
    delay(500);
    ESP.restart();
}


void handleUpload() {
    HTTPUpload &upload = server.upload();

    if (upload.status == UPLOAD_FILE_START) {
        if (!authControl()) return;
        Serial.setDebugOutput(true);
        Serial.printf("Update: %s\n", upload.filename.c_str());
        if (upload.name == "filesystem") {
            if (!Update.begin(UPDATE_SIZE_UNKNOWN, U_SPIFFS)) { //start with max available size
                Update.printError(Serial);
            }
        } else {
            if (!Update.begin(UPDATE_SIZE_UNKNOWN, U_FLASH)) { //start with max available size
                Update.printError(Serial);
            }
        }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
        Serial.printf("%d bytes\n", upload.currentSize);
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
            Update.printError(Serial);
        }
    } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) { //true to set the size to the current progress
            Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
            Update.printError(Serial);
        }
        Serial.setDebugOutput(false);
    } else {
        Serial.printf("Update Failed Unexpectedly (likely broken connection): status=%d\n", upload.status);
        ESP.restart();
    }
}


void onFinishUpload() {
    Serial.println("update finished");
    server.sendHeader("Connection", "close");
    passCors();
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    writeLog();
    delay(500);
    ESP.restart();
}


String getStringVal(JsonObject obj) {
    return obj["value"].as<String>().length() ?
           obj["value"].as<String>() :
           obj["default"].as<String>();
}


bool getBooleanVal(JsonObject obj) {
    return obj["value"] != -1 ?
           obj["value"].as<bool>() :
           obj["default"].as<bool>();
}


double getNumberVal(JsonObject obj) {
    return obj["value"] ?
           obj["value"].as<double>() :
           obj["default"].as<double>();
}


void getWiFis() {
    passCors();
    server.send(200, "application/json", networks);
}


void configUser() {
    if (!authControl())

        return;
    if (server.hasArg("plain")) {
        DynamicJsonDocument request(512);

        DeserializationError error = deserializeJson(request, server.arg("plain"));
        if (error) {
            Serial.println(error.c_str());
        }

        globalConfiguration["user"]["name"]["value"] = request["name"].as<String>();
        globalConfiguration["user"]["password"]["value"] = request["password"].as<String>();

        saveConfig();
        passCors();
        server.send(200, "text/plain", "login credentials changed successfully");
    }
}


void sendCors() {
    passCors();
    server.send(204);
}


void webSocketEvent(byte num, WStype_t type, uint8_t *payload, size_t length) {
    String data = "";
    switch (type) {
        case WStype_ERROR:
            break;
        case WStype_DISCONNECTED:
            break;
        case WStype_CONNECTED:
            Serial.println("client connected");
            ws.sendTXT(num, "connected");
            ws.broadcastTXT("I'm broadcasting");
            data += humidityRead();
            data += lightRead();
            data += pressureRead();
            data += timeRead();
            logData(data);

            break;
        case WStype_TEXT:
            data = String((const char *) payload);
            Serial.println(data);
            if (data.indexOf("time=") != -1) {
                String time = data.substring(data.indexOf("=") + 1);
                Serial.println(time);
                long seconds = atof(time.c_str());
                DateTime dateTime = DateTime(seconds);
                Serial.print("Time adjusted ");
                Serial.print(seconds);
                Serial.print(" ");
                Serial.println(dateTime.timestamp());
                RTC.adjust(dateTime);
            }
            break;
        case WStype_BIN:
            break;
        case WStype_FRAGMENT_TEXT_START:
            break;
        case WStype_FRAGMENT_BIN_START:
            break;
        case WStype_FRAGMENT:
            break;
        case WStype_FRAGMENT_FIN:
            break;
        case WStype_PING:
            break;
        case WStype_PONG:
            break;
    }
}
