/*
 * ================================================================
 *  🕷️ SPIDEY TRACKER — Motion Tracker Header
 * ================================================================
 */

#ifndef MOTION_TRACKER_H
#define MOTION_TRACKER_H

#include <Adafruit_ST7735.h>
#include "config.h"

void motionTrackerInit(Adafruit_ST7735 &tft);
bool motionTrackerUpdate(Adafruit_ST7735 &tft, IMUData &imu, ButtonState &btn);

#endif
