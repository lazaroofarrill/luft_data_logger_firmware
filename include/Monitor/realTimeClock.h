//
// Created by lazt on 4/23/21.
//

#ifndef BACKEND_REALTIMECLOCK_H
#define BACKEND_REALTIMECLOCK_H

#include "Wire.h"
#include "RTClib.h"

extern RTC_DS1307 RTC;

void timeStart();

String timeRead();


#endif //BACKEND_REALTIMECLOCK_H
