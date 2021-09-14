//
// Created by lazt on 4/23/21.
//

#ifndef BACKEND_HUMIDITYSENSOR_H
#define BACKEND_HUMIDITYSENSOR_H

#include "pinout.h"
#include "Logger/logger.h"

extern DHT dht;

void humidityStart();

String humidityRead();

#endif //BACKEND_HUMIDITYSENSOR_H
