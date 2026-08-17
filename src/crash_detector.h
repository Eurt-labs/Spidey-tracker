/*
 * ================================================================
 *  🕷️ SPIDEY TRACKER — Crash Detector Header
 * ================================================================
 */

#ifndef CRASH_DETECTOR_H
#define CRASH_DETECTOR_H

#include <Adafruit_ST7735.h>
#include "config.h"

void crashDetectorInit(Adafruit_ST7735 &tft);
bool crashDetectorUpdate(Adafruit_ST7735 &tft, IMUData &imu, ButtonState &btn);

#endif
