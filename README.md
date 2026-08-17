# 🕷️ Spidey Tracker — Tactical ESP32 Multi-Tracker

[![Platform](https://img.shields.io/badge/Platform-ESP32-red.svg)](https://www.espressif.com/en/products/socs/esp32)
[![Framework](https://img.shields.io/badge/Framework-Arduino-blue.svg)](https://www.arduino.cc/)
[![IDE](https://img.shields.io/badge/Build-PlatformIO-orange.svg)](https://platformio.org/)
[![Display](https://img.shields.io/badge/Display-ST7735%201.8%22%20TFT-yellow.svg)](https://www.adafruit.com/)
[![IMU](https://img.shields.io/badge/Sensor-MPU6050%20%2F%206500-green.svg)](https://invensense.tdk.com/)
[![WiFi](https://img.shields.io/badge/WiFi%2FNTP-Auto--Sync-cyan.svg)](https://www.espressif.com/)
[![License](https://img.shields.io/badge/License-MIT-blueviolet.svg)](LICENSE)

An advanced, mature **Spidey Tracker & Tactical Multi-Tracker** built for the **ESP32 DevKit V1**, featuring a **128×160 Portrait ST7735 TFT**, **MPU6050 6-DOF IMU**, **WiFi NTP real-time clock synchronization**, **PWM status LEDs**, and a 3-button tactical navigation system.

---

## 🌟 Key Highlights

- **🎬 4-Phase Cinematic Boot Sequence**:
  1. *Quantum Neural Web & Cyber Grid* with live telemetry logs.
  2. *Tactical Spider Crest Laser Scan* with real-time frequency equalizer animation.
  3. *Tactical Mask HUD* with dynamic iris aperture calibration, focus lock reticles, and LED strobe synchronization.
  4. *Holographic Matrix Warp* transition into the tactical main menu.
- **📶 Real-Time WiFi NTP Sync**: Connects to WiFi (`Airtel_Dhruv`) to fetch accurate global atomic time from NTP pool servers (`pool.ntp.org`, `time.google.com` with UTC+5:30 IST offset).
- **🎬 Official Marvel Timeline & Release Tracker**:
  - *Upcoming Releases*: Live countdown (`DAYS LEFT` + `HH:MM:SS`) to premiere.
  - *Released Milestones*: Live elapsed time (`+DAYS AGO` + `+HH:MM:SS`) since premiere date.
- **📱 6 Interactive Tactical Modes** in high-contrast Spider-Man aesthetics (Neon Red, Cyan, Gold, Crimson).
- **🕹️ 128×160 Vertical / Portrait Orientation**: Optimized layouts, typography, and vector graphics designed specifically for 1.8" TFT screens.
- **⚡ Ultra-Efficient Hardware Rendering**: Direct register-level MPU6050 communication, debounced button handling with long-press detection, and PWM-dimmed status LEDs.

---

## 🎮 Features & Modes

```
┌──────────────────────────────────────────────────────────┐
│                   SPIDEY TRACKER                         │
├────────┬─────────────────┬───────────────────────────────┤
│ Mode # │ Name            │ Description                   │
├────────┼─────────────────┼───────────────────────────────┤
│   01   │ MOTION RADAR    │ 6-DOF attitude web radar with │
│        │                 │ dynamic tilt vector & readout │
│   02   │ CRASH SENSOR    │ High-G strain gauge (15G      │
│        │                 │ trigger) & 4-event impact log │
│   03   │ SPIDER-SENSE    │ Dynamic mask eye iris threat  │
│        │                 │ perception engine with alerts │
│   04   │ MARVEL CINEMA   │ Official MCU countdowns &     │
│        │                 │ elapsed days since release    │
│   05   │ WEB TARGETING   │ Tilt-aimed laser shooter game │
│        │                 │ with combo multiplier & score │
│   06   │ S.H.I.E.L.D.    │ Live telemetry, IMU chip ID,  │
│        │ SYSTEM STATS    │ WiFi status & RAM diagnostics │
└────────┴─────────────────┴───────────────────────────────┘
```

### 1. 🎯 Motion Radar
- Concentric octagonal spider-web radar with 8 azimuth navigation guides.
- Real-time tilt vector line radiating from origin to target cursor.
- Full digital attitude telemetry (`aX`, `aY`, `aZ`, total `G-force`) and live magnitude bar.
- LED feedback: **Blue LED** indicates X-axis tilt; **Green LED** indicates Y-axis tilt.

### 2. 💥 Crash & Impact Sensor
- High-G web strain tension meter with 15G critical impact marker.
- Real-time acceleration magnitude and peak hold memory.
- Impact history log recording peak force and timestamp.
- Full-screen tactical red strobe alert overlay + rapid **Red LED** flashing on heavy impact.

### 3. 🕷️ Spider-Sense Threat Perception
- Center tactical mask HUD featuring dynamic iris aperture shutter blades.
- Senses subtle vibration and sudden acceleration deltas.
- On trigger: Eyes dilate wide with `! THREAT ALERT !` banner and alternating LED strobe patterns.
- Adjustable sensitivity slider via UP/DOWN navigation buttons.

### 4. 🎬 Marvel Cinema Tracker (NTP-Synced & Dual-Mode)
- Exact release dates synced with real-world atomic clocks over WiFi:
  - **Spider-Man: Brand New Day** (24 Jul 2026) — *Upcoming Countdown*
  - **Avengers: Doomsday** (18 Dec 2026) — *Upcoming Countdown*
  - **Avengers: Secret Wars** (17 Dec 2027) — *Upcoming Countdown*
  - **Spider-Man: Beyond the Spider-Verse** (25 Jun 2027) — *Upcoming Countdown*
  - **The Fantastic Four: First Steps** (25 Jul 2025) — *Elapsed Days Since Release (`+DAYS AGO`)*
  - **Spider-Man: No Way Home** (17 Dec 2021) — *Elapsed Days Since Release (`+DAYS AGO`)*

### 5. 🎮 Web Targeting Mini-Game
- Full portrait tactical arena with dynamic spider drone targets.
- Tilt the device to steer the laser crosshair reticle.
- Press **SELECT** to fire web tracers from the bottom launcher.
- Hit detection with particle explosion rings, accuracy tracking, and combo multipliers.

### 6. 📊 S.H.I.E.L.D. System Diagnostics
- Real-time 3-axis accelerometer and 3-axis gyroscope readouts.
- Automatic IMU chip identification (MPU6050 vs. MPU6500) via `WHO_AM_I` register.
- WiFi status & NTP lock telemetry, CPU temperature, system uptime, and free heap RAM.

---

## 🛠️ Hardware Pinout & Wiring

| Component | Pin Function | ESP32 Pin | GPIO | Pin Type | Notes |
|:---|:---|:---|:---|:---|:---|
| **ST7735 1.8" TFT** | VCC | 3.3V | — | Power | Display power |
| | GND | GND | — | Ground | Display ground |
| | LED / BLK | 3.3V | — | Power | Backlight power |
| | CS | D5 | GPIO 5 | SPI CS | Chip Select |
| | RESET | D4 | GPIO 4 | Digital OUT | Hardware Reset |
| | A0 / DC | D2 | GPIO 2 | Digital OUT | Data / Command Selection |
| | SDA / MOSI | D23 | GPIO 23 | Hardware SPI | VSPI MOSI Data line |
| | SCK / SCL | D18 | GPIO 18 | Hardware SPI | VSPI Clock line |
| **MPU6050 / MPU6500 IMU** | VCC | VIN / 5V | — | Power | Power (VIN to onboard 3.3V reg) |
| | GND | GND | — | Ground | Sensor ground |
| | SDA | D21 | GPIO 21 | I2C Data | Default I2C SDA (100 kHz) |
| | SCL | D22 | GPIO 22 | I2C Clock | Default I2C SCL (100 kHz) |
| | INT | D15 | GPIO 15 | Digital IN | Hardware Interrupt |
| **Status LEDs** | Blue (X-Tilt) | D12 | GPIO 12 | PWM OUT | Active High |
| | Green (Y-Tilt) | D14 | GPIO 14 | PWM OUT | Active High (PWM Dimmed) |
| | Red (Alert/Crash) | D27 | GPIO 27 | PWM OUT | Active High |
| **Navigation Switches** | UP Button | D25 | GPIO 25 | Digital IN | Active Low (`INPUT_PULLUP`) |
| | SELECT Button | D26 | GPIO 26 | Digital IN | Active Low (`INPUT_PULLUP`) |
| | DOWN Button | D33 | GPIO 33 | Digital IN | Active Low (`INPUT_PULLUP`) |

---

## 🕹️ Controls Guide

| Button | Action | Context |
|---|---|---|
| **UP** (GPIO 25) | Navigate Up / Adjust Setting | Menus, Movie Cards, Sensitivity |
| **DOWN** (GPIO 33) | Navigate Down / Adjust Setting | Menus, Movie Cards, Sensitivity |
| **SELECT** (GPIO 26) | Activate / Shoot Web / Reset Peak | Menu Selection, Web Shooter |
| **HOLD SELECT (1.2s)** | Return to Main Menu | Anywhere inside any active mode |

---

## 🚀 Getting Started

### Prerequisites
- [PlatformIO Core (CLI)](https://platformio.org/install/cli) or [VS Code PlatformIO IDE](https://platformio.org/platformio-ide)
- ESP32 DevKit V1 board
- ST7735 1.8" SPI TFT Screen (128×160)
- MPU6050 / MPU6500 IMU module

### 1. Build & Upload
```bash
# Compile firmware
pio run

# Flash to connected ESP32
pio run -t upload

# Open Serial Monitor
pio device monitor -b 115200
```

---

## 📜 License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.
