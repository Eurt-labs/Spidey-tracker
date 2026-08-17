# ESP32 DevKit V1 - Hardware Pin Layout & System Specifications

Complete hardware connection guide and pin mappings for the ESP32 Dev Kit V1 project.

---

## 1. Microcontroller Board
* **Board**: ESP32 DevKit V1 (30-Pin / 38-Pin variant)
* **Framework**: Arduino
* **Default Bus Standards**: SPI (VSPI), I2C (Wire)

---

## 2. Pin Mapping Table

| Component | Pin Function | ESP32 Pin | GPIO | Pin Type | Notes |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **ST7735 TFT Screen** | VCC | 3.3V | — | Power | Display power |
| | GND | GND | — | Ground | Display ground |
| | LED / BLK | 3.3V | — | Power | Backlight power |
| | CS | D5 | GPIO 5 | SPI CS | Chip Select |
| | RESET | D4 | GPIO 4 | Digital OUT | Hardware Reset |
| | A0 / DC | D2 | GPIO 2 | Digital OUT | Data / Command Selection |
| | SDA / MOSI | D23 | GPIO 23 | Hardware SPI | VSPI MOSI Data line |
| | SCK / SCL | D18 | GPIO 18 | Hardware SPI | VSPI Clock line |
| **MPU6050 / MPU6500 IMU** | VCC | VIN / 5V | — | Power | Recommended (Feeds onboard 3.3V regulator) |
| | GND | GND | — | Ground | Sensor ground |
| | SDA | D21 | GPIO 21 | I2C Data | Default I2C SDA (100 kHz) |
| | SCL | D22 | GPIO 22 | I2C Clock | Default I2C SCL (100 kHz) |
| | INT | D15 | GPIO 15 | Digital IN | Hardware Interrupt Output |
| **Status LEDs** | X-Axis Tilt | D12 | GPIO 12 | PWM / Digital OUT | **Blue LED** (Active High) |
| | Y-Axis Tilt | D14 | GPIO 14 | PWM / Digital OUT | **Green LED** (Active High, PWM Dimmed) |
| | Crash / Alert | D27 | GPIO 27 | PWM / Digital OUT | **Red LED** (Active High) |
| **Navigation Switches** | UP Switch | D25 | GPIO 25 | Digital IN | Active Low (`INPUT_PULLUP`) |
| | SELECT Switch | D26 | GPIO 26 | Digital IN | Active Low (`INPUT_PULLUP`) |
| | DOWN Switch | D33 | GPIO 33 | Digital IN | Active Low (`INPUT_PULLUP`) |

---

## 3. Detailed Component Specifications

### A. ST7735 1.8" TFT Screen (SPI Interface)
- **Controller**: ST7735 / ST7735R (Init tab: `INITR_BLACKTAB`)
- **Resolution**: 128 x 160 pixels
- **Orientation**: Landscape (`setRotation(1)`)
- **SPI Bus**: ESP32 VSPI (`CS=5, DC=2, RST=4, MOSI=23, SCK=18`)

### B. MPU6050 / MPU6500 6-DOF IMU Sensor (I2C + Interrupt)
- **Interface**: I2C (`SDA=21, SCL=22`)
- **Hardware Interrupt**: Pin **D15 (GPIO 15)** connected to MPU `INT` pin for data-ready or motion interrupts.
- **I2C Addresses**: 
  - `0x68`: AD0 pulled LOW or floating
  - `0x69`: AD0 pulled HIGH (3.3V)
- **Register WHO_AM_I**:
  - `0x68` $\rightarrow$ MPU6050
  - `0x70` $\rightarrow$ MPU6500

### C. Status LEDs (Active High with PWM Dimming)
- **Blue LED (D12)**: Lights up when X-axis tilt exceeds threshold ($|a_x| > 3.0 \text{ m/s}^2$).
- **Green LED (D14)**: Lights up when Y-axis tilt exceeds threshold ($|a_y| > 3.0 \text{ m/s}^2$). Output is PWM dimmed (duty cycle ~15-20%) to prevent excessive glare.
- **Red LED (D27)**: Lights up when total acceleration magnitude exceeds crash threshold ($> 15.0 \text{ m/s}^2$).

### D. Navigation Switches (Active Low)
- Internal pull-up resistors enabled (`INPUT_PULLUP`).
- Connected between the respective ESP32 GPIO and `GND`.
- **State**: LOW (`0`) when pressed, HIGH (`1`) when idle.
