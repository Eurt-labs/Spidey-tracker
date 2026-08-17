/*
 * ================================================================
 *  🕷️ SPIDEY TRACKER — Web Targeting Shooter (Portrait 128x160)
 *  Tilt-Controlled Web Shooter with Laser Crosshair & Multipliers
 * ================================================================
 */

#include "web_shooter_game.h"
#include "spider_bitmaps.h"
#include <math.h>

#define MAX_TARGETS     3
#define TARGET_RADIUS   6
#define CROSSHAIR_SIZE  7
#define HIT_DISTANCE    11
#define SPAWN_INTERVAL  1800
#define GAME_AREA_TOP   14
#define GAME_AREA_BOT   (SCREEN_H - 16)

static bool needsFullRedraw = true;
static unsigned long lastDrawMs = 0;

static float crossX, crossY;
static int prevCrossX, prevCrossY;

static struct {
    int x, y;
    bool active;
    unsigned long spawnTime;
    int size;
} targets[MAX_TARGETS];

static int score = 0;
static int shots = 0;
static int combo = 0;
static int bestCombo = 0;
static unsigned long lastSpawnMs = 0;
static unsigned long gameStartMs = 0;

static void spawnTarget() {
    for (int i = 0; i < MAX_TARGETS; i++) {
        if (!targets[i].active) {
            targets[i].x = random(TARGET_RADIUS + 6, SCREEN_W - TARGET_RADIUS - 6);
            targets[i].y = random(GAME_AREA_TOP + TARGET_RADIUS + 4, GAME_AREA_BOT - TARGET_RADIUS - 4);
            targets[i].active = true;
            targets[i].spawnTime = millis();
            targets[i].size = TARGET_RADIUS;
            break;
        }
    }
}

static void drawGameBackground(Adafruit_ST7735 &tft) {
    tft.fillScreen(SPIDEY_BLACK);

    // Top HUD score bar
    tft.fillRect(0, 0, SCREEN_W, GAME_AREA_TOP, SPIDEY_DARKRED);
    tft.drawFastHLine(0, GAME_AREA_TOP, SCREEN_W, SPIDEY_NEON_RED);

    // Subtle tactical grid
    for (int y = GAME_AREA_TOP + 20; y < GAME_AREA_BOT; y += 24) {
        tft.drawFastHLine(0, y, SCREEN_W, SPIDEY_HUD_GRID);
    }
    for (int x = 20; x < SCREEN_W; x += 24) {
        tft.drawFastVLine(x, GAME_AREA_TOP, GAME_AREA_BOT - GAME_AREA_TOP, SPIDEY_HUD_GRID);
    }

    // Bottom HUD bar
    tft.fillRect(0, GAME_AREA_BOT, SCREEN_W, SCREEN_H - GAME_AREA_BOT, SPIDEY_DARKER);
    tft.drawFastHLine(0, GAME_AREA_BOT, SCREEN_W, SPIDEY_HUD_FRAME);
}

static void drawScoreBar(Adafruit_ST7735 &tft) {
    tft.fillRect(0, 0, SCREEN_W, GAME_AREA_TOP, SPIDEY_DARKRED);

    tft.setTextColor(SPIDEY_WHITE);
    tft.setTextSize(1);
    tft.setCursor(4, 3);
    tft.print("PTS:");
    tft.setTextColor(SPIDEY_GOLD);
    tft.print(score);

    if (combo > 1) {
        tft.setTextColor(SPIDEY_NEON_RED);
        tft.setCursor(62, 3);
        tft.print("x");
        tft.print(combo);
    }

    tft.setTextColor(SPIDEY_CYAN);
    tft.setCursor(90, 3);
    if (shots > 0) {
        int acc = (score * 100) / shots;
        tft.print(acc);
        tft.print("%");
    } else {
        tft.print("100%");
    }
}

static void drawInfoBar(Adafruit_ST7735 &tft) {
    tft.fillRect(0, GAME_AREA_BOT, SCREEN_W, SCREEN_H - GAME_AREA_BOT, SPIDEY_DARKER);

    tft.setTextColor(SPIDEY_WEB);
    tft.setTextSize(1);
    tft.setCursor(4, GAME_AREA_BOT + 4);
    tft.print("TILT:AIM SEL:FIRE");
}

static void drawTarget(Adafruit_ST7735 &tft, int x, int y, int sz, bool erase) {
    if (erase) {
        tft.fillCircle(x, y, sz + 2, SPIDEY_BLACK);
        return;
    }

    tft.drawCircle(x, y, sz, SPIDEY_NEON_RED);
    tft.fillCircle(x, y, 2, SPIDEY_GOLD);

    // Target cross legs
    tft.drawPixel(x - sz + 1, y, SPIDEY_WHITE);
    tft.drawPixel(x + sz - 1, y, SPIDEY_WHITE);
    tft.drawPixel(x, y - sz + 1, SPIDEY_WHITE);
    tft.drawPixel(x, y + sz - 1, SPIDEY_WHITE);
}

static void drawCrosshair(Adafruit_ST7735 &tft, int x, int y, bool erase) {
    uint16_t color = erase ? SPIDEY_BLACK : SPIDEY_GOLD;

    tft.drawFastHLine(x - CROSSHAIR_SIZE, y, CROSSHAIR_SIZE - 2, color);
    tft.drawFastHLine(x + 3, y, CROSSHAIR_SIZE - 2, color);
    tft.drawFastVLine(x, y - CROSSHAIR_SIZE, CROSSHAIR_SIZE - 2, color);
    tft.drawFastVLine(x, y + 3, CROSSHAIR_SIZE - 2, color);

    if (!erase) {
        tft.drawPixel(x, y, SPIDEY_WHITE);
        tft.drawRect(x - 3, y - 3, 7, 7, SPIDEY_NEON_BLUE);
    }
}

void webShooterInit(Adafruit_ST7735 &tft) {
    needsFullRedraw = true;
    crossX = SCREEN_W / 2.0f;
    crossY = (GAME_AREA_TOP + GAME_AREA_BOT) / 2.0f;
    prevCrossX = -1;
    prevCrossY = -1;
    score = 0;
    shots = 0;
    combo = 0;
    bestCombo = 0;
    gameStartMs = millis();
    lastSpawnMs = millis();

    for (int i = 0; i < MAX_TARGETS; i++) {
        targets[i].active = false;
    }

    spawnTarget();
}

bool webShooterUpdate(Adafruit_ST7735 &tft, IMUData &imu, ButtonState &btn) {
    if (btn.selectLongPress) {
        ledcWrite(LED_RED_CH, 0);
        ledcWrite(LED_BLUE_CH, 0);
        ledcWrite(LED_GREEN_CH, 0);
        return false;
    }

    if (millis() - lastDrawMs < 35) return true;
    lastDrawMs = millis();

    if (needsFullRedraw) {
        drawGameBackground(tft);
        drawScoreBar(tft);
        drawInfoBar(tft);
        needsFullRedraw = false;
    }

    // Move crosshair with tilt
    float sensitivity = 3.2f;
    crossX += imu.ax * sensitivity * 0.04f;
    crossY -= imu.ay * sensitivity * 0.04f;

    crossX = CLAMP(crossX, CROSSHAIR_SIZE + 2, SCREEN_W - CROSSHAIR_SIZE - 2);
    crossY = CLAMP(crossY, GAME_AREA_TOP + CROSSHAIR_SIZE + 2, GAME_AREA_BOT - CROSSHAIR_SIZE - 2);

    int cx = (int)crossX;
    int cy = (int)crossY;

    if (prevCrossX >= 0) {
        drawCrosshair(tft, prevCrossX, prevCrossY, true);
    }

    // Shooting
    if (btn.selectPressed) {
        shots++;
        bool hit = false;

        for (int i = 0; i < MAX_TARGETS; i++) {
            if (targets[i].active) {
                float dx = cx - targets[i].x;
                float dy = cy - targets[i].y;
                float dist = sqrtf(dx * dx + dy * dy);

                if (dist < HIT_DISTANCE) {
                    hit = true;
                    score += (combo > 1 ? combo : 1) * 10;
                    combo++;
                    if (combo > bestCombo) bestCombo = combo;

                    // Web shot line
                    tft.drawLine(SCREEN_W / 2, GAME_AREA_BOT, targets[i].x, targets[i].y, SPIDEY_WHITE);
                    delay(25);
                    tft.drawLine(SCREEN_W / 2, GAME_AREA_BOT, targets[i].x, targets[i].y, SPIDEY_BLACK);

                    // Explosion
                    for (int r = targets[i].size; r < targets[i].size + 6; r += 2) {
                        tft.drawCircle(targets[i].x, targets[i].y, r, SPIDEY_GOLD);
                        delay(12);
                    }
                    tft.fillCircle(targets[i].x, targets[i].y, targets[i].size + 8, SPIDEY_BLACK);

                    targets[i].active = false;
                    ledcWrite(LED_GREEN_CH, 100);
                    drawScoreBar(tft);
                    break;
                }
            }
        }

        if (!hit) {
            combo = 0;
            ledcWrite(LED_RED_CH, 80);
            drawScoreBar(tft);
        }
    } else {
        ledcWrite(LED_GREEN_CH, 0);
        ledcWrite(LED_RED_CH, 0);
    }

    unsigned long spawnDelay = max(600UL, SPAWN_INTERVAL - (unsigned long)(score * 3));
    if (millis() - lastSpawnMs > spawnDelay) {
        spawnTarget();
        lastSpawnMs = millis();
    }

    for (int i = 0; i < MAX_TARGETS; i++) {
        if (targets[i].active) {
            if (millis() - targets[i].spawnTime > 4500) {
                drawTarget(tft, targets[i].x, targets[i].y, targets[i].size, true);
                targets[i].active = false;
                combo = 0;
                drawScoreBar(tft);
                continue;
            }

            drawTarget(tft, targets[i].x, targets[i].y, targets[i].size, false);
        }
    }

    drawCrosshair(tft, cx, cy, false);
    prevCrossX = cx;
    prevCrossY = cy;

    return true;
}
