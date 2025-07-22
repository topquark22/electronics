#include "LiquidCrystal_I2C.h"

const PROGMEM uint8_t PIN_OUT = 5;
// kill output to 0 via 2N7000 MOSFET
const PROGMEM uint8_t PIN_OUT_Z = 4;

const PROGMEM uint8_t PIN_STATUS = 2;
const PROGMEM uint8_t PIN_PWM_IN = A7;

const PROGMEM int BAUD_RATE = 9600;

// LCD I2C address and size
const PROGMEM uint8_t LCD_I2C_ADDR = 0x27;
const PROGMEM uint8_t LCD_WIDTH = 16;
const PROGMEM uint8_t LCD_HEIGHT = 2;
LiquidCrystal_I2C lcd(LCD_I2C_ADDR, LCD_WIDTH, LCD_HEIGHT);

void setup() {
  pinMode(PIN_OUT, OUTPUT);
  pinMode(PIN_OUT_Z, OUTPUT);
  pinMode(PIN_STATUS, OUTPUT);
  pinMode(PIN_PWM_IN, INPUT);
  digitalWrite(PIN_STATUS, LOW);

  Serial.begin(BAUD_RATE);

  lcd.init();
  lcd.backlight();
  lcd.clear();
}

// Constants (set these based on your hardware)
const float C = 1e-6;      // Capacitance in Farads (e.g., 1 µF)
const float R = 10000.0;    // Resistance in Ohms (e.g., 10kΩ)
const float VIN = 5.0;     // Pulse voltage (typically 5V)

unsigned long pulseWidthForVoltage(float Vout) {
  if (Vout <= 0.0) return 0;
  if (Vout >= VIN) return 25000UL;  //  5  R * C

  float tau = R * C;
  float t_sec = -tau * log(1.0 - Vout / VIN);  // seconds
  unsigned long t_us = (unsigned long)(t_sec * 1e6);  // convert to microseconds
  return t_us;
}

// get pulse width  in
void setOutput(int val) {
  analogWrite(PIN_OUT, val);
  digitalWrite(PIN_OUT_Z, 0 == val); // force output to 0V if val is 0
}

void loop() {
  float vOut = analogRead(PIN_PWM_IN) * VIN / 1023;
  lcd.clear();
  lcd.print(vOut);
  unsigned long pulseWidth = pulseWidthForVoltage(vOut);
  digitalWrite(PIN_OUT, HIGH);
  delayMicroseconds(pulseWidth);
  digitalWrite(PIN_OUT, LOW);
  delay(500);
  digitalWrite(PIN_OUT_Z, HIGH);
  digitalWrite(PIN_OUT_Z, LOW);
}