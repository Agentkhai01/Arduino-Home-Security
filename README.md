# Arduino Nano Password Unlock System
## With PIR Sensor, 4x4 Keypad, and 16x2 LCD Display

---

## Overview
This project implements a secure password-based unlocking system using an Arduino Nano, featuring motion detection via a PIR sensor, password input through a 4x4 matrix keypad, and user feedback via a 16x2 LCD display.  
When motion is detected, the system prompts the user to enter a password. If the correct password is entered, an unlocking mechanism is activated (e.g., a servo motor or electronic lock).

This project can be used for:
- Low-cost security systems
- Smart lockers
- Restricted access setups

---

## Components Used
- **Arduino Nano**
- **PIR Motion Sensor**
- **4x4 Matrix Keypad**
- **16x2 LCD Display**
- **Servo Motor** (or Relay + Lock system)
- **Breadboard and Jumper Wires**
- **Optional**: Buzzer and LEDs for additional alerts

---

## Features
- Motion-activated password entry system
- Password input masking with `*` characters
- LCD-based user feedback
- Auto-reset after successful or failed attempt
- Compact and low-power design
- Easily customizable password

---

## Circuit Connections

| Component            | Arduino Pin  |
| --------------------- | ------------- |
| PIR Sensor Output     | D12           |
| LCD RS                | D7            |
| LCD Enable            | D6            |
| LCD D4                | D5            |
| LCD D5                | D4            |
| LCD D6                | D3            |
| LCD D7                | D2            |
| Keypad Row Pins       | A0, A1, A2, A3 |
| Keypad Column Pins    | A4, A5, D9, D8 |
| Servo Motor Signal    | D10           |

> **Note:** You can adjust pins as needed based on your wiring.

---

## Required Libraries
Make sure the following libraries are installed in the Arduino IDE:
- `Keypad.h`
- `LiquidCrystal.h`
- `Servo.h`

They can be installed via the Library Manager.

---

## Working Principle
1. **Idle Mode**: System remains idle until motion is detected.
2. **Motion Detected**: LCD prompts for password entry.
3. **Password Input**: User enters the password through the keypad.
4. **Verification**:
    - If correct ➔ Unlocks the system (e.g., moves servo motor).
    - If incorrect ➔ Displays "Wrong Password" and resets.
5. **Auto Reset**: After action completion, system returns to Idle Mode.

---

## Potential Improvements
- **EEPROM Storage**: Save password changes permanently.
- **Lockout System**: Temporarily disable input after multiple wrong attempts.
- **Buzzer Alarm**: Add buzzer alerts for unauthorized access.
- **Battery Backup**: Integrate a battery or UPS for continuous operation.
- **Wi-Fi Connectivity**: Upgrade to ESP32/ESP8266 for remote monitoring.

---

## License
This project is open-source and free to use under the MIT License.

---
