#define LED_PIN 13
#define NUM_ITERATIONS 5000

void setup() {
  pinMode(LED_PIN, OUTPUT); // Setăm pinul 13 ca ieșire
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(12, OUTPUT);
}

void loop() {
  // Blink 5000 times at 10Hz
  if(digitalRead(2) == HIGH)
  for (int i = 0; i < NUM_ITERATIONS; i++) {
    digitalWrite(12, HIGH);
    digitalWrite(LED_PIN, HIGH); // Setăm pinul 13 HIGH
    delay(50); // Așteptăm 50ms (jumătate de perioadă pentru 10Hz)
    digitalWrite(LED_PIN, LOW); // Setăm pinul 13 LOW
    delay(50); // Așteptăm 50ms (jumătate de perioadă pentru 10Hz)
  }

  // Blink 5000 times at 50Hz
  if(digitalRead(3) == HIGH)
  for (int i = 0; i < NUM_ITERATIONS; i++) {
    digitalWrite(12, HIGH);
    digitalWrite(LED_PIN, HIGH); // Setăm pinul 13 HIGH
    delay(10); // Așteptăm 10ms (jumătate de perioadă pentru 50Hz)
    digitalWrite(LED_PIN, LOW); // Setăm pinul 13 LOW
    delay(10); // Așteptăm 10ms (jumătate de perioadă pentru 50Hz)
  }

  // Blink 5000 times at 80Hz
  if(digitalRead(4) == HIGH)
  for (int i = 0; i < NUM_ITERATIONS; i++) {
    digitalWrite(12, HIGH);
    digitalWrite(LED_PIN, HIGH); // Setăm pinul 13 HIGH
    delayMicroseconds(6250); // Așteptăm 6.25ms (jumătate de perioadă pentru 80Hz)
    digitalWrite(LED_PIN, LOW); // Setăm pinul 13 LOW
    delayMicroseconds(6250); // Așteptăm 6.25ms (jumătate de perioadă pentru 80Hz)
  }

  // Blink 5000 times at 100Hz
  if(digitalRead(5) == HIGH)
  for (int i = 0; i < NUM_ITERATIONS; i++) {
    digitalWrite(12, HIGH);
    digitalWrite(LED_PIN, HIGH); // Setăm pinul 13 HIGH
    delay(5); // Așteptăm 5ms (jumătate de perioadă pentru 100Hz)
    digitalWrite(LED_PIN, LOW); // Setăm pinul 13 LOW
    delay(5); // Așteptăm 5ms (jumătate de perioadă pentru 100Hz)
  }

  // Blink 5000 times at 125Hz
  if(digitalRead(6) == HIGH)
  for (int i = 0; i < NUM_ITERATIONS; i++) {
    digitalWrite(12, HIGH);
    digitalWrite(LED_PIN, HIGH); // Setăm pinul 13 HIGH
    delay(4); // Așteptăm 4ms (jumătate de perioadă pentru 125Hz)
    digitalWrite(LED_PIN, LOW); // Setăm pinul 13 LOW
    delay(4); // Așteptăm 4ms (jumătate de perioadă pentru 125Hz)
  }

  digitalWrite(12, LOW);
}

