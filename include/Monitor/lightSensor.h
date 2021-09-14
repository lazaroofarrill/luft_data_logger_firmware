//
// Created by lazt on 4/23/21.
//

#ifndef BACKEND_LIGHTSENSOR_H
#define BACKEND_LIGHTSENSOR_H

#include "Wire.h"
#include "BH1750.h"
#include "pinout.h"

void lightStart();
String lightRead();


#endif //BACKEND_LIGHTSENSOR_H
