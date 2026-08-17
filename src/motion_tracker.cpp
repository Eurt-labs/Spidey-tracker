/*
 * ================================================================
 *  🕷️ SPIDEY TRACKER — Motion Radar (Portrait 128x160)
 *  Tactical Spider-Web Radar with 6-DOF Attitude & Tilt Vector
 *  (Zero-Artifact Bounding-Box Direct Frame Buffer Rendering)
 * ================================================================
 */

#include "motion_tracker.h"
#include <math.h>

static bool needsFullRedraw = true;
static unsigned long lastDrawMs = 0;

static const int radarCX = 64;
static const int radarCY = 58;
static const int radarR  = 40;

// Draw static framing and telemetry panel
static void drawStaticUI(Adafruit_ST7735 &tft) {
    tft.fillScreen(SPIDEY_BLACK);

    // Title bar
    tft.fillRect(0, 0, SCREEN_W, 14, SPIDEY_DARKRED);
    tft.drawFastHLine(0, 14, SCREEN_W, SPIDEY_NEON_RED);
    tft.setTextColor(SPIDEY_WHITE);
    tft.setTextSize(1);
    tft.setCursor(16, 3);
    tft.print("MOTION RADAR");

    // Axis markings
    tft.setTextColor(SPIDEY_WEB);
    tft.setTextSize(1);
    tft.setCursor(radarCX + radarR - 6, radarCY - 3);
    tft.print("X");
    tft.setCursor(radarCX - 3, radarCY - radarR + 2);
    tft.print("Y");

    // Telemetry panel framing (Bottom Y: 104 - 146)
    int panelY = 104;
    tft.drawRect(2, panelY, SCREEN_W - 4, 42, SPIDEY_HUD_FRAME);
    tft.fillRect(3, panelY + 1, SCREEN_W - 6, 40, SPIDEY_DARK);

    tft.setTextColor(SPIDEY_CYAN);
    tft.setCursor(6, panelY + 4);
    tft.print("X:");
    tft.setCursor(68, panelY + 4);
    tft.print("Y:");

    tft.setCursor(6, panelY + 16);
    tft.print("Z:");
    tft.setCursor(68, panelY + 16);
    tft.setTextColor(SPIDEY_GOLD);
    tft.print("G:");

    // Magnitude bar frame
    tft.drawRect(6, panelY + 28, SCREEN_W - 12, 8, SPIDEY_HUD_FRAME);

    // Bottom exit hint
    tft.setTextColor(SPIDEY_WEB);
    tft.setCursor(18, SCREEN_H - 10);
    tft.print("HOLD SEL: BACK");
}

void motionTrackerInit(Adafruit_ST7735 &tft) {
    needsFullRedraw = true;
}

bool motionTrackerUpdate(Adafruit_ST7735 &tft, IMUData &imu, ButtonState &btn) {
    if (btn.selectLongPress) {
        ledcWrite(LED_BLUE_CH, 0);
        ledcWrite(LED_GREEN_CH, 0);
        return false;
    }

    if (millis() - lastDrawMs < 35) return true;
    lastDrawMs = millis();

    if (needsFullRedraw) {
        drawStaticUI(tft);
        needsFullRedraw = false;
    }

    // Map tilt to coordinates
    float maxTilt = 9.81f;
    int dotX = radarCX + (int)(CLAMP(imu.ax, -maxTilt, maxTilt) / maxTilt * (radarR - 4));
    int dotY = radarCY - (int)(CLAMP(imu.ay, -maxTilt, maxTilt) / maxTilt * (radarR - 4));

    float dist = sqrtf((dotX - radarCX) * (dotX - radarCX) + (dotY - radarCY) * (dotY - radarCY));
    if (dist > radarR - 3) {
        float scale = (radarR - 3) / dist;
        dotX = radarCX + (int)((dotX - radarCX) * scale);
        dotY = radarCY + (int)((dotY - radarCY) * scale);
    }

    // ============================================================
    //  CLEAN RADAR RENDERING (Zero Noise / Zero Artifacts)
    // ============================================================
    tft.fillRect(radarCX - radarR - 3, radarCY - radarR - 3, (radarR + 3) * 2 + 1, (radarR + 3) * 2 + 1, SPIDEY_BLACK);

    // Concentric spider-web radar rings
    for (int r = 11; r <= radarR; r += 11) {
        for (int a = 0; a < 360; a += 15) {
            float r1 = a * PI / 180.0f;
            float r2 = (a + 15) * PI / 180.0f;
            int x1 = radarCX + (int)(cos(r1) * r);
            int y1 = radarCY + (int)(sin(r1) * r);
            int x2 = radarCX + (int)(cos(r2) * r);
            int y2 = radarCY + (int)(sin(r2) * r);
            tft.drawLine(x1, y1, x2, y2, SPIDEY_DARK);
        }
    }

    // 8 radial web azimuth guides
    for (int i = 0; i < 8; i++) {
        float angle = i * (PI / 4.0f);
        int ex = radarCX + (int)(cos(angle) * radarR);
        int ey = radarCY + (int)(sin(angle) * radarR);
        tft.drawLine(radarCX, radarCY, ex, ey, SPIDEY_HUD_GRID);
    }

    // Outer radar rings
    tft.drawCircle(radarCX, radarCY, radarR, SPIDEY_HUD_FRAME);
    tft.drawCircle(radarCX, radarCY, radarR + 2, SPIDEY_DARKRED);

    // Draw vector line from origin to dot
    tft.drawLine(radarCX, radarCY, dotX, dotY, SPIDEY_DARKRED);

    // Draw tactical target reticle at tilt position
    tft.fillCircle(dotX, dotY, 4, SPIDEY_NEON_RED);
    tft.drawCircle(dotX, dotY, 4, SPIDEY_GOLD);

    // Center crosshair origin
    tft.fillCircle(radarCX, radarCY, 2, SPIDEY_GOLD);

    // ============================================================
    //  TELEMETRY PANEL UPDATE
    // ============================================================
    int panelY = 104;
    char buf[10];
    tft.setTextSize(1);

    // Clear value areas
    tft.fillRect(20, panelY + 4, 44, 8, SPIDEY_DARK);
    tft.fillRect(82, panelY + 4, 40, 8, SPIDEY_DARK);
    tft.fillRect(20, panelY + 16, 44, 8, SPIDEY_DARK);
    tft.fillRect(82, panelY + 16, 40, 8, SPIDEY_DARK);

    // X
    tft.setTextColor(SPIDEY_WHITE);
    dtostrf(imu.ax, 4, 1, buf);
    tft.setCursor(20, panelY + 4);
    tft.print(buf);

    // Y
    dtostrf(imu.ay, 4, 1, buf);
    tft.setCursor(82, panelY + 4);
    tft.print(buf);

    // Z
    dtostrf(imu.az, 4, 1, buf);
    tft.setCursor(20, panelY + 16);
    tft.print(buf);

    // Total G
    tft.setTextColor(SPIDEY_GOLD);
    dtostrf(imu.magnitude / 9.81f, 4, 1, buf);
    tft.setCursor(82, panelY + 16);
    tft.print(buf);

    // Magnitude bar fill
    int maxBarW = SCREEN_W - 16;
    int fillW = (int)(CLAMP(imu.magnitude / 20.0f, 0.0f, 1.0f) * maxBarW);
    tft.fillRect(8, panelY + 30, maxBarW, 4, SPIDEY_DARKER);
    uint16_t barCol = (imu.magnitude > 12.0f) ? SPIDEY_NEON_RED : SPIDEY_NEON_BLUE;
    tft.fillRect(8, panelY + 30, fillW, 4, barCol);

    // LED tilt feedback
    if (fabsf(imu.ax) > TILT_THRESHOLD) {
        ledcWrite(LED_BLUE_CH, (uint8_t)CLAMP(fabsf(imu.ax) * 22, 0, 255));
    } else {
        ledcWrite(LED_BLUE_CH, 0);
    }

    if (fabsf(imu.ay) > TILT_THRESHOLD) {
        ledcWrite(LED_GREEN_CH, (uint8_t)CLAMP(fabsf(imu.ay) * 12, 0, 45));
    } else {
        ledcWrite(LED_GREEN_CH, 0);
    }

    return true;
}
