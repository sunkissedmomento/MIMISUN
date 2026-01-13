#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ================= PINS =================
const int relayPin = 8;      // ACTIVE-LOW relay
const int buzzerPin = 9;

const int btn3m = 2;
const int btn5m = 3;
const int btn10m = 4;
const int btnStartStop = 5;

// ================= TIMER =================
unsigned long setTime = 0;
unsigned long startTime = 0;
bool running = false;

// ================= DEBOUNCE =================
unsigned long lastPress = 0;
const unsigned long debounceDelay = 250;

void setup() {
  pinMode(relayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  pinMode(btn3m, INPUT_PULLUP);
  pinMode(btn5m, INPUT_PULLUP);
  pinMode(btn10m, INPUT_PULLUP);
  pinMode(btnStartStop, INPUT_PULLUP);

  // Relay OFF by default (ACTIVE-LOW)
  digitalWrite(relayPin, HIGH);
  digitalWrite(buzzerPin, LOW);

  lcd.init();
  lcd.backlight();

  Serial.begin(9600);
  Serial.println("UVB TIMER - MINUTES MODE");
  Serial.println("1=3m 2=5m 3=10m s=Start/Stop x=Stop");

  lcd.setCursor(0, 0);
  lcd.print("UVB TIMER");
  lcd.setCursor(0, 1);
  lcd.print("Select Time");
}

void loop() {
  unsigned long now = millis();

  // ---------- SERIAL INPUT ----------
  if (Serial.available()) {
    char cmd = Serial.read();

    if (!running) {
      if (cmd == '1') { setTime = 180000; showTime(3); }
      if (cmd == '2') { setTime = 300000; showTime(5); }
      if (cmd == '3') { setTime = 600000; showTime(10); }
    }

    if (cmd == 's') {
      if (!running && setTime > 0) startTimer();
      else stopSystem();
    }

    if (cmd == 'x') {
      stopSystem();
      Serial.println("EMERGENCY STOP");
    }
  }

  // ---------- BUTTON INPUT ----------
  if (!running && now - lastPress > debounceDelay) {
    if (digitalRead(btn3m) == LOW) {
      setTime = 180000;
      showTime(3);
      lastPress = now;
    }
    if (digitalRead(btn5m) == LOW) {
      setTime = 300000;
      showTime(5);
      lastPress = now;
    }
    if (digitalRead(btn10m) == LOW) {
      setTime = 600000;
      showTime(10);
      lastPress = now;
    }
  }

  if (digitalRead(btnStartStop) == LOW && now - lastPress > debounceDelay) {
    lastPress = now;
    if (!running && setTime > 0) startTimer();
    else stopSystem();
  }

  // ---------- COUNTDOWN ----------
  if (running) {
    unsigned long elapsed = millis() - startTime;
    unsigned long remainingMin = (setTime - elapsed) / 60000;

    lcd.setCursor(0, 0);
    lcd.print("RUNNING        ");
    lcd.setCursor(0, 1);
    lcd.print("Left: ");
    lcd.print(remainingMin + 1); // better UX
    lcd.print(" min   ");

    if (elapsed >= setTime) {
      stopSystem();
      buzzerBeep();
    }
  }
}

// ================= FUNCTIONS =================

void startTimer() {
  running = true;
  startTime = millis();
  digitalWrite(relayPin, LOW);   // RELAY ON (ACTIVE-LOW)
  Serial.println("TIMER STARTED");
}

void stopSystem() {
  running = false;
  digitalWrite(relayPin, HIGH);  // RELAY OFF (ACTIVE-LOW)

  lcd.clear();
  lcd.print("DONE");
  lcd.setCursor(0, 1);
  lcd.print("Select Time");

  Serial.println("TIMER STOPPED");
}

void showTime(int min) {
  lcd.clear();
  lcd.print("Set Time:");
  lcd.setCursor(0, 1);
  lcd.print(min);
  lcd.print(" min");

  Serial.print("TIME SET: ");
  Serial.print(min);
  Serial.println(" min");
}

void buzzerBeep() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(200);
    digitalWrite(buzzerPin, LOW);
    delay(200);
  }
}
