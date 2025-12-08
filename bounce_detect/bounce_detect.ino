// Switch bounce detector for Arduino Nano (ATmega328P)
// Button between D2 and GND, using INPUT_PULLUP.
//
// Each burst of edges within ~30 ms is treated as one "event".
// If that event has multiple edges, we call it bounce.

const uint8_t PIN_BUTTON = 2;
const uint8_t PIN_LED = LED_BUILTIN;
const uint8_t PIN_BUZZER = 3;

volatile bool eventActive = false;
volatile unsigned int edgeCount = 0;
volatile unsigned long firstEdgeTime = 0;
volatile unsigned long lastEdgeTime = 0;

const unsigned long QUIET_TIME_US = 30000UL; // 30 ms of silence ends an event

void switchISR() {

  digitalWrite(PIN_LED, digitalRead(PIN_BUTTON)); // DEBUG

  unsigned long now = micros();

  if (!eventActive) {
    eventActive = true;
    edgeCount = 1;
    firstEdgeTime = now;
    lastEdgeTime = now;
  } else {
    edgeCount++;
    lastEdgeTime = now;
  }
}

void beep() {
  digitalWrite(PIN_BUZZER, HIGH);
  delay(50);
  digitalWrite(PIN_BUZZER, LOW);
}

void setup() {
  Serial.begin(9600);
  while (!Serial) { } // optional on Nano, but harmless

  pinMode(PIN_BUTTON, INPUT); // external pullup
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  digitalWrite(PIN_LED, digitalRead(PIN_BUTTON));
  digitalWrite(PIN_BUZZER, LOW);

  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON), switchISR, CHANGE);

  Serial.println(F("Switch bounce monitor ready."));
  Serial.println(F("Press and release the button a few times."));
  Serial.println(F("Each event within ~30 ms will be summarized.\n"));

  interrupts();
}

void loop() {

//  digitalWrite(PIN_LED, digitalRead(PIN_BUTTON));
  
  static bool lastEventActive = false;

  // Take a snapshot of volatile variables safely
  bool active;
  unsigned int edges;
  unsigned long tFirst, tLast, now;

  now = micros();
  noInterrupts();
  active = eventActive;
  edges = edgeCount;
  tFirst = firstEdgeTime;
  tLast = lastEdgeTime;
  interrupts();

  // If we saw edges and then things have been quiet for a while,
  // the "event" is finished.
  if (active && (now - tLast > QUIET_TIME_US)) {
    // End the event in a critical section
    noInterrupts();
    eventActive = false;
    interrupts();

    unsigned long duration = tLast - tFirst;

    Serial.print(F("Event: "));
    Serial.print(edges);
    Serial.print(F(" edges in "));
    Serial.print(duration);
    Serial.print(F(" us ("));
    Serial.print(duration / 1000.0, 2);
    Serial.println(F(" ms)"));

    if (edges <= 1) {
      Serial.println(F("→ Looks clean (no obvious bounce)."));
    } else {
      Serial.print(F("→ Bounce detected! "));
      Serial.print(edges);
      Serial.print(F(" edges in one transition to "));
      Serial.println(active ? F("HIGH") : F("LOW"));
      beep();
    }

    Serial.println();
  }

  // Nothing else to do; loop just waits and monitors
}
