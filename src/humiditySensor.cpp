//
// Created by lazt on 4/23/21.
//

#include "Monitor/humiditySensor.h"

DHT dht(DHTPIN, DHTTYPE);

void humidityStart() {
    dht.begin();
}

String humidityRead() {
    double h = dht.readHumidity();
    double t = dht.readTemperature();
    double f = dht.readTemperature(true);
    String vars = "";

    if (isnan(h) || isnan(t) || isnan(f)) {
        Serial.println("Failed to read from DHT sensor!");
        return vars;
    }

    vars += ",humidity=" + String(h);
    vars += ",temperature_c=" + String(t);
    vars += ",temperature_f=" + String(f);

    double hif = dht.computeHeatIndex(f, h);
    double hic = dht.computeHeatIndex(t, h, false);

    vars += ",heatIndex_c=" + String(hic);
    vars += ",heatIndex_f=" + String(hif);


    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));
    Serial.print(f);
    Serial.print(F("°F  Heat index: "));
    Serial.print(hic);
    Serial.print(F("°C "));
    Serial.print(hif);
    Serial.println(F("°F"));

    return vars;
}
