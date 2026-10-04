<div align="center">

# 🚗 Distance Measuring Bot

### *Precision distance estimation for 4WD Arduino robotics using kinematic curve fitting*

[![Arduino](https://img.shields.io/badge/Arduino-UNO-00979D?style=for-the-badge&logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![Display](https://img.shields.io/badge/OLED-SSD1306-4285F4?style=for-the-badge&logo=google&logoColor=white)](https://www.adafruit.com/)
[![Connectivity](https://img.shields.io/badge/Bluetooth-HC--05%20%2F%20HC--06-0082FC?style=for-the-badge&logo=bluetooth&logoColor=white)](final-code/final-code.ino)
[![Status](https://img.shields.io/badge/Status-Completed-34A853?style=for-the-badge)](https://github.com/infernogurala/distance-measuring-bot)

---

</div>

## 📌 Overview

**Distance Measuring Bot** is an autonomous 4-wheel robot car built on an **Arduino UNO** platform. It dynamically estimates net distance traveled without dedicated rotary encoders by using time-velocity differential kinematics with startup pickup lag ($\tau$) compensation. 

The robot provides real-time visual feedback on a 0.96" SSD1306 OLED display and streams live distance telemetry over Bluetooth.

---

## 🖼️ Visual Showcase

<div align="center">

| ⏱️ Initial State (`0 cm`) | 📏 Active Measurement |
| :---: | :---: |
| <img src="assets/image-1.jpeg" width="360" style="border-radius: 8px;" alt="Initial State 0 cm" /> | <img src="assets/image-2.jpeg" width="360" style="border-radius: 8px;" alt="Active State" /> |

<br/>

### 🎬 Live Demo Video

[![Watch Demo Video](https://img.shields.io/badge/▶️_Play_Demonstration_Video-1A73E8?style=for-the-badge&logo=youtube&logoColor=white)](assets/video.mp4)

<br/>

<a href="assets/video.mp4">
  <img src="assets/image-2.jpeg" width="480" style="border-radius: 12px; box-shadow: 0 4px 12px rgba(0,0,0,0.15);" alt="Click to Watch Demonstration Video" />
</a>

*Click the preview image or the badge above to watch the demonstration video.*

</div>

---

## ✨ Key Features

- **📐 Kinematic Distance Modeling**: Computes continuous distance using a calibrated exponential acceleration curve fitting model ($V_{\text{steady}} = 71.2\text{ cm/s}, \tau = 0.5\text{ s}$).
- **🖥️ Real-Time OLED Interface**: Displays status (`Ready`, `Forward`, `Backward`, `Stopped`) and net distance in centimeters on a 128x64 SSD1306 display.
- **📡 Wireless Telemetry**: Broadcasts live updates over Bluetooth serial every 500 ms during movement.
- **🎮 Remote Serial Control**: Responds to standard directional single-byte commands (`F`, `B`, `L`, `R`, `S`, `Y`).

---

## ⚡ Pinout Architecture

<div align="center">

| Component | Pin Function | Arduino UNO Pin |
| :--- | :--- | :---: |
| **HC-05 / HC-06 Bluetooth** | SoftwareSerial RX | `Pin 8` |
| | SoftwareSerial TX | `Pin 9` |
| **L298N Motor Driver** | ENA (Speed Control) | `Pin 3` (PWM) |
| | IN1 (Motor A Dir) | `Pin 7` |
| | IN2 (Motor A Dir) | `Pin 2` |
| | IN3 (Motor B Dir) | `Pin 4` |
| | IN4 (Motor B Dir) | `Pin 5` |
| | ENB (Speed Control) | `Pin 6` (PWM) |
| **SSD1306 OLED Display** | I2C SDA | `Pin A4` |
| | I2C SCL | `Pin A5` |

</div>

---

## 🧮 Kinematic Model

Distance is computed dynamically for any active segment of duration $t$ starting from rest using:

$$d(t) = V_{\text{cm/s}} \cdot \left( t - \tau \cdot (1 - e^{-t / \tau}) \right)$$

Where at constant PWM speed setting **150**:
- **$V_{\text{cm/s}}$ (Steady Speed)** = $71.2 \text{ cm/s}$
- **$\tau$ (Pickup Lag Constant)** = $0.5 \text{ s}$

---

## 🕹️ Command Reference

| Command | Action | System Response |
| :---: | :--- | :--- |
| `F` | Forward | Moves forward and accumulates positive distance |
| `B` | Backward | Moves backward and subtracts distance |
| `L` | Turn Left | Rotates vehicle left |
| `R` | Turn Right | Rotates vehicle right |
| `S` | Stop | Halts motors and commits current distance |
| `Y` | Reset | Halts motors and resets total distance to `0 cm` |

---

## 📁 Workspace Hierarchy

```
distance-measuring-bot/
├── 📄 README.md             # Project overview & documentation
├── 📄 .gitignore            # Git ignore pattern rules
├── 📂 assets/               # Media showcase files
│   ├── image-1.jpeg         # Initial state photo (0 cm)
│   ├── image-2.jpeg         # Active measurement photo
│   └── video.mp4            # Live robot demonstration video
├── 📂 docs/                 # Technical documentation
│   └── design_notes.md      # Mathematical models & development notes
├── 📂 final-code/           # Firmware
│   └── final-code.ino       # Main Arduino production sketch
└── 📂 tests/                # Test utilities
    └── oled_test/
        └── oled_test.ino   # Standalone OLED display test sketch
```

---

## 🛠️ Quick Start

1. **Wiring**: Connect hardware components according to the [Pinout Architecture](#-pinout-architecture).
2. **Dependencies**: Install the required libraries in Arduino IDE:
   - `Adafruit GFX Library`
   - `Adafruit SSD1306`
3. **Upload**: Open [`final-code/final-code.ino`](final-code/final-code.ino) and upload to your Arduino UNO.
4. **Control**: Pair via Bluetooth at `9600 baud` and send control commands!

---

<div align="center">

Made with ❤️ for Robotics & Embedded Systems

</div>
