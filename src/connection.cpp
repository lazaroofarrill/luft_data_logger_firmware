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

    ssid = getStringVal(doc["ssid"]);
    password = getStringVal(doc["password"]);

    Serial.print("SSDI is ");
    Serial.println(ssid);
    Serial.print("Password is ");
    Serial.println(password);

    int networkCount = WiFi.scanNetworks();

    if (networkCount > 0) {
        Serial.printf("found %d networks\n", networkCount);
    }
    int count = 0;
    for (int i = 0; i < networkCount; i++) {
        if (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) continue;
        if (count > 0) networks += ",";
        networks += "\"" + WiFi.SSID(i) + "\"";
        count++;
        Serial.println(WiFi.SSID(i));
    }
    networks += "]";

    if (!getBooleanVal(doc["AP"])) {
        Serial.print("Connecting to ");
        Serial.println(ssid);

        Serial.print("\nStation IP Address:");
        if (!getBooleanVal(doc["dhcp"])) {
            IPAddress address = IPAddress();
            IPAddress gateway = IPAddress();
            if (address.fromString(getStringVal(doc["ip"])) &&
                gateway.fromString(getStringVal(doc["gateway"]))) {

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
        WiFi.setHostname("estmet-01");
        Serial.println(WiFi.getHostname());
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("Setting up Hotspot");
        IPAddress address = IPAddress();
        IPAddress gateway = IPAddress();
        if (address.fromString(getStringVal(doc["ip"])) &&
            gateway.fromString(getStringVal(doc["gateway"]))) {
            WiFi.softAPConfig(address, gateway, IPAddress(255, 255, 255, 0));
        }

        WiFi.softAP(ssid.c_str(), password.c_str());
        IPAddress myIP = WiFi.softAPIP();
        Serial.print("AP IP address: ");
        Serial.println(myIP);
    }
}


void configNet() {
    if (!authControl())
        return;
    Serial.println(server.uri());
    Serial.println(server.method());
    for (int i = 0; i < server.headers(); i++) {
        Serial.printf("%s: %s\n", server.headerName(i).c_str(), server.header(i).c_str());
    }
    for (int i = 0; i < server.args(); i++) {
        Serial.printf("%s: %s\n", server.argName(i).c_str(), server.arg(i).c_str());
    }

    if (server.hasArg("plain")) {
        DynamicJsonDocument request(512);

        DeserializationError error = deserializeJson(request, server.arg("plain"));
        if (error) {
            Serial.println(error.c_str());
        }

        doc["ssid"]["value"] = request["ssid"].as<String>();
        doc["password"]["value"] = request["password"].as<String>();
        doc["AP"]["value"] = request["mode"].as<bool>();
        doc["dhcp"]["value"] = request["dhcp"].as<bool>();

        doc["ip"]["value"] = request["ip"].as<String>();
        doc["gateway"]["value"] = request["gateway"].as<String>();

        saveConfig();
    }
    passCors();
    Serial.println("network changed");
    server.send(200, "text/plain", "Network configuration changed");
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

    String user = getStringVal(doc["user"]["name"]);
    String pswd = getStringVal(doc["user"]["password"]);


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
    server.on("/frequency", HTTP_POST, []() {
        if (!authControl())
            return;
        if (server.hasArg("plain")) {
            DynamicJsonDocument request(512);

            DeserializationError error = deserializeJson(request, server.arg("plain"));
            if (error) {
                Serial.println(error.c_str());
            }

            doc["sensors"]["readFreq"]["value"] = request["frequency"].as<int>();

            saveConfig();
            passCors();
            server.send(200, "text/plain", "read frequency changed");
        } else {
            server.send(400, "mal formato de peticion");
        }
    });
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
            ESP.restart();
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

        doc["user"]["name"]["value"] = request["name"].as<String>();
        doc["user"]["password"]["value"] = request["password"].as<String>();

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
