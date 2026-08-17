/*
 * ================================================================
 *  🕷️ SPIDEY TRACKER — Boot Animation Header
 * ================================================================
 */

#ifndef BOOT_ANIMATION_H
#define BOOT_ANIMATION_H

#include <Adafruit_ST7735.h>
#include "config.h"

// Run the full 4-phase cinematic boot animation
// Phase 1: Web strands radiate from center
// Phase 2: Spider emblem scans in + typewriter title
// Phase 3: Spider-Man mask eyes animate (squint → widen)
// Phase 4: Web dissolve transition to menu
void runBootAnimation(Adafruit_ST7735 &tft);

#endif // BOOT_ANIMATION_H
