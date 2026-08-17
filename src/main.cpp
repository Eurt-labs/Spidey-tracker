/*
 * ================================================================
 *  🕷️ SPIDEY TRACKER — Main Entry Point (Portrait 128x160)
 *  ESP32 + ST7735 TFT + MPU6050 IMU + LEDs + Buttons
 *
 *  Advanced Spider-Man Themed Tactical OS & Tracker System
 * ================================================================
 */

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#include "config.h"
#include "boot_animation.h"
#include "menu_system.h"
#include "motion_tracker.h"
#include "crash_detector.h"
#include "spider_sense.h"
#include "marvel_tracker.h"
#include "web_shooter_game.h"
#include "stats_screen.h"

// ================================================================
//  GLOBAL OBJECTS
// ================================================================

// TFT Display (SPI - VSPI bus)
Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_RST);

// Application state
AppState currentState = STATE_BOOT;
AppState previousState = STATE_BOOT;

// Shared sensor data
IMUData imuData = {0};
ButtonState buttons = {0};

// Frame timing
unsigned long frameStartMs = 0;

// ================================================================
//  IMU (MPU6050/6500) FUNCTIONS
// ================================================================

static uint8_t imuReadWhoAmI() {
    Wire.beginTransmission(IMU_ADDR);
    Wire.write(0x75);  // WHO_AM_I register
    Wire.endTransmission(false);
    Wire.requestFrom((uint8_t)IMU_ADDR, (uint8_t)1, (uint8_t)true);
    if (Wire.available()) {
        return Wire.read();
    }
    return 0x00;
}

static bool imuInit() {
    Wire.begin(IMU_SDA, IMU_SCL);
    Wire.setClock(100000);  // 100kHz I2C

    delay(50);  // Let IMU power up

    imuData.chipID = imuReadWhoAmI();
    Serial.print("[IMU] WHO_AM_I: 0x");
    Serial.println(imuData.chipID, HEX);

    if (imuData.chipID != 0x68 && imuData.chipID != 0x70) {
        Serial.println("[IMU] WARNING: Unexpected chip ID!");
    }

    // Wake up MPU (clear SLEEP bit in PWR_MGMT_1)
    Wire.beginTransmission(IMU_ADDR);
    Wire.write(0x6B);  // PWR_MGMT_1
    Wire.write(0x00);
    Wire.endTransmission(true);
    delay(10);

    // Set accelerometer range to ±4g
    Wire.beginTransmission(IMU_ADDR);
    Wire.write(0x1C);  // ACCEL_CONFIG
    Wire.write(0x08);  // AFS_SEL = 1 (±4g)
    Wire.endTransmission(true);

    // Set gyroscope range to ±500°/s
    Wire.beginTransmission(IMU_ADDR);
    Wire.write(0x1B);  // GYRO_CONFIG
    Wire.write(0x08);  // FS_SEL = 1 (±500°/s)
    Wire.endTransmission(true);

    // Set DLPF for noise filtering (bandwidth ~44Hz)
    Wire.beginTransmission(IMU_ADDR);
    Wire.write(0x1A);  // CONFIG
    Wire.write(0x03);  // DLPF_CFG = 3
    Wire.endTransmission(true);

    // Set sample rate divider (100Hz)
    Wire.beginTransmission(IMU_ADDR);
    Wire.write(0x19);  // SMPLRT_DIV
    Wire.write(0x09);
    Wire.endTransmission(true);

    imuData.valid = true;
    Serial.println("[IMU] Initialized OK");
    return true;
}

static void imuRead() {
    Wire.beginTransmission(IMU_ADDR);
    Wire.write(0x3B);  // Starting register: ACCEL_XOUT_H
    Wire.endTransmission(false);
    Wire.requestFrom((uint8_t)IMU_ADDR, (uint8_t)14, (uint8_t)true);

    if (Wire.available() < 14) {
        imuData.valid = false;
        return;
    }

    int16_t rawAx = ((int16_t)Wire.read() << 8) | Wire.read();
    int16_t rawAy = ((int16_t)Wire.read() << 8) | Wire.read();
    int16_t rawAz = ((int16_t)Wire.read() << 8) | Wire.read();
    int16_t rawTemp = ((int16_t)Wire.read() << 8) | Wire.read();
    int16_t rawGx = ((int16_t)Wire.read() << 8) | Wire.read();
    int16_t rawGy = ((int16_t)Wire.read() << 8) | Wire.read();
    int16_t rawGz = ((int16_t)Wire.read() << 8) | Wire.read();

    const float accelScale = 9.81f / 8192.0f;
    imuData.ax = rawAx * accelScale;
    imuData.ay = rawAy * accelScale;
    imuData.az = rawAz * accelScale;

    imuData.temp = rawTemp / 340.0f + 36.53f;

    const float gyroScale = 1.0f / 65.5f;
    imuData.gx = rawGx * gyroScale;
    imuData.gy = rawGy * gyroScale;
    imuData.gz = rawGz * gyroScale;

    imuData.magnitude = sqrtf(
        imuData.ax * imuData.ax +
        imuData.ay * imuData.ay +
        imuData.az * imuData.az
    );

    imuData.valid = true;
}

// ================================================================
//  BUTTON HANDLING
// ================================================================
static unsigned long lastBtnReadMs = 0;

static void readButtons() {
    if (millis() - lastBtnReadMs < DEBOUNCE_MS) {
        buttons.upPressed = false;
        buttons.downPressed = false;
        buttons.selectPressed = false;
        buttons.selectLongPress = false;
        return;
    }
    lastBtnReadMs = millis();

    bool upNow = !digitalRead(BTN_UP);
    bool downNow = !digitalRead(BTN_DOWN);
    bool selectNow = !digitalRead(BTN_SELECT);

    buttons.upPressed = upNow && !buttons._upLast;
    buttons.downPressed = downNow && !buttons._downLast;
    buttons.selectPressed = selectNow && !buttons._selectLast;

    buttons.selectHeld = selectNow;

    if (selectNow && !buttons._selectLast) {
        buttons._selectStart = millis();
        buttons._longFired = false;
    }

    if (selectNow && !buttons._longFired &&
        (millis() - buttons._selectStart > LONG_PRESS_MS)) {
        buttons.selectLongPress = true;
        buttons._longFired = true;
    } else {
        buttons.selectLongPress = false;
    }

    buttons._upLast = upNow;
    buttons._downLast = downNow;
    buttons._selectLast = selectNow;
}

// ================================================================
//  LED INITIALIZATION
// ================================================================
static void initLEDs() {
    ledcSetup(LED_BLUE_CH, LED_PWM_FREQ, LED_PWM_RES);
    ledcAttachPin(LED_BLUE, LED_BLUE_CH);

    ledcSetup(LED_GREEN_CH, LED_PWM_FREQ, LED_PWM_RES);
    ledcAttachPin(LED_GREEN, LED_GREEN_CH);

    ledcSetup(LED_RED_CH, LED_PWM_FREQ, LED_PWM_RES);
    ledcAttachPin(LED_RED, LED_RED_CH);

    ledcWrite(LED_BLUE_CH, 0);
    ledcWrite(LED_GREEN_CH, 0);
    ledcWrite(LED_RED_CH, 0);

    Serial.println("[LED] PWM channels initialized");
}

static void allLEDsOff() {
    ledcWrite(LED_BLUE_CH, 0);
    ledcWrite(LED_GREEN_CH, 0);
    ledcWrite(LED_RED_CH, 0);
}

// ================================================================
//  STATE TRANSITION HANDLER
// ================================================================
static void enterState(AppState newState) {
    if (newState == currentState) return;

    previousState = currentState;
    currentState = newState;
    allLEDsOff();

    Serial.print("[STATE] Transition to state: ");
    Serial.println(newState);

    switch (newState) {
        case STATE_MENU:
            menuInit();
            break;
        case STATE_MOTION_TRACKER:
            motionTrackerInit(tft);
            break;
        case STATE_CRASH_DETECTOR:
            crashDetectorInit(tft);
            break;
        case STATE_SPIDER_SENSE:
            spiderSenseInit(tft);
            break;
        case STATE_MARVEL_TRACKER:
            marvelTrackerInit(tft);
            break;
        case STATE_WEB_SHOOTER:
            webShooterInit(tft);
            break;
        case STATE_STATS:
            statsScreenInit(tft);
            break;
        default:
            break;
    }
}

// ================================================================
//  ARDUINO SETUP
// ================================================================
void setup() {
    Serial.begin(115200);
    delay(100);
    Serial.println();
    Serial.println("========================================");
    Serial.println(" 🕷️ SPIDEY TRACKER TACTICAL OS v4.2");
    Serial.println(" ESP32 DevKit V1 + ST7735 (128x160 Portrait)");
    Serial.println("========================================");

    pinMode(BTN_UP, INPUT_PULLUP);
    pinMode(BTN_SELECT, INPUT_PULLUP);
    pinMode(BTN_DOWN, INPUT_PULLUP);
    pinMode(IMU_INT, INPUT);

    initLEDs();

    // Initialize TFT Display in Vertical / Portrait mode (128 x 160)
    tft.initR(INITR_BLACKTAB);
    tft.setRotation(TFT_ROTATION); // 0 = Vertical / Portrait
    tft.fillScreen(SPIDEY_BLACK);
    Serial.println("[TFT] Display initialized (128x160 Portrait)");

    bool imuOK = imuInit();
    if (!imuOK) {
        Serial.println("[IMU] WARNING: IMU communication failed");
    }

    randomSeed(analogRead(36) ^ micros());

    Serial.println("[BOOT] Launching cinematic boot sequence...");
    currentState = STATE_BOOT;
    runBootAnimation(tft);
    Serial.println("[BOOT] Boot sequence complete");

    enterState(STATE_MENU);
}

// ================================================================
//  ARDUINO LOOP
// ================================================================
void loop() {
    frameStartMs = millis();

    readButtons();

    if (imuData.valid || imuData.chipID != 0x00) {
        imuRead();
    }

    switch (currentState) {
        case STATE_MENU: {
            AppState result = menuUpdate(tft, buttons);
            if (result != STATE_MENU) {
                enterState(result);
            }
            break;
        }

        case STATE_MOTION_TRACKER: {
            bool stay = motionTrackerUpdate(tft, imuData, buttons);
            if (!stay) enterState(STATE_MENU);
            break;
        }

        case STATE_CRASH_DETECTOR: {
            bool stay = crashDetectorUpdate(tft, imuData, buttons);
            if (!stay) enterState(STATE_MENU);
            break;
        }

        case STATE_SPIDER_SENSE: {
            bool stay = spiderSenseUpdate(tft, imuData, buttons);
            if (!stay) enterState(STATE_MENU);
            break;
        }

        case STATE_MARVEL_TRACKER: {
            bool stay = marvelTrackerUpdate(tft, imuData, buttons);
            if (!stay) enterState(STATE_MENU);
            break;
        }

        case STATE_WEB_SHOOTER: {
            bool stay = webShooterUpdate(tft, imuData, buttons);
            if (!stay) enterState(STATE_MENU);
            break;
        }

        case STATE_STATS: {
            bool stay = statsScreenUpdate(tft, imuData, buttons);
            if (!stay) enterState(STATE_MENU);
            break;
        }

        default:
            enterState(STATE_MENU);
            break;
    }

    unsigned long elapsed = millis() - frameStartMs;
    if (elapsed < FRAME_DELAY_MS) {
        delay(FRAME_DELAY_MS - elapsed);
    }
}
