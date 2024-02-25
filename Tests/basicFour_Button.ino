// Pin definitions
const int leftButtonPin = 2;
const int rightButtonPin = 3;
const int selectButtonPin = 4;
const int homeButtonPin = 5;

void setup() {
  // Initialize Serial communication
  Serial.begin(9600);

  // Set button pins as input with pull-up resistors enabled
  pinMode(leftButtonPin, INPUT_PULLUP);
  pinMode(rightButtonPin, INPUT_PULLUP);
  pinMode(selectButtonPin, INPUT_PULLUP);
  pinMode(homeButtonPin, INPUT_PULLUP);
}

void loop() {
  // Check each button and print its name if pressed
  if (digitalRead(leftButtonPin) == LOW) {
    Serial.println("Left button pressed");
    delay(100); // Debounce delay
  }

  if (digitalRead(rightButtonPin) == LOW) {
    Serial.println("Right button pressed");
    delay(100); // Debounce delay
  }

  if (digitalRead(selectButtonPin) == LOW) {
    Serial.println("Select button pressed");
    delay(100); // Debounce delay
  }

  if (digitalRead(homeButtonPin) == LOW) {
    Serial.println("Home button pressed");
    delay(100); // Debounce delay
  }
}
