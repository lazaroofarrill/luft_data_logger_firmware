//
// Created by lazt on 4/23/21.
//

#include "FileSystem/sdcard.h"

bool cardMounted = false;

#define SD_PIN 13

void initSDCard() {
    cardMounted = SD.begin(SD_PIN);
    if (!cardMounted) {
        Serial.println("Card mount failed");
        return;
    }

    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE) {
        Serial.println("No SD card attached");
        return;
    }
    Serial.print("SD Card Type: ");
    if (cardType == CARD_MMC) {
        Serial.println("MMC");
    } else if (cardType == CARD_SD) {
        Serial.println("SDSC");
    } else if (cardType == CARD_SDHC) {
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
}
