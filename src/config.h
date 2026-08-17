/*
 * ================================================================
 *  🕷️ SPIDEY TRACKER — Configuration & Pin Definitions
 *  ESP32 DevKit V1 Hardware Constants (Portrait Mode: 128x160)
 * ================================================================
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ================================================================
//  PIN DEFINITIONS (from HARDWARE_PINOUT.md)
// ================================================================

// ST7735 TFT Screen (SPI - VSPI)
#define TFT_CS    5     // GPIO 5  — Chip Select
#define TFT_RST   4     // GPIO 4  — Hardware Reset
#define TFT_DC    2     // GPIO 2  — Data/Command
#define TFT_MOSI  23    // GPIO 23 — VSPI MOSI
#define TFT_SCK   18    // GPIO 18 — VSPI Clock

// MPU6050 / MPU6500 IMU (I2C)
#define IMU_SDA   21    // GPIO 21 — I2C Data
#define IMU_SCL   22    // GPIO 22 — I2C Clock
#define IMU_INT   15    // GPIO 15 — Interrupt
#define IMU_ADDR  0x68  // I2C Address (AD0 = LOW)

// Status LEDs (Active High, PWM capable)
#define LED_BLUE  12    // GPIO 12 — X-Axis Tilt / Sys Blue
#define LED_GREEN 14    // GPIO 14 — Y-Axis Tilt / Sys Green
#define LED_RED   27    // GPIO 27 — Crash / Alert / Sys Red

// Navigation Switches (Active Low, INPUT_PULLUP)
#define BTN_UP     25   // GPIO 25
#define BTN_SELECT 26   // GPIO 26
#define BTN_DOWN   33   // GPIO 33

// ================================================================
//  DISPLAY SETTINGS (PORTRAIT MODE 128 x 160)
// ================================================================
#define SCREEN_W  128   // Portrait width
#define SCREEN_H  160   // Portrait height
#define TFT_ROTATION 0  // 0 = Standard Portrait

// ================================================================
//  🕷️ ADVANCED SPIDER-MAN HUD COLOR PALETTE (RGB565)
// ================================================================

// Primary colors
#define SPIDEY_RED       0xD800   // Deep Spider-Man red
#define SPIDEY_CRIMSON   0x9800   // Darker crimson red
#define SPIDEY_NEON_RED  0xF904   // Glowing HUD neon red
#define SPIDEY_BLUE      0x0119   // Deep tactical blue
#define SPIDEY_NEON_BLUE 0x05BF   // Glowing cyan-blue
#define SPIDEY_BLACK     0x0000   // Pure black
#define SPIDEY_WHITE     0xFFFF   // Pure white

// Background & HUD panels
#define SPIDEY_DARK      0x1082   // Dark HUD panel background
#define SPIDEY_DARKER    0x0841   // Deepest background
#define SPIDEY_DARKRED   0x3800   // Very dark red tint
#define SPIDEY_HUD_FRAME 0x2124   // Tactical HUD frame border
#define SPIDEY_HUD_GRID  0x1103   // Subtle grid lines

// Accents & High-Tech Highlights
#define SPIDEY_WEB       0xBDF7   // Crisp web lines
#define SPIDEY_GOLD      0xFE60   // Gold/Amber HUD highlight
#define SPIDEY_AMBER     0xFD20   // Tactical amber
#define SPIDEY_CYAN      0x07FF   // Tactical cyan / S.H.I.E.L.D.
#define SPIDEY_GREEN     0x07E0   // System green / online
#define SPIDEY_ORANGE    0xFC00   // Warning orange
#define SPIDEY_ALERT     0xF800   // Bright alert red

// ================================================================
//  TIMING CONSTANTS
// ================================================================
#define DEBOUNCE_MS       50      // Button debounce (ms)
#define LONG_PRESS_MS     1200    // Long-press threshold (ms)
#define MENU_ANIM_MS      60      // Menu animation speed (ms)
#define FRAME_DELAY_MS    30      // ~33 fps target

// ================================================================
//  IMU THRESHOLDS
// ================================================================
#define TILT_THRESHOLD    3.0f    // Tilt alert threshold (m/s²)
#define CRASH_THRESHOLD  15.0f    // Crash detection threshold (m/s²)
#define SENSE_DEFAULT     2.2f    // Default spider-sense sensitivity (m/s²)
#define SENSE_MIN         0.5f    // Minimum sensitivity
#define SENSE_MAX         8.0f    // Maximum sensitivity
#define SENSE_STEP        0.5f    // Sensitivity adjustment step

// ================================================================
//  LED PWM CONFIGURATION
// ================================================================
#define LED_PWM_FREQ     5000     // PWM frequency (Hz)
#define LED_PWM_RES      8        // PWM resolution (bits) → 0-255

// LED PWM channels (for ESP32 Arduino Core 2.x)
#define LED_BLUE_CH      0
#define LED_GREEN_CH     1
#define LED_RED_CH       2

// ================================================================
//  APPLICATION STATES
// ================================================================
enum AppState {
    STATE_BOOT,
    STATE_MENU,
    STATE_MOTION_TRACKER,
    STATE_CRASH_DETECTOR,
    STATE_SPIDER_SENSE,
    STATE_MARVEL_TRACKER,
    STATE_WEB_SHOOTER,
    STATE_STATS
};

// ================================================================
//  SHARED DATA STRUCTURES
// ================================================================

// IMU sensor readings
struct IMUData {
    float ax, ay, az;       // Acceleration (m/s²)
    float gx, gy, gz;       // Gyroscope (°/s)
    float temp;             // Temperature (°C)
    float magnitude;        // Total acceleration magnitude (m/s²)
    uint8_t chipID;         // WHO_AM_I register value
    bool valid;             // true if IMU communication is OK
};

// Button states with edge detection and long-press
struct ButtonState {
    bool upPressed;         // Rising edge: UP just pressed
    bool downPressed;       // Rising edge: DOWN just pressed
    bool selectPressed;     // Rising edge: SELECT just pressed
    bool selectHeld;        // SELECT is currently held down
    bool selectLongPress;   // Long-press detected (>1.2s)

    // Internal state
    bool _upLast;
    bool _downLast;
    bool _selectLast;
    unsigned long _selectStart;
    bool _longFired;
};

// ================================================================
//  MENU CONFIGURATION (PORTRAIT 128x160)
// ================================================================
#define MENU_ITEM_COUNT   6
#define MENU_ITEM_HEIGHT  20
#define MENU_VISIBLE      5
#define MENU_TOP_BAR      20
#define MENU_BOTTOM_BAR   16

// ================================================================
//  HELPER MACROS
// ================================================================
#define CLAMP(val, lo, hi) ((val) < (lo) ? (lo) : ((val) > (hi) ? (hi) : (val)))

inline float mapFloat(float x, float inMin, float inMax, float outMin, float outMax) {
    return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

#endif // CONFIG_H
