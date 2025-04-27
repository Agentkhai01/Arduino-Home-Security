#Arduino Nano Password Unlock System
##with PIR Sensor, 4x4 Keypad, and 16x2 LCD Display)

Overview
This project implements a secure password-based unlock system using an Arduino Nano, featuring motion detection, keypad password entry, and visual feedback via a 16x2 LCD screen.
When the PIR sensor detects movement, the system prompts for a password. Upon correct entry, a locking mechanism is activated (e.g., a servo motor unlocks).
The system is designed for applications such as low-cost security systems, smart lockers, or restricted access setups.

System Components
Arduino Nano — Microcontroller for processing input and controlling outputs.

PIR Sensor — Detects motion to activate the password entry system.

4x4 Matrix Keypad — User input device for entering the password.

16x2 LCD Display — Provides user feedback (status messages).

Servo Motor (or Relay + Lock) — Acts as the locking/unlocking mechanism.

Breadboard and Jumper Wires — For circuit prototyping.

(Optional) Buzzer / LEDs — For additional alerts.

Features
Motion-activated system: triggers only when movement is detected.

Password-protected unlocking mechanism.

Password input masking using "*" characters.

Visual status feedback via LCD.

Auto-reset after successful or failed password attempt.

Compact design using Arduino Nano.

Easily customizable password.

Circuit Design
Key Connections:


Component	Arduino Pin
PIR Sensor Output	D12
LCD RS	D7
LCD Enable	D6
LCD D4	D5
LCD D5	D4
LCD D6	D3
LCD D7	D2
Keypad Rows	A0, A1, A2, A3
Keypad Columns	A4, A5, D9, D8
Servo Signal Pin	D10
(Modify pins as per your setup if needed.)

Required Libraries
Make sure the following libraries are installed in the Arduino IDE:

Keypad.h — For handling keypad inputs.

LiquidCrystal.h — For driving the LCD display.

Servo.h — For controlling the servo motor.

Working Principle
Idle Mode: System waits for motion detection.

Motion Detected: LCD prompts user to enter password.

Password Entry: User inputs password via keypad.

Verification:

Correct Password ➔ Servo unlocks for a few seconds.

Incorrect Password ➔ Error message is displayed.

Reset: After action completion or timeout, the system returns to Idle Mode.
