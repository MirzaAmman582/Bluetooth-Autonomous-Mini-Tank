# Bluetooth-Operated Mini Tank: Autonomous & Manual Control

## 📌 Project Overview
A multi-modal robotic platform capable of real-time manual operation via Bluetooth and fully autonomous navigation. The system integrates sensor fusion (Ultrasonic & Photoresistors) with a differential drive locomotion system to navigate complex environments.

### 🤖 Core Robotics Competencies
* **Autonomous Navigation:** Implemented logic for **Obstacle Avoidance** (20cm threshold), **Object Following** (10-20cm range), and **Light Tracking**.
* **Embedded Systems:** Developed C++/Arduino firmware for the **ATmega328P (Uno R3)** to handle asynchronous sensor polling and motor PWM.
* **Wireless Communication:** Real-time command processing via **HC-06 Bluetooth module** with sub-millisecond latency.
* **Human-Machine Interface (HMI):** Integrated an **I2C 16x8 LED Matrix** to provide visual status icons and directional feedback.

## 🛠 Hardware Stack
* **Microcontroller:** Arduino Uno R3 + Sensor Shield
* **Locomotion:** Dual DC Motors with L298N/L293D Driver
* **Sensors:** HC-SR04 Ultrasonic (on Servo), Photoresistors (LDRs)
* **Comms:** HC-06 Bluetooth Module
* **Power:** Dual 3.7V 3300mAh Li-ion batteries

## 📁 Repository Structure
* `/Firmware`: `RC_Tank.ino` - The complete source code for all autonomous and manual modes.
* `/Docs`: `Mini_Tank_Technical_Report.pdf` - Detailed analysis of locomotion and sensor integration.
* `README.md`: Project summary and hardware specifications.

---
*Note: This project was completed as part of postgraduate technical studies.*
