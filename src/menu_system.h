/*
 * ================================================================
 *  🕷️ SPIDEY TRACKER — Menu System Header
 * ================================================================
 */

#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include <Adafruit_ST7735.h>
#include "config.h"

// Initialize menu state (call once before first draw)
void menuInit();

// Update menu logic (button handling, selection animation)
// Returns: the AppState to switch to if SELECT is pressed,
//          or STATE_MENU if still navigating
AppState menuUpdate(Adafruit_ST7735 &tft, ButtonState &btn);

// Draw the full menu screen
void menuDraw(Adafruit_ST7735 &tft);

// Get currently selected menu index (0-based)
int menuGetSelected();

#endif // MENU_SYSTEM_H
