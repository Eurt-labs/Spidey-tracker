# 🕷️ Spidey Tracker — Stark-Tech Tactical OS

[![Platform](https://img.shields.io/badge/Platform-ESP32-red.svg)](https://www.espressif.com/en/products/socs/esp32)
[![Framework](https://img.shields.io/badge/Framework-Arduino-blue.svg)](https://www.arduino.cc/)
[![IDE](https://img.shields.io/badge/Build-PlatformIO-orange.svg)](https://platformio.org/)
[![Display](https://img.shields.io/badge/Display-ST7735%201.8%22%20TFT-yellow.svg)](https://www.adafruit.com/)
[![IMU](https://img.shields.io/badge/Sensor-MPU6050%20%2F%206500-green.svg)](https://invensense.tdk.com/)
[![License](https://img.shields.io/badge/License-MIT-blueviolet.svg)](LICENSE)

An advanced, mature **Spider-Man / Stark-Tech Tactical HUD OS & Multi-Tracker** built for the **ESP32 DevKit V1**, featuring a **128×160 Portrait ST7735 TFT**, **MPU6050 6-DOF IMU**, **PWM status LEDs**, and a 3-button tactical navigation system.

---

## 🌟 Key Highlights

- **🎬 4-Phase Cinematic Boot Sequence**:
  1. *Quantum Neural Web & Cyber Grid* with live telemetry logs.
  2. *Tactical Spider Crest Laser Scan* with real-time frequency equalizer animation.
  3. *Tactical Mask HUD* with dynamic iris aperture calibration, focus lock reticles, and LED strobe synchronization.
  4. *Holographic Matrix Warp* transition into the tactical main menu.
- **📱 6 Interactive Tactical Modes** in high-contrast Spider-Man HUD aesthetics (Neon Red, Cyan, Gold, Crimson).
- **🕹️ 128×160 Vertical / Portrait Orientation**: Optimized layouts, typography, and vector graphics designed specifically for 1.8" TFT screens.
- **⚡ Ultra-Efficient Hardware Rendering**: Direct register-level MPU6050 communication, debounced button handling with long-press detection, and PWM-dimmed status LEDs.

---

## 🎮 Features & Modes

```
┌──────────────────────────────────────────────────────────┐
│                   SPIDEY TRACKER HUD                     │
├────────┬─────────────────┬───────────────────────────────┤
│ Mode # │ Name            │ Description                   │
├────────┼─────────────────┼───────────────────────────────┤
│   01   │ MOTION RADAR    │ 6-DOF attitude web radar with │
│        │                 │ dynamic tilt vector & readout │
│   02   │ CRASH SENSOR    │ High-G strain gauge (15G      │
│        │                 │ trigger) & 4-event impact log │
│   03   │ SPIDER-SENSE    │ Dynamic mask eye iris threat  │
│        │                 │ perception engine with alerts │
│   04   │ MARVEL CINEMA   │ 5 movie release countdowns    │
│        │                 │ with live ticking clocks      │
│   05   │ WEB TARGETING   │ Tilt-aimed laser shooter game │
│        │                 │ with combo multiplier & score │
│   06   │ S.H.I.E.L.D.    │ Live telemetry, IMU chip ID,  │
│        │ SYSTEM STATS    │ CPU temp & RAM diagnostics    │
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

### 4. 🎬 Marvel Cinema Tracker
- Portrait movie cards for upcoming Marvel releases:
  - **Spider-Man: Brand New Day** (Neon Red & Blue)
  - **Avengers: Secret Wars** (Gold & Crimson)
  - **Fantastic Four: First Steps** (Cyan & White)
  - **Blade: The Daywalker** (Alert Red & Silver)
  - **X-Men: Mutant Genesis** (Gold & Navy)
- Large prominent `DAYS` countdown and live ticking `HH : MM : SS` digital clock.

### 5. 🎮 Web Targeting Mini-Game
- Full portrait tactical arena with dynamic spider drone targets.
- Tilt the device to steer the laser crosshair reticle.
- Press **SELECT** to fire web tracers from the bottom launcher.
- Hit detection with particle explosion rings, accuracy tracking, and combo multipliers.

### 6. 📊 S.H.I.E.L.D. System Diagnostics
- Real-time 3-axis accelerometer and 3-axis gyroscope readouts.
- Automatic IMU chip identification (MPU6050 vs. MPU6500) via `WHO_AM_I` register.
- CPU temperature monitoring, system uptime, and free heap RAM telemetry.

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

## 📁 Project Structure

```
Spidey-tracker/
├── HARDWARE_PINOUT.md        # Hardware reference and pin specifications
├── platformio.ini            # PlatformIO configuration & library dependencies
├── README.md                 # Project documentation
├── .gitignore                # Git ignore rules
└── src/
    ├── config.h              # Pin mappings, RGB565 HUD colors, data structs
    ├── spider_bitmaps.h      # PROGMEM Iron Spider crest & HUD icons
    ├── boot_animation.h/.cpp # 4-Phase Stark-Tech cinematic boot animation
    ├── menu_system.h/.cpp    # Portrait scrollable HUD menu with neon brackets
    ├── motion_tracker.h/.cpp # 6-DOF spider-web radar with attitude telemetry
    ├── crash_detector.h/.cpp # Web strain gauge, impact logger & warning strobe
    ├── spider_sense.h/.cpp   # Iris aperture mask HUD & threat perception
    ├── marvel_tracker.h/.cpp # Marvel movie countdown cards & ticking clocks
    ├── web_shooter_game.h/.cpp # Tilt-aimed laser shooter game with combos
    ├── stats_screen.h/.cpp   # S.H.I.E.L.D. system diagnostics & telemetry
    └── main.cpp              # Hardware init, state machine, sensor polling
```

---

## 🚀 Getting Started

### Prerequisites
- [PlatformIO Core (CLI)](https://platformio.org/install/cli) or [VS Code PlatformIO IDE](https://platformio.org/platformio-ide)
- ESP32 DevKit V1 board
- ST7735 1.8" SPI TFT Screen (128×160)
- MPU6050 / MPU6500 IMU module

### 1. Clone the Repository
```bash
git clone https://github.com/your-username/Spidey-tracker.git
cd Spidey-tracker
```

### 2. Build the Firmware
```bash
pio run
```

### 3. Flash to ESP32
Connect your ESP32 via USB and run:
```bash
pio run -t upload
```

### 4. Open Serial Monitor
```bash
pio device monitor -b 115200
```

---

## 📜 License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.
