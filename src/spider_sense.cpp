/*
 * ================================================================
 *  🕷️ SPIDEY TRACKER — Spider-Sense Threat Perception (Portrait 128x160)
 *  Stark-Tech Neural Threat Perception & Iris Aperture Engine
 * ================================================================
 */

#include "spider_sense.h"
#include <math.h>

static bool needsFullRedraw = true;
static unsigned long lastDrawMs = 0;

static float sensitivity = SENSE_DEFAULT;
static bool senseTriggered = false;
static unsigned long senseStartMs = 0;
static float prevMagnitude = 9.81f;
static float deltaSmooth = 0.0f;

static float eyeOpenness = 1.0f;
static float targetOpenness = 1.0f;
static uint8_t alertPulse = 0;

static void drawTacticalEye(Adafruit_ST7735 &tft, int cx, int cy, float openness, bool isRight) {
    int maxW = 20;
    int maxH = 32;
    int h = (int)(maxH * openness);
    if (h < 4) h = 4;
    int halfH = h / 2;

    for (int row = -halfH; row <= halfH; row++) {
        float t = 1.0f - fabsf((float)row / (float)halfH);
        float widthFactor = powf(t, 0.55f);
        int rowW = (int)(maxW * widthFactor);
        if (rowW < 2) rowW = 2;

        int offsetX = isRight ? (int)(row * 0.28f) : (int)(-row * 0.28f);
        int startX = cx - rowW / 2 + offsetX;

        tft.fillRect(startX, cy + row, rowW, 1, SPIDEY_WHITE);

        if (rowW > 3) {
            tft.drawPixel(startX - 1, cy + row, SPIDEY_BLACK);
            tft.drawPixel(startX - 2, cy + row, SPIDEY_DARK);
            tft.drawPixel(startX + rowW, cy + row, SPIDEY_BLACK);
            tft.drawPixel(startX + rowW + 1, cy + row, SPIDEY_DARK);
        }
    }
}

static void drawBackground(Adafruit_ST7735 &tft) {
    tft.fillScreen(SPIDEY_CRIMSON);

    int cx = SCREEN_W / 2;      // 64
    int cy = SCREEN_H / 2 - 12; // 68

    // Suit nano-fiber web lines
    for (int i = 0; i < 16; i++) {
        float angle = i * (PI / 8.0f);
        int ex = cx + (int)(cos(angle) * 85);
        int ey = cy + (int)(sin(angle) * 105);
        tft.drawLine(cx, cy, ex, ey, SPIDEY_DARKRED);
    }

    for (int r = 16; r < 80; r += 16) {
        for (int a = 0; a < 360; a += 12) {
            float rad = a * PI / 180.0f;
            int px = cx + (int)(cos(rad) * r);
            int py = cy + (int)(sin(rad) * (r * 1.2f));
            if (px >= 0 && px < SCREEN_W && py >= 0 && py < SCREEN_H) {
                tft.drawPixel(px, py, SPIDEY_DARKRED);
            }
        }
    }

    // Top status bar
    tft.fillRect(0, 0, SCREEN_W, 14, SPIDEY_DARKRED);
    tft.drawFastHLine(0, 14, SCREEN_W, SPIDEY_NEON_RED);
    tft.setTextColor(SPIDEY_WHITE);
    tft.setTextSize(1);
    tft.setCursor(18, 3);
    tft.print("SPIDER-SENSE");

    // Bottom Sensitivity panel (Y: 130 to 160)
    tft.fillRect(0, SCREEN_H - 26, SCREEN_W, 26, SPIDEY_DARKER);
    tft.drawFastHLine(0, SCREEN_H - 26, SCREEN_W, SPIDEY_HUD_FRAME);

    tft.setTextColor(SPIDEY_CYAN);
    tft.setTextSize(1);
    tft.setCursor(4, SCREEN_H - 22);
    tft.print("SENS:");

    tft.setTextColor(SPIDEY_WEB);
    tft.setCursor(4, SCREEN_H - 10);
    tft.print("\x18\x19:ADJ  HOLD:BACK");
}

static void drawSensitivityBar(Adafruit_ST7735 &tft) {
    int barX = 36;
    int barY = SCREEN_H - 22;
    int barW = 54;
    int barH = 6;

    tft.fillRect(barX, barY, barW + 2, barH + 2, SPIDEY_DARKER);
    tft.drawRect(barX, barY, barW, barH, SPIDEY_HUD_FRAME);

    float t = (sensitivity - SENSE_MIN) / (SENSE_MAX - SENSE_MIN);
    int fillW = (int)(t * (barW - 2));
    tft.fillRect(barX + 1, barY + 1, fillW, barH - 2, SPIDEY_GOLD);

    char buf[8];
    tft.fillRect(barX + barW + 4, barY - 1, 30, 8, SPIDEY_DARKER);
    tft.setTextColor(SPIDEY_WHITE);
    tft.setTextSize(1);
    dtostrf(sensitivity, 3, 1, buf);
    tft.setCursor(barX + barW + 4, barY - 1);
    tft.print(buf);
}

void spiderSenseInit(Adafruit_ST7735 &tft) {
    needsFullRedraw = true;
    senseTriggered = false;
    eyeOpenness = 1.0f;
    targetOpenness = 1.0f;
    deltaSmooth = 0.0f;
    alertPulse = 0;
}

bool spiderSenseUpdate(Adafruit_ST7735 &tft, IMUData &imu, ButtonState &btn) {
    if (btn.selectLongPress) {
        ledcWrite(LED_RED_CH, 0);
        ledcWrite(LED_BLUE_CH, 0);
        ledcWrite(LED_GREEN_CH, 0);
        return false;
    }

    if (btn.upPressed) {
        sensitivity += SENSE_STEP;
        if (sensitivity > SENSE_MAX) sensitivity = SENSE_MAX;
        drawSensitivityBar(tft);
    }
    if (btn.downPressed) {
        sensitivity -= SENSE_STEP;
        if (sensitivity < SENSE_MIN) sensitivity = SENSE_MIN;
        drawSensitivityBar(tft);
    }

    if (millis() - lastDrawMs < 35) return true;
    lastDrawMs = millis();

    if (needsFullRedraw) {
        drawBackground(tft);
        drawSensitivityBar(tft);
        needsFullRedraw = false;
    }

    float delta = fabsf(imu.magnitude - prevMagnitude);
    deltaSmooth = deltaSmooth * 0.6f + delta * 0.4f;
    prevMagnitude = imu.magnitude;

    if (deltaSmooth > sensitivity && !senseTriggered) {
        senseTriggered = true;
        senseStartMs = millis();
        alertPulse = 0;
    }

    int cx = SCREEN_W / 2;      // 64
    int cy = SCREEN_H / 2 - 12; // 68

    if (senseTriggered) {
        unsigned long elapsed = millis() - senseStartMs;

        if (elapsed < 2400) {
            targetOpenness = 1.3f + sinf(elapsed * 0.015f) * 0.2f;

            alertPulse++;
            bool textOn = (alertPulse / 3) % 2 == 0;

            uint16_t borderColor = textOn ? SPIDEY_GOLD : SPIDEY_CRIMSON;
            tft.drawRect(0, 14, SCREEN_W, SCREEN_H - 40, borderColor);
            tft.drawRect(1, 15, SCREEN_W - 2, SCREEN_H - 42, borderColor);

            if (textOn) {
                tft.fillRect(4, 18, SCREEN_W - 8, 11, SPIDEY_BLACK);
                tft.setTextColor(SPIDEY_GOLD);
                tft.setTextSize(1);
                tft.setCursor(14, 20);
                tft.print("! THREAT ALERT !");
            } else {
                tft.fillRect(4, 18, SCREEN_W - 8, 11, SPIDEY_CRIMSON);
            }

            if (textOn) {
                ledcWrite(LED_RED_CH, 220);
                ledcWrite(LED_BLUE_CH, 40);
            } else {
                ledcWrite(LED_RED_CH, 40);
                ledcWrite(LED_BLUE_CH, 220);
            }
            ledcWrite(LED_GREEN_CH, (alertPulse * 6) % 50);

        } else {
            senseTriggered = false;
            targetOpenness = 1.0f;

            tft.drawRect(0, 14, SCREEN_W, SCREEN_H - 40, SPIDEY_CRIMSON);
            tft.drawRect(1, 15, SCREEN_W - 2, SCREEN_H - 42, SPIDEY_CRIMSON);
            tft.fillRect(4, 18, SCREEN_W - 8, 11, SPIDEY_CRIMSON);

            ledcWrite(LED_RED_CH, 0);
            ledcWrite(LED_BLUE_CH, 0);
            ledcWrite(LED_GREEN_CH, 0);

            needsFullRedraw = true;
        }
    } else {
        targetOpenness = 0.9f + sinf(millis() * 0.003f) * 0.08f;
        if (deltaSmooth > sensitivity * 0.5f) {
            targetOpenness = 0.6f;
        }
    }

    eyeOpenness += (targetOpenness - eyeOpenness) * 0.35f;
    float clampedOpen = CLAMP(eyeOpenness, 0.25f, 1.4f);

    // Redraw mask eyes area
    tft.fillRect(cx - 48, cy - 20, 96, 40, SPIDEY_CRIMSON);

    for (int i = 0; i < 16; i++) {
        float angle = i * (PI / 8.0f);
        int ex = cx + (int)(cos(angle) * 50);
        int ey = cy + (int)(sin(angle) * 24);
        tft.drawLine(cx, cy, ex, ey, SPIDEY_DARKRED);
    }

    drawTacticalEye(tft, cx - 22, cy, clampedOpen, false);
    drawTacticalEye(tft, cx + 22, cy, clampedOpen, true);

    return true;
}
