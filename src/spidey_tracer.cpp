/*
 * ================================================================
 *  🕷️ SPIDEY TRACKER — Movie Spider-Tracer Gadget Simulator
 *  Tactical Sonar Radar, Signal Strength, Target Lock & Sonar Ping
 * ================================================================
 */

#include "spidey_tracer.h"
#include "spider_bitmaps.h"
#include <math.h>

#define TRACER_TARGET_COUNT 5

struct TracerTarget {
    const char *name;
    const char *code;
    float initialDist;
    float currentDist;
    float bearingAngle; // in radians
    const char *freq;
    uint16_t themeColor;
};

static TracerTarget targets[TRACER_TARGET_COUNT] = {
    { "SPIDER-MAN",  "BEACON #01", 142.0f, 142.0f, 0.65f, "433.92 MHz", SPIDEY_NEON_RED },
    { "VULTURE",     "FLIGHT PACK",380.0f, 380.0f, 2.10f, "868.10 MHz", SPIDEY_GREEN },
    { "GREEN GOBLIN","GLIDER TRAC",510.0f, 510.0f, 4.20f, "915.00 MHz", SPIDEY_AMBER },
    { "DOC OCK",     "NEURAL ARM", 215.0f, 215.0f, 5.40f, "433.40 MHz", SPIDEY_GOLD },
    { "ELECTRO",     "BIO-VOLT",   680.0f, 680.0f, 3.14f, "1.200 GHz",  SPIDEY_CYAN }
};

static int currentTargetIdx = 0;
static bool needsFullRedraw = true;
static unsigned long lastDrawMs = 0;

static const int radarCX = 64;
static const int radarCY = 58;
static const int radarR  = 40;

static float sweepAngle = 0.0f;
static float pingRadius = -1.0f;
static int prevBlipX = -1, prevBlipY = -1;
static int prevSweepX = -1, prevSweepY = -1;

static void drawRadarBackground(Adafruit_ST7735 &tft) {
    tft.fillScreen(SPIDEY_BLACK);

    // Top Header
    tft.fillRect(0, 0, SCREEN_W, 14, SPIDEY_DARKRED);
    tft.drawFastHLine(0, 14, SCREEN_W, SPIDEY_NEON_RED);

    tft.setTextColor(SPIDEY_WHITE);
    tft.setTextSize(1);
    tft.setCursor(4, 3);
    tft.print("SPIDER-TRACER");

    // Target code tag on right
    char codeBuf[10];
    snprintf(codeBuf, sizeof(codeBuf), "#%02d", currentTargetIdx + 1);
    tft.setTextColor(SPIDEY_GOLD);
    tft.setCursor(SCREEN_W - 24, 3);
    tft.print(codeBuf);

    // Sonar radar rings
    for (int r = 13; r <= radarR; r += 13) {
        tft.drawCircle(radarCX, radarCY, r, SPIDEY_DARK);
    }

    // Cardinal azimuth lines
    tft.drawFastHLine(radarCX - radarR, radarCY, radarR * 2 + 1, SPIDEY_HUD_GRID);
    tft.drawFastVLine(radarCX, radarCY - radarR, radarR * 2 + 1, SPIDEY_HUD_GRID);

    // Outer radar bevel ring
    tft.drawCircle(radarCX, radarCY, radarR, SPIDEY_HUD_FRAME);
    tft.drawCircle(radarCX, radarCY, radarR + 2, SPIDEY_DARKRED);

    // Center locator dot
    tft.fillCircle(radarCX, radarCY, 2, SPIDEY_GOLD);

    // Target Profile Card (Y: 102 to 144)
    int panelY = 102;
    tft.drawRect(2, panelY, SCREEN_W - 4, 42, SPIDEY_HUD_FRAME);
    tft.fillRect(3, panelY + 1, SCREEN_W - 6, 40, SPIDEY_DARK);

    tft.setTextColor(targets[currentTargetIdx].themeColor);
    tft.setCursor(6, panelY + 4);
    tft.print(targets[currentTargetIdx].name);

    tft.setTextColor(SPIDEY_WEB);
    tft.setCursor(76, panelY + 4);
    tft.print(targets[currentTargetIdx].code);

    tft.setTextColor(SPIDEY_CYAN);
    tft.setCursor(6, panelY + 16);
    tft.print("DIST:");

    tft.setCursor(68, panelY + 16);
    tft.print("SIG:");

    // Signal Strength Bar Frame
    tft.drawRect(6, panelY + 28, SCREEN_W - 12, 8, SPIDEY_HUD_FRAME);

    // Bottom Guide Bar
    tft.fillRect(0, SCREEN_H - 14, SCREEN_W, 14, SPIDEY_DARKER);
    tft.drawFastHLine(0, SCREEN_H - 14, SCREEN_W, SPIDEY_HUD_FRAME);

    tft.setTextColor(SPIDEY_WEB);
    tft.setCursor(4, SCREEN_H - 10);
    tft.print("\x18\x19:TGT SEL:PING");
}

void spideyTracerInit(Adafruit_ST7735 &tft) {
    needsFullRedraw = true;
    sweepAngle = 0.0f;
    pingRadius = -1.0f;
    prevBlipX = -1;
    prevBlipY = -1;
    prevSweepX = -1;
    prevSweepY = -1;
}

bool spideyTracerUpdate(Adafruit_ST7735 &tft, IMUData &imu, ButtonState &btn) {
    if (btn.selectLongPress) {
        ledcWrite(LED_BLUE_CH, 0);
        ledcWrite(LED_RED_CH, 0);
        ledcWrite(LED_GREEN_CH, 0);
        return false;
    }

    // Switch targets with UP / DOWN
    if (btn.upPressed) {
        currentTargetIdx--;
        if (currentTargetIdx < 0) currentTargetIdx = TRACER_TARGET_COUNT - 1;
        needsFullRedraw = true;
        ledcWrite(LED_BLUE_CH, 50);
    }
    if (btn.downPressed) {
        currentTargetIdx++;
        if (currentTargetIdx >= TRACER_TARGET_COUNT) currentTargetIdx = 0;
        needsFullRedraw = true;
        ledcWrite(LED_BLUE_CH, 50);
    }

    if (!btn.upPressed && !btn.downPressed && pingRadius < 0) {
        ledcWrite(LED_BLUE_CH, 0);
    }

    // Sonar Ping on SELECT press
    if (btn.selectPressed) {
        pingRadius = 4.0f;
        // Simulate distance decreasing as you track
        targets[currentTargetIdx].currentDist -= random(3, 10);
        if (targets[currentTargetIdx].currentDist < 12.0f) {
            targets[currentTargetIdx].currentDist = targets[currentTargetIdx].initialDist;
        }
        ledcWrite(LED_RED_CH, 150);
        ledcWrite(LED_BLUE_CH, 150);
    }

    if (millis() - lastDrawMs < 35) return true;
    lastDrawMs = millis();

    if (needsFullRedraw) {
        drawRadarBackground(tft);
        needsFullRedraw = false;
    }

    TracerTarget &tgt = targets[currentTargetIdx];

    // Calculate relative bearing angle offset by device tilt / gyro X/Y
    float deviceBearing = atan2f(-imu.ax, imu.ay);
    float relativeAngle = tgt.bearingAngle - deviceBearing - (PI / 2.0f);

    // Compute target blip coordinates on radar
    float distScale = CLAMP(tgt.currentDist / 600.0f, 0.25f, 0.90f);
    int blipR = (int)(distScale * (radarR - 4));
    int blipX = radarCX + (int)(cos(relativeAngle) * blipR);
    int blipY = radarCY + (int)(sin(relativeAngle) * blipR);

    // Compute signal strength (peaks when pointed directly toward bearing: relativeAngle ≈ -PI/2 or 0 depending on reference)
    float angleDelta = fabsf(atan2f(sin(relativeAngle + PI / 2.0f), cos(relativeAngle + PI / 2.0f)));
    float alignmentFactor = 1.0f - (angleDelta / PI); // 0.0 to 1.0
    int signalPercent = (int)(alignmentFactor * 85.0f + 15.0f);

    // Erase previous radar sweep line
    if (prevSweepX >= 0) {
        tft.drawLine(radarCX, radarCY, prevSweepX, prevSweepY, SPIDEY_BLACK);
        // Redraw rings that were clipped
        for (int r = 13; r <= radarR; r += 13) {
            tft.drawCircle(radarCX, radarCY, r, SPIDEY_DARK);
        }
    }

    // Erase previous blip
    if (prevBlipX >= 0) {
        tft.fillCircle(prevBlipX, prevBlipY, 4, SPIDEY_BLACK);
        tft.drawCircle(prevBlipX, prevBlipY, 6, SPIDEY_BLACK);
    }

    // Erase previous sonar ping circle
    if (pingRadius >= 0) {
        tft.drawCircle(radarCX, radarCY, (int)pingRadius, SPIDEY_BLACK);
        pingRadius += 5.0f;
        if (pingRadius > radarR) {
            pingRadius = -1.0f;
            ledcWrite(LED_RED_CH, 0);
            ledcWrite(LED_BLUE_CH, 0);
        } else {
            tft.drawCircle(radarCX, radarCY, (int)pingRadius, SPIDEY_NEON_BLUE);
        }
    }

    // Draw rotating radar sweep line
    sweepAngle += 0.12f;
    if (sweepAngle > 2.0f * PI) sweepAngle -= 2.0f * PI;
    int sweepX = radarCX + (int)(cos(sweepAngle) * (radarR - 2));
    int sweepY = radarCY + (int)(sin(sweepAngle) * (radarR - 2));
    tft.drawLine(radarCX, radarCY, sweepX, sweepY, SPIDEY_DARKRED);
    prevSweepX = sweepX;
    prevSweepY = sweepY;

    // Draw Target Blip (Movie Spider Beacon)
    bool isLockOn = (signalPercent > 85);
    uint16_t blipColor = isLockOn ? SPIDEY_ALERT : tgt.themeColor;

    tft.fillCircle(blipX, blipY, 3, blipColor);
    tft.drawCircle(blipX, blipY, 5, isLockOn ? SPIDEY_GOLD : SPIDEY_WHITE);

    // Directional vector tracer line when aligned
    if (isLockOn) {
        tft.drawLine(radarCX, radarCY, blipX, blipY, SPIDEY_NEON_RED);
        // Lock-on target brackets `[ + ]`
        tft.drawRect(blipX - 6, blipY - 6, 13, 13, SPIDEY_GOLD);
        ledcWrite(LED_RED_CH, 180);
    } else {
        ledcWrite(LED_RED_CH, (uint8_t)(signalPercent * 0.4f));
    }

    // Redraw center origin dot
    tft.fillCircle(radarCX, radarCY, 2, SPIDEY_GOLD);

    prevBlipX = blipX;
    prevBlipY = blipY;

    // Update Telemetry Panel (Y: 102)
    int panelY = 102;
    char valBuf[16];
    tft.setTextSize(1);

    // Distance
    tft.fillRect(36, panelY + 16, 30, 8, SPIDEY_DARK);
    tft.setTextColor(SPIDEY_WHITE);
    snprintf(valBuf, sizeof(valBuf), "%dm", (int)tgt.currentDist);
    tft.setCursor(36, panelY + 16);
    tft.print(valBuf);

    // Signal %
    tft.fillRect(92, panelY + 16, 28, 8, SPIDEY_DARK);
    tft.setTextColor(isLockOn ? SPIDEY_GOLD : SPIDEY_CYAN);
    snprintf(valBuf, sizeof(valBuf), "%d%%", signalPercent);
    tft.setCursor(92, panelY + 16);
    tft.print(valBuf);

    // Signal Strength Bar
    int barW = SCREEN_W - 16;
    int fillW = (int)((signalPercent / 100.0f) * barW);
    tft.fillRect(8, panelY + 30, barW, 4, SPIDEY_DARKER);
    uint16_t barCol = isLockOn ? SPIDEY_GOLD : (signalPercent > 50 ? SPIDEY_GREEN : SPIDEY_NEON_BLUE);
    tft.fillRect(8, panelY + 30, fillW, 4, barCol);

    return true;
}
