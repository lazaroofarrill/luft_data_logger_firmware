#include <sys/cdefs.h>
//
// Created by lazt on 4/23/21.
//

#ifndef BACKEND_MONITORPROCESS_H
#define BACKEND_MONITORPROCESS_H

#include "humiditySensor.h"
#include "lightSensor.h"
#include "pressureSensor.h"
#include "realTimeClock.h"
#include "Logger/logger.h"

extern TaskHandle_t monitorTask;

void monitorSetup();

[[noreturn]] void monitorLoop(void *param);

#endif //BACKEND_MONITORPROCESS_H
