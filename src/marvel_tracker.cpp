/*
 * ================================================================
 *  🕷️ SPIDEY TRACKER — Marvel Cinema Tracker (Portrait 128x160)
 *  Accurate NTP-Synced Real-Time Movie Release & Post-Release Tracker
 * ================================================================
 */

#include "marvel_tracker.h"
#include "spider_bitmaps.h"
#include "config.h"
#include <time.h>
#include <math.h>

#define MOVIE_COUNT 6

struct MarvelMovie {
    const char *title;
    const char *subtitle;
    const char *releaseDateStr;
    int year;
    int month;
    int day;
    uint16_t themeColor;
    uint16_t accentColor;
};

// Accurate timeline with upcoming blockbusters & released milestones
static const MarvelMovie movies[MOVIE_COUNT] = {
    {
        "SPIDER-MAN",
        "Brand New Day",
        "24 JUL 2026",
        2026, 7, 24,
        SPIDEY_NEON_RED,
        SPIDEY_NEON_BLUE
    },
    {
        "AVENGERS",
        "Doomsday",
        "18 DEC 2026",
        2026, 12, 18,
        SPIDEY_GOLD,
        SPIDEY_NEON_RED
    },
    {
        "AVENGERS",
        "Secret Wars",
        "17 DEC 2027",
        2027, 12, 17,
        SPIDEY_GOLD,
        SPIDEY_CRIMSON
    },
    {
        "SPIDER-VERSE",
        "Beyond Spider-Verse",
        "25 JUN 2027",
        2027, 6, 25,
        SPIDEY_NEON_BLUE,
        SPIDEY_GOLD
    },
    {
        "FANTASTIC 4",
        "First Steps",
        "25 JUL 2025",
        2025, 7, 25,
        SPIDEY_CYAN,
        SPIDEY_WHITE
    },
    {
        "SPIDER-MAN",
        "No Way Home",
        "17 DEC 2021",
        2021, 12, 17,
        SPIDEY_NEON_RED,
        SPIDEY_GOLD
    }
};

static int currentMovie = 0;
static bool needsFullRedraw = true;
static unsigned long lastDrawMs = 0;

static time_t getMovieEpoch(const MarvelMovie &m) {
    struct tm t = {0};
    t.tm_year = m.year - 1900;
    t.tm_mon  = m.month - 1;
    t.tm_mday = m.day;
    t.tm_hour = 0;
    t.tm_min  = 0;
    t.tm_sec  = 0;
    t.tm_isdst = -1;
    return mktime(&t);
}

static void drawMovieCard(Adafruit_ST7735 &tft, int idx) {
    const MarvelMovie &m = movies[idx];
    time_t targetEpoch = getMovieEpoch(m);
    time_t now = time(nullptr);
    bool isReleased = (now > 1000000 && now >= targetEpoch);

    tft.fillScreen(SPIDEY_BLACK);

    // Top Marvel banner
    tft.fillRect(0, 0, SCREEN_W, 16, SPIDEY_DARKRED);
    tft.drawFastHLine(0, 16, SCREEN_W, m.themeColor);

    tft.setTextColor(SPIDEY_WHITE);
    tft.setTextSize(1);
    tft.setCursor(4, 4);
    tft.print("MARVEL");

    // Index e.g. "[1/6]"
    char numBuf[10];
    snprintf(numBuf, sizeof(numBuf), "[%d/%d]", idx + 1, MOVIE_COUNT);
    tft.setTextColor(m.themeColor);
    tft.setCursor(SCREEN_W - strlen(numBuf) * 6 - 4, 4);
    tft.print(numBuf);

    // Movie Title
    tft.setTextColor(m.themeColor);
    tft.setTextSize(1);
    int titleW = strlen(m.title) * 6;
    tft.setCursor((SCREEN_W - titleW) / 2, 22);
    tft.print(m.title);

    // Subtitle
    tft.setTextColor(m.accentColor);
    int subW = strlen(m.subtitle) * 6;
    tft.setCursor((SCREEN_W - subW) / 2, 33);
    tft.print(m.subtitle);

    // Separator line
    tft.drawFastHLine(16, 44, SCREEN_W - 32, SPIDEY_HUD_FRAME);
    tft.drawPixel(16, 44, m.themeColor);
    tft.drawPixel(SCREEN_W - 17, 44, m.themeColor);

    // Release Date Badge
    tft.setTextColor(SPIDEY_GOLD);
    tft.setTextSize(1);
    char dateBuf[24];
    snprintf(dateBuf, sizeof(dateBuf), "DATE: %s", m.releaseDateStr);
    int dateW = strlen(dateBuf) * 6;
    tft.setCursor((SCREEN_W - dateW) / 2, 48);
    tft.print(dateBuf);

    // Center icon badge
    tft.drawRect(SCREEN_W / 2 - 12, 59, 24, 24, SPIDEY_HUD_FRAME);
    tft.fillRect(SCREEN_W / 2 - 11, 60, 22, 22, SPIDEY_DARK);
    tft.drawBitmap(SCREEN_W / 2 - 8, 63, menuIcons[3], 16, 16, m.themeColor);

    // Countdown / Elapsed Box frame (Y: 86 to 140)
    int boxY = 86;
    tft.drawRect(4, boxY, SCREEN_W - 8, 52, SPIDEY_HUD_FRAME);
    tft.fillRect(5, boxY + 1, SCREEN_W - 10, 50, SPIDEY_DARK);

    // Network Sync / Mode Tag
    tft.setTextSize(1);
    if (isReleased) {
        tft.setTextColor(SPIDEY_GREEN);
        tft.setCursor(10, boxY + 4);
        tft.print("[MOVIE RELEASED]");
    } else if (sysStatus.timeSynced) {
        tft.setTextColor(SPIDEY_CYAN);
        tft.setCursor(10, boxY + 4);
        tft.print("[NTP SYNCED]");
    } else if (sysStatus.wifiConnected) {
        tft.setTextColor(SPIDEY_GREEN);
        tft.setCursor(10, boxY + 4);
        tft.print("[WIFI SYNCING]");
    } else {
        tft.setTextColor(SPIDEY_AMBER);
        tft.setCursor(10, boxY + 4);
        tft.print("[CONNECTING WIFI]");
    }

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
    time_t targetEpoch = getMovieEpoch(m);
    time_t now = time(nullptr);

    bool isPast = false;
    unsigned long diffSec = 0;

    if (now > 1000000) { // Valid NTP time
        if (now >= targetEpoch) {
            isPast = true;
            diffSec = (unsigned long)(now - targetEpoch);
        } else {
            isPast = false;
            diffSec = (unsigned long)(targetEpoch - now);
        }
    } else {
        // Fallback before NTP lock (estimated from 2026 reference)
        diffSec = 86400UL * 120 + 3600UL * 14;
    }

    unsigned long days = diffSec / 86400;
    unsigned long hours = (diffSec % 86400) / 3600;
    unsigned long mins = (diffSec % 3600) / 60;
    unsigned long secs = diffSec % 60;

    int boxY = 86;

    // Clear inner countdown area
    tft.fillRect(8, boxY + 16, SCREEN_W - 16, 32, SPIDEY_DARK);

    if (isPast) {
        // MOVIE IS ALREADY RELEASED: Show "DAYS GONE SINCE RELEASE"
        char dayBuf[24];
        snprintf(dayBuf, sizeof(dayBuf), "+%lu DAYS AGO", days);
        int dayW = strlen(dayBuf) * 6;
        tft.setTextColor(SPIDEY_GREEN);
        tft.setTextSize(1);
        tft.setCursor((SCREEN_W - dayW) / 2, boxY + 18);
        tft.print(dayBuf);

        // Elapsed HH:MM:SS clock
        char timeBuf[24];
        snprintf(timeBuf, sizeof(timeBuf), "+%02lu:%02lu:%02lu", hours, mins, secs);
        int timeW = strlen(timeBuf) * 6;
        tft.setTextColor(SPIDEY_GOLD);
        tft.setCursor((SCREEN_W - timeW) / 2, boxY + 32);
        tft.print(timeBuf);

        // Blinking indicator dot
        if (secs % 2 == 0) {
            tft.fillCircle(SCREEN_W - 14, boxY + 35, 2, SPIDEY_GREEN);
        } else {
            tft.fillCircle(SCREEN_W - 14, boxY + 35, 2, SPIDEY_DARK);
        }
    } else {
        // UPCOMING MOVIE: Show "DAYS UNTIL RELEASE"
        char dayBuf[20];
        snprintf(dayBuf, sizeof(dayBuf), "%lu DAYS LEFT", days);
        int dayW = strlen(dayBuf) * 6;
        tft.setTextColor(m.themeColor);
        tft.setTextSize(1);
        tft.setCursor((SCREEN_W - dayW) / 2, boxY + 18);
        tft.print(dayBuf);

        // Live HH:MM:SS countdown clock
        char timeBuf[24];
        snprintf(timeBuf, sizeof(timeBuf), "%02lu:%02lu:%02lu", hours, mins, secs);
        int timeW = strlen(timeBuf) * 6;
        tft.setTextColor(SPIDEY_WHITE);
        tft.setCursor((SCREEN_W - timeW) / 2, boxY + 32);
        tft.print(timeBuf);

        // Blinking indicator dot
        if (secs % 2 == 0) {
            tft.fillCircle(SCREEN_W - 14, boxY + 35, 2, m.themeColor);
        } else {
            tft.fillCircle(SCREEN_W - 14, boxY + 35, 2, SPIDEY_DARK);
        }
    }
}

void marvelTrackerInit(Adafruit_ST7735 &tft) {
    currentMovie = 0;
    needsFullRedraw = true;
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
