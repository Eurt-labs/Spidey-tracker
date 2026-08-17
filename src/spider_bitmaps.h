/*
 * ================================================================
 *  🕷️ SPIDEY TRACKER — Bitmap Assets (PROGMEM)
 *  Tactical Spider-Man HUD icons and Vector Crest Bitmaps
 * ================================================================
 */

#ifndef SPIDER_BITMAPS_H
#define SPIDER_BITMAPS_H

#include <Arduino.h>

// ================================================================
//  TACTICAL SPIDER CREST — 32×32 pixels (128 bytes)
//  Angular Stark-Tech / Iron Spider chest insignia
// ================================================================
static const uint8_t PROGMEM spider_logo_32x32[] = {
    // Row 00-03: Top leg tips & mandibles
    0x80, 0x01, 0x80, 0x01,
    0x40, 0x03, 0xC0, 0x02,
    0x20, 0x07, 0xE0, 0x04,
    0x10, 0x0E, 0x70, 0x08,
    // Row 04-07: Upper leg arches & head
    0x18, 0x1C, 0x38, 0x18,
    0x0C, 0x38, 0x1C, 0x30,
    0x06, 0x7F, 0xFE, 0x60,
    0x03, 0xFF, 0xFF, 0xC0,
    // Row 08-11: Thorax & upper mid-legs
    0x03, 0xFF, 0xFF, 0xC0,
    0x07, 0xE7, 0xE7, 0xE0,
    0x0E, 0xC3, 0xC3, 0x70,
    0x1C, 0x81, 0x81, 0x38,
    // Row 12-15: Waist & lower mid-legs
    0x38, 0x01, 0x80, 0x1C,
    0x70, 0x03, 0xC0, 0x0E,
    0x60, 0x07, 0xE0, 0x06,
    0x40, 0x0F, 0xF0, 0x02,
    // Row 16-19: Abdomen flare & lower legs
    0x40, 0x1F, 0xF8, 0x02,
    0x60, 0x3F, 0xFC, 0x06,
    0x70, 0x7F, 0xFE, 0x0E,
    0x38, 0x7F, 0xFE, 0x1C,
    // Row 20-23: Lower abdomen & leg sweeps
    0x1C, 0x3F, 0xFC, 0x38,
    0x0E, 0x1F, 0xF8, 0x70,
    0x07, 0x0F, 0xF0, 0xE0,
    0x03, 0x87, 0xE1, 0xC0,
    // Row 24-27: Lower leg extensions
    0x01, 0xC3, 0xC3, 0x80,
    0x00, 0xE1, 0x87, 0x00,
    0x00, 0x70, 0x0E, 0x00,
    0x00, 0x38, 0x1C, 0x00,
    // Row 28-31: Tail stinger & leg tips
    0x00, 0x1C, 0x38, 0x00,
    0x00, 0x0E, 0x70, 0x00,
    0x00, 0x07, 0xE0, 0x00,
    0x00, 0x03, 0xC0, 0x00
};

// ================================================================
//  MINI SPIDER CREST — 12×12 pixels (for top status bar)
// ================================================================
static const uint8_t PROGMEM spider_mini_12x12[] = {
    0x80, 0x10,  // X..........X
    0x43, 0x20,  // .X....XX...X
    0x27, 0xE0,  // ..X..XXXXXX.
    0x1F, 0xF0,  // ...XXXXXXXX.
    0x3B, 0xD0,  // ..XXX.XXXX..
    0x71, 0x90,  // .XXX...XX...
    0x3F, 0xD0,  // ..XXXXXX.X..
    0x1F, 0xF0,  // ...XXXXXXXX.
    0x0F, 0xE0,  // ....XXXXXX..
    0x1B, 0xB0,  // ...XX.XX.XX.
    0x21, 0x80,  // ..X....XX...
    0x40, 0x20   // .X..........
};

// ================================================================
//  TACTICAL HUD MENU ICONS — 16×16 pixels each
// ================================================================

// Icon 0: Motion Tracker (Tactical HUD Gyro Radar)
static const uint8_t PROGMEM icon_motion[] = {
    0x03, 0xC0,  // ......XXXX......
    0x0C, 0x30,  // ....XX....XX....
    0x10, 0x08,  // ...X........X...
    0x21, 0x84,  // ..X....XX....X..
    0x41, 0x82,  // .X.....XX.....X.
    0x40, 0x02,  // .X............X.
    0x83, 0xC1,  // X.....XXXX.....X
    0x83, 0xC1,  // X.....XXXX.....X
    0x83, 0xC1,  // X.....XXXX.....X
    0x83, 0xC1,  // X.....XXXX.....X
    0x40, 0x02,  // .X............X.
    0x41, 0x82,  // .X.....XX.....X.
    0x21, 0x84,  // ..X....XX....X..
    0x10, 0x08,  // ...X........X...
    0x0C, 0x30,  // ....XX....XX....
    0x03, 0xC0   // ......XXXX......
};

// Icon 1: Crash Detector (High-G Impact Strain Wave)
static const uint8_t PROGMEM icon_crash[] = {
    0x01, 0x80,  // .......XX.......
    0x11, 0x88,  // ...X...XX...X...
    0x31, 0x8C,  // ..XX...XX...XX..
    0x2B, 0xD4,  // ..X.X.XXXX.X.X..
    0x0F, 0xF0,  // ....XXXXXXXX....
    0x1F, 0xF8,  // ...XXXXXXXXXX...
    0x7F, 0xFE,  // .XXXXXXXXXXXXXX.
    0xFF, 0xFF,  // XXXXXXXXXXXXXXXX
    0xFF, 0xFF,  // XXXXXXXXXXXXXXXX
    0x7F, 0xFE,  // .XXXXXXXXXXXXXX.
    0x1F, 0xF8,  // ...XXXXXXXXXX...
    0x0F, 0xF0,  // ....XXXXXXXX....
    0x2B, 0xD4,  // ..X.X.XXXX.X.X..
    0x31, 0x8C,  // ..XX...XX...XX..
    0x11, 0x88,  // ...X...XX...X...
    0x01, 0x80   // .......XX.......
};

// Icon 2: Spider-Sense (Threat Perception HUD Waves)
static const uint8_t PROGMEM icon_sense[] = {
    0x81, 0x81,  // X......XX......X
    0x43, 0xC2,  // .X....XXXX....X.
    0x22, 0x44,  // ..X...X..X...X..
    0x14, 0x28,  // ...X.X....X.X...
    0x08, 0x10,  // ....X......X....
    0x07, 0xE0,  // .....XXXXXX.....
    0x0D, 0xB0,  // ....XX.XX.XX....
    0x19, 0x98,  // ...XX..XX..XX...
    0x1B, 0xD8,  // ...XX.XXXX.XX...
    0x0F, 0xF0,  // ....XXXXXXXX....
    0x07, 0xE0,  // .....XXXXXX.....
    0x08, 0x10,  // ....X......X....
    0x14, 0x28,  // ...X.X....X.X...
    0x22, 0x44,  // ..X...X..X...X..
    0x43, 0xC2,  // .X....XXXX....X.
    0x81, 0x81   // X......XX......X
};

// Icon 3: Marvel Cinema (Cinematic Arc Reactor Star)
static const uint8_t PROGMEM icon_marvel[] = {
    0x03, 0xC0,  // ......XXXX......
    0x07, 0xE0,  // .....XXXXXX.....
    0x0F, 0xF0,  // ....XXXXXXXX....
    0x1F, 0xF8,  // ...XXXXXXXXXX...
    0x3F, 0xFC,  // ..XXXXXXXXXXXX..
    0x7F, 0xFE,  // .XXXXXXXXXXXXXX.
    0xFF, 0xFF,  // XXXXXXXXXXXXXXXX
    0xEF, 0xF7,  // XXXX.XXXXXX.XXXX
    0xCF, 0xF3,  // XX...XXXXXX...XX
    0x9F, 0xF9,  // X....XXXXXX....X
    0x3F, 0xFC,  // ..XXXXXXXXXXXX..
    0x7B, 0xDE,  // .XXXX.XXXX.XXXX.
    0xF1, 0x8F,  // XXXXX..XX..XXXXX
    0xE0, 0x07,  // XXX..........XXX
    0xC0, 0x03,  // XX............XX
    0x80, 0x01   // X..............X
};

// Icon 4: Web Shooter (Laser Precision Crosshair)
static const uint8_t PROGMEM icon_game[] = {
    0x01, 0x80,  // .......XX.......
    0x01, 0x80,  // .......XX.......
    0x0F, 0xF0,  // ....XXXXXXXX....
    0x10, 0x08,  // ...X........X...
    0x23, 0xC4,  // ..X...XXXX...X..
    0x24, 0x24,  // ..X..X....X..X..
    0x48, 0x12,  // .X..X......X..X.
    0xFF, 0xFF,  // XXXXXXXXXXXXXXXX
    0xFF, 0xFF,  // XXXXXXXXXXXXXXXX
    0x48, 0x12,  // .X..X......X..X.
    0x24, 0x24,  // ..X..X....X..X..
    0x23, 0xC4,  // ..X...XXXX...X..
    0x10, 0x08,  // ...X........X...
    0x0F, 0xF0,  // ....XXXXXXXX....
    0x01, 0x80,  // .......XX.......
    0x01, 0x80   // .......XX.......
};

// Icon 5: Stats Screen (S.H.I.E.L.D. System Monitor)
static const uint8_t PROGMEM icon_stats[] = {
    0xFE, 0x7F,  // XXXXXXX..XXXXXXX
    0x82, 0x41,  // X.....X..X.....X
    0x82, 0x41,  // X.....X..X.....X
    0x82, 0x71,  // X.....X..XXX...X
    0x82, 0x71,  // X.....X..XXX...X
    0x8E, 0x71,  // X...XXX..XXX...X
    0x8E, 0x71,  // X...XXX..XXX...X
    0x8E, 0xF1,  // X...XXX..XXXX..X
    0xBE, 0xF1,  // X.XXXXX..XXXX..X
    0xBE, 0xF1,  // X.XXXXX..XXXX..X
    0xBE, 0xFD,  // X.XXXXX..XXXXX.X
    0xFE, 0xFD,  // XXXXXXX..XXXXX.X
    0xFE, 0xFD,  // XXXXXXX..XXXXX.X
    0x80, 0x01,  // X..............X
    0xFF, 0xFF,  // XXXXXXXXXXXXXXXX
    0x00, 0x00   // ................
};

// ================================================================
//  MENU ITEM LABELS
// ================================================================
static const char* const menuLabels[] = {
    "MOTION RADAR",
    "CRASH SENSOR",
    "SPIDER-SENSE",
    "MARVEL CINEMA",
    "WEB TARGETING",
    "SYSTEM STATS"
};

static const uint8_t* const menuIcons[] = {
    icon_motion,
    icon_crash,
    icon_sense,
    icon_marvel,
    icon_game,
    icon_stats
};

#endif // SPIDER_BITMAPS_H
