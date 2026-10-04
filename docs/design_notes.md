# Design & Development Notes

This document retains the original technical notes, hardware list, and conceptual approaches evaluated during the development of the Distance Measuring Bot.

---

## 💡 Project Concept

The objective of the project is to build a typical 4-wheel robot car capable of measuring distance traveled in real-time, either by motor runtime modeling or wheel revolution counts.

---

## 📦 Bill of Materials (BOM)

- **Arduino UNO** (Main microcontroller)
- **L298N Dual H-Bridge Motor Driver**
- **4WD Robot Car Chassis with 4 DC Motors**
- **0.96-inch SSD1306 OLED Display (128x64, I2C)**
- **HC-05 / HC-06 Bluetooth Transceiver Module**

---

## 🔬 Mathematical Approaches Evaluated

### 1. Wheel Revolution / Degree Rotation Method
Calculates exact distance based on wheel circumference ($z$) and total wheel revolutions ($x$):

$$\text{Circumference } (z) = \pi \times D$$

- $1 \text{ revolution} = z \text{ cm}$ (e.g., $20\text{ cm}$)
- $x \text{ revolutions} \implies z \times x = \text{Total Distance Covered}$

$$\boxed{\text{Total Distance} = z \times x}$$

*Pros*: High precision per pulse with optical rotary encoders.  
*Cons*: Requires dedicated physical encoder hardware and digital interrupt pins.

---

### 2. Time-Kinematics & Acceleration Curve Fitting (Selected Implementation)
Estimates distance via software timing at constant PWM voltage by fitting an exponential velocity acceleration curve to compensate for motor pickup lag ($\tau$):

$$d(t) = V_{\text{steady}} \cdot \left( t - \tau \cdot (1 - e^{-t / \tau}) \right)$$

- Fitted parameters at PWM Speed 150:
  - $V_{\text{steady}} = 71.2 \text{ cm/s}$
  - $\tau = 0.5 \text{ s}$

*Pros*: No extra hardware sensors required; accurate within $\sim 3\text{--}5\text{ cm}$ on consistent surfaces.
