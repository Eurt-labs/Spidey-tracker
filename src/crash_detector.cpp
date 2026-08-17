/*
 * ================================================================
 *  🕷️ SPIDEY TRACKER — Crash & Impact Sensor (Portrait 128x160)
 *  Tactical Impact Telemetry, Web Strain Meter & Event Log
 * ================================================================
 */

#include "crash_detector.h"
#include <math.h>

static bool needsFullRedraw = true;
static unsigned long lastDrawMs = 0;

#define MAX_CRASH_LOG 4
static struct {
    float peakG;
    unsigned long timeMs;
} crashLog[MAX_CRASH_LOG];
static int crashCount = 0;

static bool crashActive = false;
static unsigned long crashFlashStart = 0;
static float peakMagnitude = 0.0f;
static float smoothMag = 9.81f;

static const int barX = 8;
static const int barY = 22;
static const int barW = 112;
static const int barH = 14;
static const int barMaxG = 25;

static void drawBackground(Adafruit_ST7735 &tft) {
    tft.fillScreen(SPIDEY_BLACK);

    // Title bar
    tft.fillRect(0, 0, SCREEN_W, 14, SPIDEY_DARKRED);
    tft.drawFastHLine(0, 14, SCREEN_W, SPIDEY_NEON_RED);
    tft.setTextColor(SPIDEY_WHITE);
    tft.setTextSize(1);
    tft.setCursor(20, 3);
    tft.print("IMPACT SENSOR");

    // Gauge frame & label
    tft.setTextColor(SPIDEY_WEB);
    tft.setCursor(8, barY - 7);
    tft.print("WEB STRAIN GAUGE");

    tft.drawRect(barX - 1, barY - 1, barW + 2, barH + 2, SPIDEY_HUD_FRAME);

    // 15G threshold marker line
    int threshX = barX + (int)((CRASH_THRESHOLD / barMaxG) * barW);
    tft.drawFastVLine(threshX, barY - 3, barH + 6, SPIDEY_ALERT);
    tft.setTextColor(SPIDEY_ALERT);
    tft.setCursor(threshX - 8, barY + barH + 3);
    tft.print("15G");

    // Readout panel
    int readY = barY + barH + 14;
    tft.drawRect(8, readY, SCREEN_W - 16, 26, SPIDEY_HUD_FRAME);
    tft.fillRect(9, readY + 1, SCREEN_W - 18, 24, SPIDEY_DARK);

    tft.setTextColor(SPIDEY_CYAN);
    tft.setCursor(14, readY + 4);
    tft.print("LIVE:");
    tft.setCursor(14, readY + 14);
    tft.setTextColor(SPIDEY_GOLD);
    tft.print("PEAK:");

    // Event Log header
    int logY = readY + 30;
    tft.setTextColor(SPIDEY_AMBER);
    tft.setCursor(8, logY);
    tft.print("-- IMPACT LOG --");
    tft.drawFastHLine(8, logY + 9, SCREEN_W - 16, SPIDEY_HUD_FRAME);

    // Bottom hint
    tft.setTextColor(SPIDEY_WEB);
    tft.setCursor(18, SCREEN_H - 10);
    tft.print("HOLD SEL: BACK");
}

static void drawCrashLog(Adafruit_ST7735 &tft) {
    int logY = barY + barH + 54;
    tft.fillRect(8, logY, SCREEN_W - 16, MAX_CRASH_LOG * 11, SPIDEY_BLACK);
    tft.setTextSize(1);

    if (crashCount == 0) {
        tft.setTextColor(SPIDEY_HUD_FRAME);
        tft.setCursor(20, logY + 10);
        tft.print("NO IMPACTS");
        return;
    }

    int displayed = 0;
    for (int i = crashCount - 1; i >= 0 && displayed < MAX_CRASH_LOG; i--, displayed++) {
        int idx = i % MAX_CRASH_LOG;
        int y = logY + displayed * 11;

        unsigned long ago = (millis() - crashLog[idx].timeMs) / 1000;
        char buf[20];

        tft.setTextColor(SPIDEY_ALERT);
        tft.setCursor(8, y);
        tft.print("#");
        tft.print(i + 1);

        tft.setTextColor(SPIDEY_WHITE);
        tft.setCursor(26, y);
        dtostrf(crashLog[idx].peakG, 4, 1, buf);
        tft.print(buf);
        tft.print("G");

        tft.setTextColor(SPIDEY_WEB);
        tft.setCursor(84, y);
        if (ago < 60) {
            snprintf(buf, sizeof(buf), "%lus", ago);
        } else {
            snprintf(buf, sizeof(buf), "%lum", ago / 60);
        }
        tft.print(buf);
    }
}

void crashDetectorInit(Adafruit_ST7735 &tft) {
    needsFullRedraw = true;
    crashActive = false;
    peakMagnitude = 0.0f;
    smoothMag = 9.81f;
}

bool crashDetectorUpdate(Adafruit_ST7735 &tft, IMUData &imu, ButtonState &btn) {
    if (btn.selectLongPress) {
        ledcWrite(LED_RED_CH, 0);
        return false;
    }

    if (millis() - lastDrawMs < 40) return true;
    lastDrawMs = millis();

    if (needsFullRedraw) {
        drawBackground(tft);
        drawCrashLog(tft);
        needsFullRedraw = false;
    }

    smoothMag = smoothMag * 0.7f + imu.magnitude * 0.3f;

    if (imu.magnitude > peakMagnitude) {
        peakMagnitude = imu.magnitude;
    }

    if (imu.magnitude > CRASH_THRESHOLD && !crashActive) {
        crashActive = true;
        crashFlashStart = millis();

        int logIdx = crashCount % MAX_CRASH_LOG;
        crashLog[logIdx].peakG = imu.magnitude;
        crashLog[logIdx].timeMs = millis();
        crashCount++;

        drawCrashLog(tft);
    }

    if (crashActive) {
        unsigned long elapsed = millis() - crashFlashStart;
        if (elapsed < 1400) {
            bool flashOn = (elapsed / 80) % 2 == 0;
            uint16_t borderColor = flashOn ? SPIDEY_ALERT : SPIDEY_BLACK;
            tft.drawRect(0, 15, SCREEN_W, SCREEN_H - 26, borderColor);
            tft.drawRect(1, 16, SCREEN_W - 2, SCREEN_H - 28, borderColor);

            ledcWrite(LED_RED_CH, flashOn ? 255 : 0);

            if (flashOn) {
                tft.fillRect(16, 16, SCREEN_W - 32, 10, SPIDEY_BLACK);
                tft.setTextColor(SPIDEY_ALERT);
                tft.setTextSize(1);
                tft.setCursor(24, 17);
                tft.print("! IMPACT !");
            } else {
                tft.fillRect(16, 16, SCREEN_W - 32, 10, SPIDEY_BLACK);
            }
        } else {
            crashActive = false;
            ledcWrite(LED_RED_CH, 0);
            tft.drawRect(0, 15, SCREEN_W, SCREEN_H - 26, SPIDEY_BLACK);
            tft.drawRect(1, 16, SCREEN_W - 2, SCREEN_H - 28, SPIDEY_BLACK);
            tft.fillRect(16, 16, SCREEN_W - 32, 10, SPIDEY_BLACK);
        }
    }

    // Bar Meter fill
    int fillW = (int)(CLAMP(smoothMag, 0, barMaxG) / barMaxG * barW);
    tft.fillRect(barX, barY, barW, barH, SPIDEY_DARKER);

    for (int x = 0; x < fillW; x++) {
        float t = (float)x / barW;
        uint16_t color;
        if (t < 0.45f) {
            color = SPIDEY_NEON_BLUE;
        } else if (t < 0.65f) {
            color = SPIDEY_GOLD;
        } else {
            color = SPIDEY_ALERT;
        }
        tft.drawFastVLine(barX + x, barY, barH, color);
    }

    // Threshold indicator line
    int threshX = barX + (int)((CRASH_THRESHOLD / barMaxG) * barW);
    tft.drawFastVLine(threshX, barY, barH, SPIDEY_WHITE);

    // Numeric readouts
    char buf[12];
    int readY = barY + barH + 14;

    tft.fillRect(50, readY + 4, 64, 8, SPIDEY_DARK);
    tft.setTextColor(SPIDEY_WHITE);
    tft.setTextSize(1);
    dtostrf(smoothMag, 4, 1, buf);
    tft.setCursor(50, readY + 4);
    tft.print(buf);
    tft.print(" m/s\xFD");

    tft.fillRect(50, readY + 14, 64, 8, SPIDEY_DARK);
    tft.setTextColor(peakMagnitude > CRASH_THRESHOLD ? SPIDEY_ALERT : SPIDEY_GOLD);
    dtostrf(peakMagnitude, 4, 1, buf);
    tft.setCursor(50, readY + 14);
    tft.print(buf);
    tft.print(" m/s\xFD");

    if (btn.selectPressed) {
        peakMagnitude = 0.0f;
    }

    if (!crashActive) {
        if (smoothMag > CRASH_THRESHOLD * 0.7f) {
            ledcWrite(LED_RED_CH, 60);
        } else {
            ledcWrite(LED_RED_CH, 0);
        }
    }

    return true;
}
