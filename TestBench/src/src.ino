const int buttonPin = 2;   // The pin where the button is connected
const int outputPin = 13;   // The pin where the square wave will be generated

void setup() {
  pinMode(buttonPin, INPUT); // Set button pin as input with an internal pull-up resistor
  pinMode(outputPin, OUTPUT);       // Set output pin as output
}

void loop() {
  // Check if the button is pressed
  if (digitalRead(buttonPin) == HIGH) {
    // Generate a 100Hz square wave
    for (int i = 0; i < 100; i++) { // Generate 100 cycles of square wave
      digitalWrite(outputPin, HIGH);
      delayMicroseconds(5000);      // High for 5ms (half period of 100Hz)
      digitalWrite(outputPin, LOW);
      delayMicroseconds(5000);      // Low for 5ms (half period of 100Hz)
    }
  } else {
    digitalWrite(outputPin, LOW); // Ensure the output is low when the button is not pressed
  }
}

