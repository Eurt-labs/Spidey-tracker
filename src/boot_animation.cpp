/*
 * ================================================================
 *  🕷️ SPIDEY TRACKER — Advanced Tactical Boot Animation
 *  Stark Industries / Peter Parker Tactical OS Boot Sequence
 *  Optimized for Portrait 128x160 TFT
 * ================================================================
 */

#include "boot_animation.h"
#include "spider_bitmaps.h"
#include <math.h>

static void drawCyberWebMatrix(Adafruit_ST7735 &tft);
static void drawSpiderVectorScan(Adafruit_ST7735 &tft);
static void drawTacticalMaskHUD(Adafruit_ST7735 &tft);
static void holographicWarpTransition(Adafruit_ST7735 &tft);
static void drawTacticalEye(Adafruit_ST7735 &tft, int cx, int cy, float openness, bool isRight);

// ================================================================
//  MAIN BOOT ANIMATION ENTRY POINT
// ================================================================
void runBootAnimation(Adafruit_ST7735 &tft) {
    // Phase 1: Quantum Neural Web & Cyber Grid Initialization (~1.2s)
    drawCyberWebMatrix(tft);

    // Phase 2: Tactical Spider Crest Laser Synthesis (~1.6s)
    drawSpiderVectorScan(tft);

    // Phase 3: Mature Mask HUD & Dynamic Lens Aperture Calibration (~1.5s)
    drawTacticalMaskHUD(tft);

    // Phase 4: Holographic Matrix Warp Transition (~0.6s)
    holographicWarpTransition(tft);
}

// ================================================================
//  PHASE 1: QUANTUM NEURAL WEB & CYBER GRID INITIALIZATION
// ================================================================
static void drawCyberWebMatrix(Adafruit_ST7735 &tft) {
    tft.fillScreen(SPIDEY_BLACK);

    int cx = SCREEN_W / 2;      // 64
    int cy = SCREEN_H / 2 - 10; // 70

    // Draw HUD Corner Brackets
    tft.drawLine(2, 2, 14, 2, SPIDEY_HUD_FRAME);
    tft.drawLine(2, 2, 2, 14, SPIDEY_HUD_FRAME);
    tft.drawLine(SCREEN_W - 3, 2, SCREEN_W - 15, 2, SPIDEY_HUD_FRAME);
    tft.drawLine(SCREEN_W - 3, 2, SCREEN_W - 3, 14, SPIDEY_HUD_FRAME);
    tft.drawLine(2, SCREEN_H - 3, 14, SCREEN_H - 3, SPIDEY_HUD_FRAME);
    tft.drawLine(2, SCREEN_H - 3, 2, SCREEN_H - 15, SPIDEY_HUD_FRAME);
    tft.drawLine(SCREEN_W - 3, SCREEN_H - 3, SCREEN_W - 15, SCREEN_H - 3, SPIDEY_HUD_FRAME);
    tft.drawLine(SCREEN_W - 3, SCREEN_H - 3, SCREEN_W - 3, SCREEN_H - 15, SPIDEY_HUD_FRAME);

    // Header telemetry
    tft.setTextColor(SPIDEY_CYAN);
    tft.setTextSize(1);
    tft.setCursor(18, 5);
    tft.print("NEURAL LINK 4.2");

    // Breathing LED pulse
    ledcWrite(LED_BLUE_CH, 40);

    // Expanding multi-angle cyber-web strands
    const int numStrands = 12;
    for (int i = 0; i < numStrands; i++) {
        float angle = i * (2.0f * PI / numStrands);
        int endX = cx + (int)(cos(angle) * 75);
        int endY = cy + (int)(sin(angle) * 75);

        // Segmented laser drawing
        for (int s = 1; s <= 10; s++) {
            float t = (float)s / 10.0f;
            int px = cx + (int)(cos(angle) * (t - 0.1f) * 70);
            int py = cy + (int)(sin(angle) * (t - 0.1f) * 70);
            int sx = cx + (int)(cos(angle) * t * 70);
            int sy = cy + (int)(sin(angle) * t * 70);
            tft.drawLine(px, py, sx, sy, SPIDEY_WEB);
        }
        delay(20);
    }

    // Concentric octagonal web rings with rotating sweep ticks
    for (int ring = 1; ring <= 4; ring++) {
        int r = ring * 14;
        for (int i = 0; i < numStrands; i++) {
            float a1 = i * (2.0f * PI / numStrands);
            float a2 = (i + 1) * (2.0f * PI / numStrands);
            int x1 = cx + (int)(cos(a1) * r);
            int y1 = cy + (int)(sin(a1) * r);
            int x2 = cx + (int)(cos(a2) * r);
            int y2 = cy + (int)(sin(a2) * r);
            tft.drawLine(x1, y1, x2, y2, SPIDEY_NEON_BLUE);
            tft.drawPixel(x1, y1, SPIDEY_GOLD);
        }
        delay(40);
    }

    // Center targeting reticle
    tft.drawCircle(cx, cy, 5, SPIDEY_GOLD);
    tft.drawCircle(cx, cy, 8, SPIDEY_NEON_RED);

    // Boot Diagnostic telemetry readout
    const char *logs[] = {
        ">> CORE: ONLINE",
        ">> IMU: 6-DOF LOCK",
        ">> PROTOCOL: ACTIVE"
    };

    tft.setTextSize(1);
    for (int line = 0; line < 3; line++) {
        tft.setTextColor(SPIDEY_AMBER);
        tft.setCursor(6, SCREEN_H - 34 + line * 10);
        tft.print(logs[line]);
        ledcWrite(LED_RED_CH, 60);
        delay(90);
        ledcWrite(LED_RED_CH, 0);
    }

    ledcWrite(LED_BLUE_CH, 0);
    delay(200);
}

// ================================================================
//  PHASE 2: TACTICAL SPIDER CREST LASER SYNTHESIS
// ================================================================
static void drawSpiderVectorScan(Adafruit_ST7735 &tft) {
    tft.fillScreen(SPIDEY_BLACK);

    int cx = SCREEN_W / 2;      // 64
    int cy = SCREEN_H / 2 - 12; // 68

    // HUD Top & Bottom framing brackets
    tft.drawFastHLine(8, 12, SCREEN_W - 16, SPIDEY_HUD_FRAME);
    tft.drawFastHLine(8, SCREEN_H - 16, SCREEN_W - 16, SPIDEY_HUD_FRAME);
    tft.drawPixel(8, 13, SPIDEY_GOLD);
    tft.drawPixel(SCREEN_W - 9, 13, SPIDEY_GOLD);
    tft.drawPixel(8, SCREEN_H - 17, SPIDEY_GOLD);
    tft.drawPixel(SCREEN_W - 9, SCREEN_H - 17, SPIDEY_GOLD);

    // Title HUD Box
    tft.fillRect(18, 4, SCREEN_W - 36, 12, SPIDEY_DARKRED);
    tft.drawRect(18, 4, SCREEN_W - 36, 12, SPIDEY_NEON_RED);
    tft.setTextColor(SPIDEY_WHITE);
    tft.setTextSize(1);
    tft.setCursor(24, 6);
    tft.print("STARK-TECH HUD");

    int logoX = cx - 16;
    int logoY = cy - 16;

    // Laser Beam Scanning synthesis of Spider Crest
    for (int row = 0; row < 32; row++) {
        // Draw laser scan line
        tft.drawFastHLine(logoX - 6, logoY + row, 44, SPIDEY_NEON_BLUE);

        for (int col = 0; col < 32; col++) {
            uint8_t byteIdx = col / 8;
            uint8_t bitIdx = 7 - (col % 8);
            uint8_t bitmapByte = pgm_read_byte(&spider_logo_32x32[row * 4 + byteIdx]);
            if (bitmapByte & (1 << bitIdx)) {
                tft.drawPixel(logoX + col, logoY + row, SPIDEY_NEON_RED);
            }
        }

        delay(18);

        // Erase laser trail
        tft.drawFastHLine(logoX - 6, logoY + row, 6, SPIDEY_BLACK);
        tft.drawFastHLine(logoX + 32, logoY + row, 6, SPIDEY_BLACK);
    }

    // Outer cybernetic targeting brackets around the spider crest
    tft.drawRect(logoX - 6, logoY - 6, 44, 44, SPIDEY_HUD_FRAME);
    tft.drawPixel(logoX - 6, logoY - 6, SPIDEY_GOLD);
    tft.drawPixel(logoX + 37, logoY - 6, SPIDEY_GOLD);
    tft.drawPixel(logoX - 6, logoY + 37, SPIDEY_GOLD);
    tft.drawPixel(logoX + 37, logoY + 37, SPIDEY_GOLD);

    // Pulsing energy equalizer bars at bottom
    for (int step = 0; step < 10; step++) {
        int barY = SCREEN_H - 32;
        tft.fillRect(16, barY, SCREEN_W - 32, 12, SPIDEY_BLACK);

        for (int b = 0; b < 12; b++) {
            int h = (int)(sin(step * 0.5f + b * 0.6f) * 5.0f + 6.0f);
            uint16_t col = (b % 2 == 0) ? SPIDEY_NEON_RED : SPIDEY_CYAN;
            tft.fillRect(18 + b * 8, barY + 12 - h, 5, h, col);
        }

        ledcWrite(LED_BLUE_CH, step * 15);
        ledcWrite(LED_GREEN_CH, step * 5);
        delay(40);
    }

    // Typewriter Subtitle: "PETER PARKER OS"
    const char *sub = "SPIDEY OS v4.2";
    int textX = (SCREEN_W - strlen(sub) * 6) / 2;
    tft.setTextColor(SPIDEY_GOLD);
    tft.setCursor(textX, cy + 26);
    tft.print(sub);

    ledcWrite(LED_BLUE_CH, 0);
    ledcWrite(LED_GREEN_CH, 0);
    delay(400);
}

// ================================================================
//  PHASE 3: MATURE TACTICAL MASK HUD & LENS APERTURE CALIBRATION
// ================================================================
static void drawTacticalMaskHUD(Adafruit_ST7735 &tft) {
    int cx = SCREEN_W / 2;      // 64
    int cy = SCREEN_H / 2 - 5;  // 75

    tft.fillScreen(SPIDEY_CRIMSON);

    // Suit Nano-weave web grid matrix on mask
    for (int i = 0; i < 16; i++) {
        float angle = i * (PI / 8.0f);
        int ex = cx + (int)(cos(angle) * 90);
        int ey = cy + (int)(sin(angle) * 110);
        tft.drawLine(cx, cy, ex, ey, SPIDEY_DARKRED);
    }

    for (int r = 16; r < 90; r += 16) {
        for (int a = 0; a < 360; a += 12) {
            float rad = a * PI / 180.0f;
            int px = cx + (int)(cos(rad) * r);
            int py = cy + (int)(sin(rad) * (r * 1.2f));
            if (px >= 0 && px < SCREEN_W && py >= 0 && py < SCREEN_H) {
                tft.drawPixel(px, py, SPIDEY_DARKRED);
            }
        }
    }

    // Initial eyes
    drawTacticalEye(tft, cx - 24, cy, 1.0f, false);
    drawTacticalEye(tft, cx + 24, cy, 1.0f, true);
    delay(400);

    // Multi-LED Strobe
    ledcWrite(LED_RED_CH, 100);
    ledcWrite(LED_BLUE_CH, 80);
    ledcWrite(LED_GREEN_CH, 40);

    // Aperture Focus & Squint Calibration (Narrowing)
    for (float s = 1.0f; s >= 0.25f; s -= 0.08f) {
        tft.fillRect(cx - 50, cy - 24, 100, 48, SPIDEY_CRIMSON);
        for (int i = 0; i < 16; i++) {
            float angle = i * (PI / 8.0f);
            int ex = cx + (int)(cos(angle) * 55);
            int ey = cy + (int)(sin(angle) * 28);
            tft.drawLine(cx, cy, ex, ey, SPIDEY_DARKRED);
        }
        drawTacticalEye(tft, cx - 24, cy, s, false);
        drawTacticalEye(tft, cx + 24, cy, s, true);
        delay(30);
    }

    delay(200);

    // Tactical Target Lock Reticle over narrowed eyes
    tft.drawCircle(cx, cy, 16, SPIDEY_GOLD);
    tft.drawFastHLine(cx - 22, cy, 10, SPIDEY_GOLD);
    tft.drawFastHLine(cx + 12, cy, 10, SPIDEY_GOLD);
    tft.drawFastVLine(cx, cy - 22, 10, SPIDEY_GOLD);
    tft.drawFastVLine(cx, cy + 12, 10, SPIDEY_GOLD);

    tft.setTextColor(SPIDEY_GOLD);
    tft.setTextSize(1);
    tft.setCursor(cx - 30, SCREEN_H - 18);
    tft.print("[TARGET LOCK]");
    delay(300);

    // Aperture Dilation & Focus Reveal (Widen)
    for (float s = 0.25f; s <= 1.05f; s += 0.08f) {
        float clamped = (s > 1.0f) ? 1.0f : s;
        tft.fillRect(cx - 50, cy - 24, 100, 48, SPIDEY_CRIMSON);
        for (int i = 0; i < 16; i++) {
            float angle = i * (PI / 8.0f);
            int ex = cx + (int)(cos(angle) * 55);
            int ey = cy + (int)(sin(angle) * 28);
            tft.drawLine(cx, cy, ex, ey, SPIDEY_DARKRED);
        }
        drawTacticalEye(tft, cx - 24, cy, clamped, false);
        drawTacticalEye(tft, cx + 24, cy, clamped, true);
        delay(30);
    }

    ledcWrite(LED_RED_CH, 0);
    ledcWrite(LED_BLUE_CH, 0);
    ledcWrite(LED_GREEN_CH, 0);
    delay(350);
}

// ================================================================
//  DRAW TACTICAL MASK EYE (Angular Titanium Bevel + Glow)
// ================================================================
static void drawTacticalEye(Adafruit_ST7735 &tft, int cx, int cy, float openness, bool isRight) {
    int maxW = 22;
    int maxH = 34;
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

        // Inner glowing white lens
        tft.fillRect(startX, cy + row, rowW, 1, SPIDEY_WHITE);

        // Titanium black/dark bevel outline
        if (rowW > 3) {
            tft.drawPixel(startX - 1, cy + row, SPIDEY_BLACK);
            tft.drawPixel(startX - 2, cy + row, SPIDEY_DARK);
            tft.drawPixel(startX + rowW, cy + row, SPIDEY_BLACK);
            tft.drawPixel(startX + rowW + 1, cy + row, SPIDEY_DARK);
        }
    }
}

// ================================================================
//  PHASE 4: HOLOGRAPHIC MATRIX WARP TRANSITION
// ================================================================
static void holographicWarpTransition(Adafruit_ST7735 &tft) {
    // High-tech digital matrix scanline dissolve
    for (int i = 0; i < 400; i++) {
        int x = random(SCREEN_W);
        int y = random(SCREEN_H);
        int len = random(4, 16);
        tft.drawFastHLine(x, y, len, SPIDEY_BLACK);

        if (i % 6 == 0) {
            tft.drawPixel(random(SCREEN_W), random(SCREEN_H), SPIDEY_CYAN);
        }
        if (i % 4 == 0) delay(1);
    }

    // Fast wipe
    for (int y = 0; y < SCREEN_H; y += 4) {
        tft.fillRect(0, y, SCREEN_W, 4, SPIDEY_BLACK);
        delay(4);
    }

    delay(100);
}
