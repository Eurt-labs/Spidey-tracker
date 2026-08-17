/*
 * ================================================================
 *  🕷️ SPIDEY TRACKER — Marvel Tracker Header
 * ================================================================
 */

#ifndef MARVEL_TRACKER_H
#define MARVEL_TRACKER_H

#include <Adafruit_ST7735.h>
#include "config.h"

void marvelTrackerInit(Adafruit_ST7735 &tft);
bool marvelTrackerUpdate(Adafruit_ST7735 &tft, IMUData &imu, ButtonState &btn);

#endif
