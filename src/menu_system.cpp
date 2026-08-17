/*
 * ================================================================
 *  🕷️ SPIDEY TRACKER — Tactical Menu System (Portrait 128x160)
 *  Stark-Tech HUD Scrollable Menu with Neon Accents
 * ================================================================
 */

#include "menu_system.h"
#include "spider_bitmaps.h"
#include <math.h>

// ================================================================
//  MODULE STATE
// ================================================================
static int selectedIdx = 0;
static int scrollOffset = 0;
static bool needsFullRedraw = true;
static int prevSelectedIdx = -1;
static unsigned long lastAnimMs = 0;
static uint8_t pulsePhase = 0;

// ================================================================
//  INTERNAL HELPERS
// ================================================================

// Draw sleek top status header
static void drawTopBar(Adafruit_ST7735 &tft) {
    // Red/Dark gradient bar
    tft.fillRect(0, 0, SCREEN_W, MENU_TOP_BAR, SPIDEY_DARKRED);
    tft.drawFastHLine(0, MENU_TOP_BAR - 1, SCREEN_W, SPIDEY_NEON_RED);

    // Mini spider crest on left
    tft.drawBitmap(3, 4, spider_mini_12x12, 12, 12, SPIDEY_NEON_RED);

    // Title: SPIDEY TRACKER
    tft.setTextColor(SPIDEY_WHITE);
    tft.setTextSize(1);
    tft.setCursor(18, 6);
    tft.print("SPIDEY TRACKER");

    // WiFi / Status indicator dot (Cyan if time synced, Green if WiFi connected, Dim red if offline)
    uint16_t statusCol = sysStatus.timeSynced ? SPIDEY_CYAN : (sysStatus.wifiConnected ? SPIDEY_GREEN : SPIDEY_DARKRED);
    tft.fillCircle(SCREEN_W - 8, 10, 2, statusCol);
}

// Draw bottom status bar
static void drawBottomBar(Adafruit_ST7735 &tft) {
    int barY = SCREEN_H - MENU_BOTTOM_BAR;

    tft.fillRect(0, barY, SCREEN_W, MENU_BOTTOM_BAR, SPIDEY_DARKER);
    tft.drawFastHLine(0, barY, SCREEN_W, SPIDEY_HUD_FRAME);

    // Index indicator: "[ 1/6 ]"
    char buf[12];
    snprintf(buf, sizeof(buf), "[%d/%d]", selectedIdx + 1, MENU_ITEM_COUNT);
    tft.setTextColor(SPIDEY_GOLD);
    tft.setTextSize(1);
    tft.setCursor(4, barY + 4);
    tft.print(buf);

    // Nav guide
    tft.setTextColor(SPIDEY_WEB);
    tft.setCursor(54, barY + 4);
    tft.print("\x18\x19:NAV SEL:GO");
}

// Draw a single menu item card in portrait layout
static void drawMenuItem(Adafruit_ST7735 &tft, int idx, int yPos, bool isSelected) {
    int x = 4;
    int w = SCREEN_W - 8;      // 120px wide
    int h = MENU_ITEM_HEIGHT;  // 20px high

    if (isSelected) {
        // Selected card: Dark red with glowing neon border and brackets
        tft.fillRect(x, yPos, w, h, SPIDEY_DARKRED);

        // Gold border with corner brackets
        tft.drawRect(x, yPos, w, h, SPIDEY_GOLD);
        tft.fillRect(x, yPos, 3, 3, SPIDEY_NEON_RED);
        tft.fillRect(x + w - 3, yPos, 3, 3, SPIDEY_NEON_RED);
        tft.fillRect(x, yPos + h - 3, 3, 3, SPIDEY_NEON_RED);
        tft.fillRect(x + w - 3, yPos + h - 3, 3, 3, SPIDEY_NEON_RED);

        // Selection arrow
        tft.fillTriangle(x + 3, yPos + 6, x + 3, yPos + h - 7, x + 7, yPos + h / 2, SPIDEY_GOLD);

        // 16x16 icon (white with highlight)
        tft.drawBitmap(x + 10, yPos + 2, menuIcons[idx], 16, 16, SPIDEY_WHITE);

        // Text label (Gold/White)
        tft.setTextColor(SPIDEY_GOLD);
        tft.setTextSize(1);
        tft.setCursor(x + 29, yPos + 6);
        tft.print(menuLabels[idx]);

    } else {
        // Unselected card: Dark panel with subtle frame
        tft.fillRect(x, yPos, w, h, SPIDEY_DARK);
        tft.drawRect(x, yPos, w, h, SPIDEY_HUD_FRAME);

        // Dim icon
        tft.drawBitmap(x + 10, yPos + 2, menuIcons[idx], 16, 16, SPIDEY_CRIMSON);

        // Dim text
        tft.setTextColor(SPIDEY_WEB);
        tft.setTextSize(1);
        tft.setCursor(x + 29, yPos + 6);
        tft.print(menuLabels[idx]);
    }
}

// ================================================================
//  PUBLIC API
// ================================================================

void menuInit() {
    selectedIdx = 0;
    scrollOffset = 0;
    needsFullRedraw = true;
    prevSelectedIdx = -1;
    pulsePhase = 0;
}

AppState menuUpdate(Adafruit_ST7735 &tft, ButtonState &btn) {
    if (btn.upPressed) {
        selectedIdx--;
        if (selectedIdx < 0) selectedIdx = MENU_ITEM_COUNT - 1;
        needsFullRedraw = true;
        ledcWrite(LED_BLUE_CH, 40);
    }
    if (btn.downPressed) {
        selectedIdx++;
        if (selectedIdx >= MENU_ITEM_COUNT) selectedIdx = 0;
        needsFullRedraw = true;
        ledcWrite(LED_BLUE_CH, 40);
    }

    if (!btn.upPressed && !btn.downPressed) {
        ledcWrite(LED_BLUE_CH, 0);
    }

    if (btn.selectPressed) {
        ledcWrite(LED_RED_CH, 120);
        ledcWrite(LED_BLUE_CH, 100);
        ledcWrite(LED_GREEN_CH, 60);
        delay(80);
        ledcWrite(LED_RED_CH, 0);
        ledcWrite(LED_BLUE_CH, 0);
        ledcWrite(LED_GREEN_CH, 0);

        switch (selectedIdx) {
            case 0: return STATE_MOTION_TRACKER;
            case 1: return STATE_CRASH_DETECTOR;
            case 2: return STATE_SPIDER_SENSE;
            case 3: return STATE_MARVEL_TRACKER;
            case 4: return STATE_WEB_SHOOTER;
            case 5: return STATE_STATS;
            default: return STATE_MENU;
        }
    }

    if (selectedIdx < scrollOffset) {
        scrollOffset = selectedIdx;
    }
    if (selectedIdx >= scrollOffset + MENU_VISIBLE) {
        scrollOffset = selectedIdx - MENU_VISIBLE + 1;
    }

    pulsePhase++;
    menuDraw(tft);

    return STATE_MENU;
}

void menuDraw(Adafruit_ST7735 &tft) {
    if (needsFullRedraw) {
        tft.fillScreen(SPIDEY_BLACK);

        drawTopBar(tft);
        drawBottomBar(tft);

        int menuAreaY = MENU_TOP_BAR + 4;
        for (int i = 0; i < MENU_VISIBLE && (scrollOffset + i) < MENU_ITEM_COUNT; i++) {
            int idx = scrollOffset + i;
            int yPos = menuAreaY + i * (MENU_ITEM_HEIGHT + 3);
            drawMenuItem(tft, idx, yPos, idx == selectedIdx);
        }

        needsFullRedraw = false;
        prevSelectedIdx = selectedIdx;

    } else if (prevSelectedIdx != selectedIdx) {
        int menuAreaY = MENU_TOP_BAR + 4;

        if (prevSelectedIdx >= scrollOffset && prevSelectedIdx < scrollOffset + MENU_VISIBLE) {
            int slot = prevSelectedIdx - scrollOffset;
            int yPos = menuAreaY + slot * (MENU_ITEM_HEIGHT + 3);
            drawMenuItem(tft, prevSelectedIdx, yPos, false);
        }

        if (selectedIdx >= scrollOffset && selectedIdx < scrollOffset + MENU_VISIBLE) {
            int slot = selectedIdx - scrollOffset;
            int yPos = menuAreaY + slot * (MENU_ITEM_HEIGHT + 3);
            drawMenuItem(tft, selectedIdx, yPos, true);
        }

        drawBottomBar(tft);
        prevSelectedIdx = selectedIdx;

    } else {
        if (millis() - lastAnimMs > MENU_ANIM_MS) {
            lastAnimMs = millis();
            pulsePhase++;

            int menuAreaY = MENU_TOP_BAR + 4;
            if (selectedIdx >= scrollOffset && selectedIdx < scrollOffset + MENU_VISIBLE) {
                int slot = selectedIdx - scrollOffset;
                int yPos = menuAreaY + slot * (MENU_ITEM_HEIGHT + 3);
                drawMenuItem(tft, selectedIdx, yPos, true);
            }
        }
    }
}

int menuGetSelected() {
    return selectedIdx;
}
