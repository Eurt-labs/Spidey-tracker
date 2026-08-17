<p align="center">
  <img src="assets/spidey_banner.svg" width="100%" alt="Spidey Tracker Banner"/>
</p>

<p align="center">
  <a href="https://www.espressif.com/en/products/socs/esp32"><img src="https://img.shields.io/badge/ESP32-DevKit%20V1-red?style=for-the-badge&logo=espressif&logoColor=white" alt="ESP32"/></a>
  <a href="https://platformio.org/"><img src="https://img.shields.io/badge/PlatformIO-Firmware-orange?style=for-the-badge&logo=platformio&logoColor=white" alt="PlatformIO"/></a>
  <a href="https://www.arduino.cc/"><img src="https://img.shields.io/badge/Framework-Arduino-blue?style=for-the-badge&logo=arduino&logoColor=white" alt="Arduino"/></a>
  <a href="https://github.com/Eurt-labs/Spidey-tracker/blob/main/LICENSE"><img src="https://img.shields.io/badge/License-MIT-gold?style=for-the-badge" alt="MIT License"/></a>
</p>

---

## 🕸️ Hey Everyone! Welcome to Spidey Tracker

Ever watched Peter Parker stick a mini electronic **Spider-Tracer** on a villain's getaway car and track them across the NYC skyline on a handheld radar? 

I wanted to bring that exact gadget to life on actual hardware.

**Spidey Tracker** is a custom embedded OS and multi-sensor gadget built for the **ESP32**, a **1.8" ST7735 color TFT display**, and an **MPU6050 6-DOF IMU**. It comes packed with a cinematic boot animation straight out of Tony Stark's lab, 7 interactive tactical modes, real-time WiFi atomic clock synchronization, and a whole lot of friendly neighborhood attitude.

---

## ✨ What Can This Thing Do?

```
┌─────────────────────────────────────────────────────────────┐
│                    SPIDEY TRACKER HUD                       │
├────────┬───────────────────┬────────────────────────────────┤
│ Mode # │ Tactical Mode     │ What It Does                   │
├────────┼───────────────────┼────────────────────────────────┤
│   01   │ 🕷️ SPIDER-TRACER  │ Movie gadget locator & radar   │
│   02   │ 🎯 MOTION RADAR   │ 6-DOF attitude & tilt vectors  │
│   03   │ 💥 CRASH SENSOR   │ High-G impact tension meter    │
│   04   │ ⚡ SPIDER-SENSE   │ Threat perception & mask iris  │
│   05   │ 🎬 MARVEL CINEMA  │ NTP-synced movie countdowns    │
│   06   │ 🎯 WEB TARGETING  │ Tilt-aimed laser shooter game  │
│   07   │ 📊 SYSTEM STATS   │ S.H.I.E.L.D. diagnostics       │
└────────┴───────────────────┴────────────────────────────────┘
```

### 1. 🕷️ Movie Spider-Tracer Simulator
The flagship gadget. It simulates Peter Parker's iconic handheld locator:
- **5 Trackable Targets**: Lock onto *Spider-Man (Suit Beacon)*, *Vulture (Flight Pack)*, *Green Goblin (Glider)*, *Doc Ock (Neural Arm)*, or *Electro (Bio-Volt)*.
- **Directional Steering**: Rotate and turn your ESP32 in your hand! As your device aligns with the target's bearing, the signal climbs to **99%**, the red LED pulses in rhythm, and a `[TARGET LOCK]` reticle snaps onto the blip.
- **Interactive Sonar Ping (`SELECT`)**: Press the button to fire an expanding radar sonar wave across the screen and update target distance in real time.

### 2. 🎯 6-DOF Motion Radar
- Concentric spider-web radar with 8 azimuth guides that tracks real-world device pitch, roll, and tilt.
- Draws live vector lines and attitude reticles with real-time numeric telemetry (`aX`, `aY`, `aZ`, total `G-force`).
- Status LEDs respond with smooth PWM dimming as you tilt.

### 3. 💥 Impact & Crash Detector
- High-G web-tension strain meter with a **15G critical impact threshold**.
- Keeps a peak-hold record and logs the last 4 impacts with exact timestamps.
- Features a full-screen tactical red strobe alert if you drop or shake the device hard enough!

### 4. ⚡ Spider-Sense Threat Perception Engine
- A center-stage Spider-Man mask HUD with animated titanium lenses.
- Senses subtle ambient vibrations and sudden acceleration spikes.
- When danger is detected, the mask eyes dilate wide, the screen flashes `! THREAT ALERT !`, and the LEDs strobe in alternating warning pulses.

### 5. 🎬 Marvel Cinema Tracker (WiFi + NTP Synced)
- Hooks up to your local WiFi (`Airtel_Dhruv`) and syncs with global atomic clocks via NTP.
- **Upcoming Movies**: Real-time ticking countdown (`DAYS LEFT` + `HH:MM:SS`) to premiere:
  - *Spider-Man: Brand New Day* (24 Jul 2026)
  - *Avengers: Doomsday* (18 Dec 2026)
  - *Avengers: Secret Wars* (17 Dec 2027)
  - *Spider-Man: Beyond the Spider-Verse* (25 Jun 2027)
- **Released Milestones**: Counts elapsed time (`+DAYS AGO` + `+HH:MM:SS`) since premiere:
  - *The Fantastic Four: First Steps* (25 Jul 2025)
  - *Spider-Man: No Way Home* (17 Dec 2021)

### 6. 🎮 Web Targeting Mini-Game
- Tilt the ESP32 to steer a high-tech laser crosshair across a vertical playfield.
- Tap **SELECT** to fire web tracers from the bottom launcher.
- Includes moving drone targets, particle explosion rings, accuracy tracking, and score combo multipliers!

### 7. 📊 S.H.I.E.L.D. Diagnostics
- Live telemetry monitor showing real-time 3-axis accelerometer and gyroscope waveforms.
- Automatic IMU chip detection (MPU6050 vs. MPU6500) via the `WHO_AM_I` register, CPU core temperature, system uptime, and heap memory usage.

---

## 🛠️ The Hardware Setup

Here's the full wiring blueprint to build your own Spidey Tracker:

| Component | Pin Function | ESP32 Pin | GPIO | Pin Type | Notes |
|:---|:---|:---|:---|:---|:---|
| **ST7735 1.8" TFT** | VCC | 3.3V | — | Power | 3.3V power |
| | GND | GND | — | Ground | Common ground |
| | LED / BLK | 3.3V | — | Power | Backlight power |
| | CS | D5 | GPIO 5 | SPI CS | Chip Select |
| | RESET | D4 | GPIO 4 | Digital OUT | Hardware Reset |
| | A0 / DC | D2 | GPIO 2 | Digital OUT | Data / Command |
| | SDA / MOSI | D23 | GPIO 23 | Hardware SPI | VSPI MOSI line |
| | SCK / SCL | D18 | GPIO 18 | Hardware SPI | VSPI Clock line |
| **MPU6050 / 6500 IMU** | VCC | VIN / 5V | — | Power | Onboard 3.3V regulator |
| | GND | GND | — | Ground | Common ground |
| | SDA | D21 | GPIO 21 | I2C Data | Hardware I2C (100 kHz) |
| | SCL | D22 | GPIO 22 | I2C Clock | Hardware I2C (100 kHz) |
| | INT | D15 | GPIO 15 | Digital IN | Hardware Interrupt |
| **Status LEDs** | Blue (X-Tilt) | D12 | GPIO 12 | PWM OUT | Active High |
| | Green (Y-Tilt) | D14 | GPIO 14 | PWM OUT | Active High (PWM Dimmed) |
| | Red (Alert/Crash) | D27 | GPIO 27 | PWM OUT | Active High |
| **Tactile Buttons** | UP Button | D25 | GPIO 25 | Digital IN | Active Low (`INPUT_PULLUP`) |
| | SELECT Button | D26 | GPIO 26 | Digital IN | Active Low (`INPUT_PULLUP`) |
| | DOWN Button | D33 | GPIO 33 | Digital IN | Active Low (`INPUT_PULLUP`) |

---

## 🕹️ How to Control It

| Button | What It Does |
|---|---|
| **▲ UP** (GPIO 25) | Scroll up through menus / switch movie cards / adjust sensitivity |
| **▼ DOWN** (GPIO 33) | Scroll down through menus / switch movie cards / adjust sensitivity |
| **● SELECT** (GPIO 26) | Open mode / Fire web tracer / Trigger sonar ping / Reset peak G |
| **● HOLD SELECT (1.2s)** | Quick escape hatch — takes you straight back to the main menu from any mode |

---

## 🚀 Building & Flashing

Getting this running on your ESP32 takes less than 2 minutes using PlatformIO:

### 1. Clone the Repo
```bash
git clone https://github.com/Eurt-labs/Spidey-tracker.git
cd Spidey-tracker
```

### 2. Build the Code
```bash
pio run
```

### 3. Flash to Your ESP32
Plug your board in via USB and run:
```bash
pio run -t upload
```

### 4. Open Serial Monitor (Optional)
To watch boot logs and WiFi NTP synchronization:
```bash
pio device monitor -b 115200
```

---

## 📁 Repository Layout

```text
Spidey-tracker/
├── assets/
│   └── spidey_banner.svg     # Animated Spider-Man vector banner
├── HARDWARE_PINOUT.md        # Hardware reference specs
├── platformio.ini            # PlatformIO config & dependencies
├── README.md                 # You are here!
├── .gitignore                # Build & IDE ignores
└── src/
    ├── config.h              # Pinouts, RGB565 HUD colors & WiFi credentials
    ├── spider_bitmaps.h      # PROGMEM Iron Spider insignia & HUD icons
    ├── boot_animation.h/.cpp # 4-Phase Stark-Tech cinematic boot animation
    ├── menu_system.h/.cpp    # Portrait scrollable HUD menu with neon brackets
    ├── spidey_tracer.h/.cpp  # Movie Spider-Tracer gadget simulator
    ├── motion_tracker.h/.cpp # 6-DOF spider-web radar with attitude telemetry
    ├── crash_detector.h/.cpp # Web strain gauge, impact logger & warning strobe
    ├── spider_sense.h/.cpp   # Iris aperture mask HUD & threat perception
    ├── marvel_tracker.h/.cpp # Marvel movie countdown cards & ticking clocks
    ├── web_shooter_game.h/.cpp # Tilt-aimed laser shooter game with combos
    ├── stats_screen.h/.cpp   # S.H.I.E.L.D. system diagnostics & telemetry
    └── main.cpp              # Hardware init, state machine & WiFi NTP loop
```

---

## 📜 License & Credits

Built with ❤️ by **Dhruv Saraswat ([@Eurt-labs](https://github.com/Eurt-labs))**.

This project is licensed under the [MIT License](LICENSE). Feel free to build your own, customize the suit OS, and swing across the web!

> *"With great power comes great response time."* 🕷️⚡
