/*
 * ================================================================
 *  🕷️ SPIDEY TRACKER — Movie Spider-Tracer Gadget Header
 * ================================================================
 */

#ifndef SPIDEY_TRACER_H
#define SPIDEY_TRACER_H

#include <Adafruit_ST7735.h>
#include "config.h"

void spideyTracerInit(Adafruit_ST7735 &tft);
bool spideyTracerUpdate(Adafruit_ST7735 &tft, IMUData &imu, ButtonState &btn);

#endif // SPIDEY_TRACER_H
