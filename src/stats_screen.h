/*
 * ================================================================
 *  🕷️ SPIDEY TRACKER — Stats Screen Header
 * ================================================================
 */

#ifndef STATS_SCREEN_H
#define STATS_SCREEN_H

#include <Adafruit_ST7735.h>
#include "config.h"

void statsScreenInit(Adafruit_ST7735 &tft);
bool statsScreenUpdate(Adafruit_ST7735 &tft, IMUData &imu, ButtonState &btn);

#endif
