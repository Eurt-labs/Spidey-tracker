/*
 * ================================================================
 *  🕷️ SPIDEY TRACKER — Marvel Cinema Tracker (Portrait 128x160)
 *  Cinematic Movie Countdowns & Themed Marvel Release Schedule
 * ================================================================
 */

#include "marvel_tracker.h"
#include "spider_bitmaps.h"
#include <math.h>

#define MOVIE_COUNT 5

struct MarvelMovie {
    const char *title;
    const char *subtitle;
    uint16_t themeColor;
    uint16_t accentColor;
    unsigned long countdownSec;
};

static const MarvelMovie movies[MOVIE_COUNT] = {
    {
        "SPIDER-MAN",
        "Brand New Day",
        SPIDEY_NEON_RED,
        SPIDEY_NEON_BLUE,
        86400UL * 180
    },
    {
        "AVENGERS",
        "Secret Wars",
        SPIDEY_GOLD,
        SPIDEY_CRIMSON,
        86400UL * 365
    },
    {
        "FANTASTIC 4",
        "First Steps",
        SPIDEY_CYAN,
        SPIDEY_WHITE,
        86400UL * 90
    },
    {
        "BLADE",
        "The Daywalker",
        SPIDEY_ALERT,
        SPIDEY_WEB,
        86400UL * 270
    },
    {
        "X-MEN",
        "Mutant Genesis",
        SPIDEY_GOLD,
        SPIDEY_NEON_BLUE,
        86400UL * 450
    }
};

static int currentMovie = 0;
static bool needsFullRedraw = true;
static unsigned long lastDrawMs = 0;
static unsigned long bootMs = 0;

static void drawMovieCard(Adafruit_ST7735 &tft, int idx) {
    const MarvelMovie &m = movies[idx];

    tft.fillScreen(SPIDEY_BLACK);

    // Top Marvel banner
    tft.fillRect(0, 0, SCREEN_W, 16, SPIDEY_DARKRED);
    tft.drawFastHLine(0, 16, SCREEN_W, m.themeColor);

    tft.setTextColor(SPIDEY_WHITE);
    tft.setTextSize(1);
    tft.setCursor(4, 4);
    tft.print("MARVEL");

    char numBuf[10];
    snprintf(numBuf, sizeof(numBuf), "[%d/%d]", idx + 1, MOVIE_COUNT);
    tft.setTextColor(m.themeColor);
    tft.setCursor(SCREEN_W - strlen(numBuf) * 6 - 4, 4);
    tft.print(numBuf);

    // Movie Title (Large)
    tft.setTextColor(m.themeColor);
    tft.setTextSize(1);
    int titleW = strlen(m.title) * 6;
    tft.setCursor((SCREEN_W - titleW) / 2, 24);
    tft.print(m.title);

    // Subtitle
    tft.setTextColor(m.accentColor);
    int subW = strlen(m.subtitle) * 6;
    tft.setCursor((SCREEN_W - subW) / 2, 36);
    tft.print(m.subtitle);

    // Themed separator line
    tft.drawFastHLine(16, 48, SCREEN_W - 32, SPIDEY_HUD_FRAME);
    tft.drawPixel(16, 48, m.themeColor);
    tft.drawPixel(SCREEN_W - 17, 48, m.themeColor);

    // Center icon badge
    tft.drawRect(SCREEN_W / 2 - 14, 54, 28, 28, SPIDEY_HUD_FRAME);
    tft.fillRect(SCREEN_W / 2 - 13, 55, 26, 26, SPIDEY_DARK);
    tft.drawBitmap(SCREEN_W / 2 - 8, 60, menuIcons[3], 16, 16, m.themeColor);

    // Countdown Box frame (Y: 88 to 138)
    int boxY = 88;
    tft.drawRect(4, boxY, SCREEN_W - 8, 48, SPIDEY_HUD_FRAME);
    tft.fillRect(5, boxY + 1, SCREEN_W - 10, 46, SPIDEY_DARK);

    tft.setTextColor(SPIDEY_WEB);
    tft.setCursor(14, boxY + 4);
    tft.print("TARGET RELEASE");

    // Bottom Navigation Bar
    tft.fillRect(0, SCREEN_H - 16, SCREEN_W, 16, SPIDEY_DARKER);
    tft.drawFastHLine(0, SCREEN_H - 16, SCREEN_W, SPIDEY_HUD_FRAME);

    tft.setTextColor(SPIDEY_WEB);
    tft.setTextSize(1);
    tft.setCursor(4, SCREEN_H - 12);
    tft.print("\x18\x19:BROWSE");
    tft.setCursor(72, SCREEN_H - 12);
    tft.print("HOLD:BACK");
}

static void drawCountdown(Adafruit_ST7735 &tft, int idx) {
    const MarvelMovie &m = movies[idx];

    unsigned long elapsedSec = (millis() - bootMs) / 1000;
    unsigned long remainSec = (elapsedSec < m.countdownSec) ? (m.countdownSec - elapsedSec) : 0;

    unsigned long days = remainSec / 86400;
    unsigned long hours = (remainSec % 86400) / 3600;
    unsigned long mins = (remainSec % 3600) / 60;
    unsigned long secs = remainSec % 60;

    int boxY = 88;

    // Clear inner countdown area
    tft.fillRect(8, boxY + 16, SCREEN_W - 16, 30, SPIDEY_DARK);

    // Large DAYS display
    char dayBuf[16];
    snprintf(dayBuf, sizeof(dayBuf), "%lu DAYS", days);
    int dayW = strlen(dayBuf) * 6;
    tft.setTextColor(m.themeColor);
    tft.setTextSize(1);
    tft.setCursor((SCREEN_W - dayW) / 2, boxY + 16);
    tft.print(dayBuf);

    // Live HH:MM:SS clock
    char timeBuf[20];
    snprintf(timeBuf, sizeof(timeBuf), "%02lu:%02lu:%02lu", hours, mins, secs);
    int timeW = strlen(timeBuf) * 6;
    tft.setTextColor(SPIDEY_WHITE);
    tft.setCursor((SCREEN_W - timeW) / 2, boxY + 30);
    tft.print(timeBuf);

    // Blinking activity dot
    if (secs % 2 == 0) {
        tft.fillCircle(SCREEN_W - 14, boxY + 33, 2, m.themeColor);
    } else {
        tft.fillCircle(SCREEN_W - 14, boxY + 33, 2, SPIDEY_DARK);
    }
}

void marvelTrackerInit(Adafruit_ST7735 &tft) {
    currentMovie = 0;
    needsFullRedraw = true;
    bootMs = millis();
}

bool marvelTrackerUpdate(Adafruit_ST7735 &tft, IMUData &imu, ButtonState &btn) {
    if (btn.selectLongPress) {
        return false;
    }

    if (btn.upPressed) {
        currentMovie--;
        if (currentMovie < 0) currentMovie = MOVIE_COUNT - 1;
        needsFullRedraw = true;
        ledcWrite(LED_BLUE_CH, 40);
    }
    if (btn.downPressed) {
        currentMovie++;
        if (currentMovie >= MOVIE_COUNT) currentMovie = 0;
        needsFullRedraw = true;
        ledcWrite(LED_BLUE_CH, 40);
    }

    if (!btn.upPressed && !btn.downPressed) {
        ledcWrite(LED_BLUE_CH, 0);
    }

    static unsigned long lastCountdownMs = 0;
    bool countdownUpdate = (millis() - lastCountdownMs >= 1000);
    if (countdownUpdate) lastCountdownMs = millis();

    if (needsFullRedraw) {
        drawMovieCard(tft, currentMovie);
        drawCountdown(tft, currentMovie);
        needsFullRedraw = false;
    } else if (countdownUpdate) {
        drawCountdown(tft, currentMovie);
    }

    return true;
}
