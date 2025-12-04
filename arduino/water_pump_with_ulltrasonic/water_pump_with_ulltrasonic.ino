#define TRIG_PIN 9
#define ECHO_PIN 10
#define RELAY_PUMP 7

long duration;
float distance;

// Pump state tracking
bool pumpOn = false;

// Time tracking
unsigned long highWaterStart = 0;
unsigned long lowWaterStart = 0;

const int ON_THRESHOLD = 20;  // Pump turns ON when water is LOW (>= 20cm)
const int OFF_THRESHOLD = 10; // Pump turns OFF when water is HIGH (<= 10cm)
const int DELAY_MS = 2000;    // 2 seconds delay to avoid rapid switching

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RELAY_PUMP, OUTPUT);

  digitalWrite(RELAY_PUMP, HIGH); // pump OFF initially
  Serial.begin(9600);
}

float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2; // cm
}

void loop() {
  float d = getDistance();
  Serial.print("Distance: ");
  Serial.print(d);
  Serial.println(" cm");

  unsigned long now = millis();

  // -------------------------------------------
  // Water LOW (>= 20cm) → REQUEST PUMP ON
  // -------------------------------------------
  if (d >= ON_THRESHOLD) {
    if (lowWaterStart == 0) lowWaterStart = now;

    // If it stayed LOW for enough time → turn ON pump
    if (!pumpOn && (now - lowWaterStart >= DELAY_MS)) {
      digitalWrite(RELAY_PUMP, LOW);  // ON
      pumpOn = true;
      Serial.println("Pump: ON (after delay)");
    }
  } else {
    lowWaterStart = 0; // reset timer
  }

  // -------------------------------------------
  // Water HIGH (<= 10cm) → REQUEST PUMP OFF
  // -------------------------------------------
  if (d <= OFF_THRESHOLD) {
    if (highWaterStart == 0) highWaterStart = now;

    // If it stayed HIGH for enough time → turn OFF pump
    if (pumpOn && (now - highWaterStart >= DELAY_MS)) {
      digitalWrite(RELAY_PUMP, HIGH); // OFF
      pumpOn = false;
      Serial.println("Pump: OFF (after delay)");
    }
  } else {
    highWaterStart = 0; // reset timer
  }

  delay(300);
}
