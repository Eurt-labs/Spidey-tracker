/*
 * ================================================================
 *  🕷️ SPIDEY TRACKER — Web Shooter Game Header
 * ================================================================
 */

#ifndef WEB_SHOOTER_GAME_H
#define WEB_SHOOTER_GAME_H

#include <Adafruit_ST7735.h>
#include "config.h"

void webShooterInit(Adafruit_ST7735 &tft);
bool webShooterUpdate(Adafruit_ST7735 &tft, IMUData &imu, ButtonState &btn);

#endif
