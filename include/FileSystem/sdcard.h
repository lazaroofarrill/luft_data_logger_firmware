//
// Created by lazt on 4/23/21.
//

#ifndef BACKEND_SDCARD_H
#define BACKEND_SDCARD_H

#include <Arduino.h>
#include <FS.h>
#include <SPI.h>
#include <SD.h>

extern bool cardMounted;
void initSDCard();

#endif //BACKEND_SDCARD_H
