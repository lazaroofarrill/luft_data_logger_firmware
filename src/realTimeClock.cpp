//
// Created by lazt on 4/23/21.
//

#include "Monitor/realTimeClock.h"

RTC_DS1307 RTC;


void timeStart() {
    RTC.begin();
    if (!RTC.isrunning()) {
        Serial.println("RTC is NOT running!");
    }
}

String timeRead() {
    String vars = "";
    DateTime now = RTC.now();
    vars += now.year();
    vars += "/";
    vars += now.month();
    vars += "/";
    vars += now.day();
    vars += " ";
    vars += now.hour();
    vars += ":";
    vars += now.minute();
    vars += ":";
    vars += now.second();
    Serial.println(vars);
    Serial.println();

    vars = ";date=\"" + vars + "\"";
    vars += ";timestamp=" + String(now.unixtime());
    return vars;
}
