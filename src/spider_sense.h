/*
 * ================================================================
 *  🕷️ SPIDEY TRACKER — Spider-Sense Header
 * ================================================================
 */

#ifndef SPIDER_SENSE_H
#define SPIDER_SENSE_H

#include <Adafruit_ST7735.h>
#include "config.h"

void spiderSenseInit(Adafruit_ST7735 &tft);
bool spiderSenseUpdate(Adafruit_ST7735 &tft, IMUData &imu, ButtonState &btn);

#endif
