#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>


const int wrongLEDPin = 13;
const int correctLEDPin = A0;
const int backlightControlPin = A6;
const int servoPin = 5;
const int buzzerPin = A5;
const int pirPin = 2;

// === LCD Setup (RS, E, D4, D5, D6, D7) ===
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// === Keypad Setup ===
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {3, 4, A4, A2};
byte colPins[COLS] = {6, A1, A3, A7};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// === State Variables ===
Servo lockServo;
bool inInteraction = false;
bool accessGranted = false;
bool isActive = false;
bool lockedOut = false;
String inputPassword = "";
const String correctPassword = "2468";
int attemptsLeft = 3;
const int maxAttempts = 3;

unsigned long lastMotionTime = 0;
const unsigned long idleTimeout = 1000;

int hashCount = 0;
unsigned long lastHashTime = 0;
const unsigned long hashResetInterval = 5000;  

const int lockedPos = 0;
const int unlockedPos = 90;

void printWithEffect(String message, int row = 0, int delayMs = 50);

// === Setup ===
void setup() {
  pinMode(pirPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(backlightControlPin, OUTPUT);
  pinMode(wrongLEDPin, OUTPUT);
  pinMode(correctLEDPin, OUTPUT);

  digitalWrite(buzzerPin, LOW);
  digitalWrite(wrongLEDPin, LOW);
  digitalWrite(correctLEDPin, LOW);

  analogWrite(backlightControlPin, 255);
  lockServo.attach(servoPin);
  lockServo.write(lockedPos); 
  lcd.begin(16, 2);

  jarvisBootAnimation();
}

// === Main Loop ===
void loop() {
  if (digitalRead(pirPin) == HIGH && !isActive && !lockedOut) {
    wakeSystem();
    lastMotionTime = millis();
  }

  if (!isActive || lockedOut) {
    // Auto-sleep after inactivity
    if (millis() - lastMotionTime > idleTimeout) {
      sleepSystem();
    }
    return;
  }

  char key = keypad.getKey();
  if (key) {
    if (key == '#') {
      unsigned long now = millis();
      if (now - lastHashTime <= hashResetInterval) {
        hashCount++;
      } else {
        hashCount = 1;
      }
      lastHashTime = now;

      if (hashCount >= 10) {
        performAdminOverride();
      }
    } else {
      hashCount = 0;
    }

    if (key != '#' && inputPassword.length() < 4) {
      inputPassword += key;
      lcd.print("*");

      if (inputPassword.length() == 4) {
        lcd.clear();
        if (inputPassword == correctPassword) {
          jarvisSuccess();
          inputPassword = "";
          sleepSystem();
        } else {
          attemptsLeft--;
          jarvisFailure();

          if (attemptsLeft <= 0) {
            lockedOut = true;
            lcd.clear();
            printWithEffect("INTRUDER LOCKED", 0, 60);
            lcd.setCursor(0, 1);
            printWithEffect("PROTOCOL 7 ACTIVE", 1, 60);

            digitalWrite(wrongLEDPin, HIGH);
            for (int i = 0; i < 10; i++) {
              tone(buzzerPin, 1000); delay(150);
              noTone(buzzerPin); delay(100);
            }

            sleepSystem();
          } else {
            lcd.clear();
            printWithEffect("Awaiting Input...");
            lcd.setCursor(0, 1);
          }
        }
        inputPassword = "";
      }
    }
  }
}


void performAdminOverride() {
  lcd.clear();
  printWithEffect("ADMIN OVERRIDE", 0, 60);
  lcd.setCursor(0, 1);
  printWithEffect("SYSTEM RESET", 1, 60);

 
  for (int i = 0; i < 6; i++) {
    digitalWrite(wrongLEDPin, HIGH);
    digitalWrite(correctLEDPin, LOW);
    tone(buzzerPin, 800); delay(80);

    digitalWrite(wrongLEDPin, LOW);
    digitalWrite(correctLEDPin, HIGH);
    tone(buzzerPin, 1200); delay(80);

    digitalWrite(correctLEDPin, LOW);
    noTone(buzzerPin);
    delay(80);
  }

  // Final green LED + success beep
  digitalWrite(correctLEDPin, HIGH);
  tone(buzzerPin, 1500, 300); delay(300);
  noTone(buzzerPin);
  digitalWrite(correctLEDPin, LOW);

  lcd.clear();
  attemptsLeft = maxAttempts;
  lockedOut = false;
  digitalWrite(wrongLEDPin, LOW);
  printWithEffect("Awaiting Input...", 0, 50);
  lcd.setCursor(0, 1);
  hashCount = 0;
  inputPassword = "";
}


void sleepSystem() {
  isActive = false;
  lcd.clear();
  lcd.noDisplay();
  analogWrite(backlightControlPin, 10);  
  lockServo.write(lockedPos);
}

// === System Wake ===
void wakeSystem() {
  isActive = true;
  lockServo.write(lockedPos);
  analogWrite(backlightControlPin, 255); 
  lcd.display();
  lcd.clear();
  printWithEffect("IDENTITY DETECTED", 0, 60);
  delay(500);
  lcd.clear();
  printWithEffect("Greetings, Human", 0, 60);
  delay(500);
  lcd.clear();
  printWithEffect("Awaiting Input...");
  lcd.setCursor(0, 1);
}

// === Boot Animation ===
void jarvisBootAnimation() {
  lcd.clear();
  printWithEffect("INITIALIZING...", 0, 80);
  delay(600);
  lcd.clear();
  printWithEffect("System Check:", 0, 70);
  lcd.setCursor(0, 1);
  printWithEffect("✔ Motors OK", 1, 60);
  delay(400);
  lcd.clear();
  printWithEffect("✔ Display OK", 0, 60);
  delay(400);
  lcd.clear();
  printWithEffect("✔ Sensors OK", 0, 60);
  delay(400);
  lcd.clear();
  printWithEffect("JARVIS ONLINE", 0, 70);
  delay(600);
  lcd.clear();
  printWithEffect("Ready to Serve", 0, 70);
  delay(600);
  lcd.clear();
}

// === Access Granted ===
void jarvisSuccess() {
  printWithEffect("ACCESS GRANTED", 0, 70);
  lcd.setCursor(0, 1);
  printWithEffect("WELCOME, SIR", 1, 70);

  // Quick success beep sequence
  tone(buzzerPin, 1000, 150); delay(150);
  tone(buzzerPin, 1500, 150); delay(150);
  tone(buzzerPin, 2000, 150); delay(150);

  digitalWrite(correctLEDPin, HIGH);
  lockServo.write(unlockedPos);
  delay(500);
  digitalWrite(correctLEDPin, LOW);
}


void jarvisFailure() {
  printWithEffect("ACCESS DENIED", 0, 70);
  lcd.setCursor(0, 1);
  printWithEffect("Try again.", 1, 70);


  for (int i = 0; i < 3; i++) {
    digitalWrite(wrongLEDPin, HIGH);
    tone(buzzerPin, 300); delay(100);
    digitalWrite(wrongLEDPin, LOW);
    noTone(buzzerPin); delay(100);
  }

  digitalWrite(wrongLEDPin, LOW);
  delay(500);
  lcd.clear();
  lcd.print("Attempts left: ");
  lcd.setCursor(0, 1);
  lcd.print(attemptsLeft);
  delay(1000);
  lcd.clear();
}


void printWithEffect(String message, int row = 0, int delayMs = 50) {
  lcd.setCursor(0, row);
  for (int i = 0; i < message.length(); i++) {
    lcd.print(message[i]);
    delay(delayMs);
  }
}
