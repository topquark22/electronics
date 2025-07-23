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
const PROGMEM float C = 1e-6;         // Capacitance in Farads (e.g., 1 µF)
const PROGMEM float R = 10000.0;       // Resistance in Ohms (e.g., 10kΩ)
const PROGMEM float Vdiode = 0.6; // adjust for diode drop voltage
const PROGMEM float Vdd = 5.0;

// pulse width for desired output voltage (microseconds)
uint32_t voltageToPulseWidth(float Vout) {
  const float tau = R * C;
  const float t_max = 5.0 * tau;  // cap at 5RC
  const float Vmax = Vdd - Vdiode;

  if (Vout <= Vdiode + 0.01) return 0; // below diode drop → skip
  if (Vout >= Vdd) return (uint32_t)(t_max * 1E6); // cap max width

  float t = -tau * log(1.0 - (Vout - Vdiode) / Vmax);
  if (t > t_max) t = t_max;

  return (uint32_t)(t * 1E6);  // in microseconds
}

float prevVout = -1;  // global or static

void loop() {
  float desiredVoltage = analogRead(PIN_PWM_IN) * Vdd / 1023.0;

  // Compensate for diode drop
  float vOut = desiredVoltage + Vdiode;

  // Limit to max
  if (vOut > Vdd) vOut = Vdd;

  if (abs(vOut - prevVout) > 0.01) {
    prevVout = vOut;

    uint32_t pulseWidth = voltageToPulseWidth(vOut);

    // Reset and pulse
    digitalWrite(PIN_OUT_Z, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_OUT_Z, LOW);

    if (pulseWidth > 0) {
      digitalWrite(PIN_OUT, HIGH);
      if (pulseWidth >= 1000) delay(pulseWidth / 1000);
      delayMicroseconds(pulseWidth % 1000);
      digitalWrite(PIN_OUT, LOW);
    }

    // Display the *true* output voltage, not adjusted one
    lcd.clear();
    lcd.print("V=");
    lcd.print(desiredVoltage);  // not vOut
    lcd.setCursor(0, 1);
    lcd.print("t=");
    lcd.print(pulseWidth);
  }

  delay(50);
}