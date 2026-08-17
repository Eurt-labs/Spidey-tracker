/*
 * ================================================================
 *  🕷️ SPIDEY TRACKER — System Diagnostics (Portrait 128x160)
 *  S.H.I.E.L.D. / Stark Industries Tactical Diagnostic Monitor
 * ================================================================
 */

#include "stats_screen.h"
#include <math.h>

static bool needsFullRedraw = true;
static unsigned long lastDrawMs = 0;
static unsigned long lastSlowUpdate = 0;

static void drawTerminalBG(Adafruit_ST7735 &tft) {
    tft.fillScreen(SPIDEY_BLACK);

    // Title bar
    tft.fillRect(0, 0, SCREEN_W, 14, 0x0200);
    tft.drawFastHLine(0, 14, SCREEN_W, SPIDEY_CYAN);
    tft.setTextColor(SPIDEY_CYAN);
    tft.setTextSize(1);
    tft.setCursor(8, 3);
    tft.print("S.H.I.E.L.D. DIAG");

    // Accel Section (Y: 18 to 56)
    tft.setTextColor(0x03E0);
    tft.setCursor(4, 18);
    tft.print("-- ACCELEROMETER --");

    tft.setTextColor(SPIDEY_CYAN);
    tft.setCursor(4, 28);
    tft.print("aX:");
    tft.setCursor(4, 38);
    tft.print("aY:");
    tft.setCursor(4, 48);
    tft.print("aZ:");

    // Gyro Section (Y: 60 to 98)
    tft.setTextColor(0x03E0);
    tft.setCursor(4, 60);
    tft.print("-- GYROSCOPE --");

    tft.setTextColor(SPIDEY_CYAN);
    tft.setCursor(4, 70);
    tft.print("gX:");
    tft.setCursor(4, 80);
    tft.print("gY:");
    tft.setCursor(4, 90);
    tft.print("gZ:");

    // System Section (Y: 102 to 144)
    tft.drawFastHLine(0, 102, SCREEN_W, 0x03E0);
    tft.setTextColor(0x03E0);
    tft.setCursor(4, 106);
    tft.print("-- SYSTEM TELEMETRY --");

    tft.setTextColor(SPIDEY_CYAN);
    tft.setCursor(4, 118);
    tft.print("CHIP:");
    tft.setCursor(68, 118);
    tft.print("TEMP:");

    tft.setCursor(4, 130);
    tft.print("UP:");
    tft.setCursor(68, 130);
    tft.print("HEAP:");

    // Bottom hint
    tft.setTextColor(SPIDEY_WEB);
    tft.setCursor(18, SCREEN_H - 10);
    tft.print("HOLD SEL: BACK");
}

static void formatUptime(char *buf, size_t len) {
    unsigned long sec = millis() / 1000;
    unsigned long h = sec / 3600;
    unsigned long m = (sec % 3600) / 60;
    unsigned long s = sec % 60;
    snprintf(buf, len, "%02lu:%02lu:%02lu", h, m, s);
}

void statsScreenInit(Adafruit_ST7735 &tft) {
    needsFullRedraw = true;
}

bool statsScreenUpdate(Adafruit_ST7735 &tft, IMUData &imu, ButtonState &btn) {
    if (btn.selectLongPress) {
        return false;
    }

    if (millis() - lastDrawMs < 80) return true;
    lastDrawMs = millis();

    if (needsFullRedraw) {
        drawTerminalBG(tft);
        needsFullRedraw = false;
    }

    char buf[16];
    tft.setTextSize(1);

    // Accel Values
    tft.fillRect(24, 28, 54, 8, SPIDEY_BLACK);
    tft.fillRect(24, 38, 54, 8, SPIDEY_BLACK);
    tft.fillRect(24, 48, 54, 8, SPIDEY_BLACK);

    tft.setTextColor(SPIDEY_GREEN);

    dtostrf(imu.ax, 6, 2, buf);
    tft.setCursor(24, 28);
    tft.print(buf);

    dtostrf(imu.ay, 6, 2, buf);
    tft.setCursor(24, 38);
    tft.print(buf);

    dtostrf(imu.az, 6, 2, buf);
    tft.setCursor(24, 48);
    tft.print(buf);

    // Accel Magnitude on right
    tft.fillRect(80, 28, 44, 8, SPIDEY_BLACK);
    tft.setTextColor(SPIDEY_GOLD);
    tft.setCursor(80, 28);
    tft.print("|a|=");
    dtostrf(imu.magnitude, 4, 1, buf);
    tft.print(buf);

    // Gyro Values
    tft.fillRect(24, 70, 54, 8, SPIDEY_BLACK);
    tft.fillRect(24, 80, 54, 8, SPIDEY_BLACK);
    tft.fillRect(24, 90, 54, 8, SPIDEY_BLACK);

    tft.setTextColor(SPIDEY_CYAN);

    dtostrf(imu.gx, 6, 1, buf);
    tft.setCursor(24, 70);
    tft.print(buf);

    dtostrf(imu.gy, 6, 1, buf);
    tft.setCursor(24, 80);
    tft.print(buf);

    dtostrf(imu.gz, 6, 1, buf);
    tft.setCursor(24, 90);
    tft.print(buf);

    // Gyro Total on right
    tft.fillRect(80, 70, 44, 8, SPIDEY_BLACK);
    tft.setTextColor(SPIDEY_WEB);
    tft.setCursor(80, 70);
    float rot = sqrtf(imu.gx * imu.gx + imu.gy * imu.gy + imu.gz * imu.gz);
    dtostrf(rot, 4, 1, buf);
    tft.print(buf);
    tft.print("\xF8/s");

    // System Telemetry (slow update: 1 Hz)
    bool slowUpdate = (millis() - lastSlowUpdate > 1000);
    if (slowUpdate) {
        lastSlowUpdate = millis();

        // Chip
        tft.fillRect(34, 118, 30, 8, SPIDEY_BLACK);
        tft.setTextColor(SPIDEY_WHITE);
        tft.setCursor(34, 118);
        if (imu.chipID == 0x68) {
            tft.print("6050");
        } else if (imu.chipID == 0x70) {
            tft.print("6500");
        } else {
            tft.print("OK");
        }

        // Temp
        tft.fillRect(100, 118, 26, 8, SPIDEY_BLACK);
        tft.setTextColor(SPIDEY_ORANGE);
        tft.setCursor(100, 118);
        dtostrf(imu.temp, 3, 0, buf);
        tft.print(buf);
        tft.print("C");

        // Uptime
        tft.fillRect(22, 130, 44, 8, SPIDEY_BLACK);
        tft.setTextColor(SPIDEY_GREEN);
        formatUptime(buf, sizeof(buf));
        tft.setCursor(22, 130);
        tft.print(buf);

        // Heap
        tft.fillRect(98, 130, 28, 8, SPIDEY_BLACK);
        tft.setTextColor(SPIDEY_GREEN);
        tft.setCursor(98, 130);
        tft.print(ESP.getFreeHeap() / 1024);
        tft.print("K");

        // WiFi Status line
        tft.fillRect(4, 140, SCREEN_W - 8, 8, SPIDEY_BLACK);
        tft.setTextSize(1);
        if (sysStatus.timeSynced) {
            tft.setTextColor(SPIDEY_CYAN);
            tft.setCursor(4, 140);
            tft.print("WIFI: NTP LOCKED");
        } else if (sysStatus.wifiConnected) {
            tft.setTextColor(SPIDEY_GREEN);
            tft.setCursor(4, 140);
            tft.print("WIFI: CONNECTED");
        } else {
            tft.setTextColor(SPIDEY_AMBER);
            tft.setCursor(4, 140);
            tft.print("WIFI: SEARCHING");
        }
    }

    return true;
}
