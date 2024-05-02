#include <Arduino.h>

const int pwmPin1 = 23; // GPIO12 for PWM control
const int pwmPin2 = 22; // GPIO14 for PWM control
const int relayPin = 16; // ESP32 pin GPIO16 connected to the fan via the relay
const int speedStates[] = {64, 128, 192, 255, 0}; // Speed settings (25%, 50%, 75%, 100%, off)
const int numberOfStates = sizeof(speedStates) / sizeof(speedStates[0]); // Calculate the number of speed states

void setup() {
  Serial.begin(115200); // Initialize serial communication at a baud rate of 115200
  ledcSetup(0, 10000, 8); // Channel 0, 10kHz frequency, 8-bit resolution for PWM
  ledcAttachPin(pwmPin1, 0); // Connect the PWM pin to channel 0 for fan control
  ledcSetup(1, 10000, 8); // Channel 1, 10kHz frequency, 8-bit resolution for PWM
  ledcAttachPin(pwmPin2, 0); // Connect the PWM pin to channel 0 for fan control
  pinMode(relayPin, OUTPUT); // Initialize the relay pin as an output
}

void loop() {
   // Turn on the fan via the relay

  for (int i = 0; i < numberOfStates; i++) {
    if (speedStates[i] == 0){
      Serial.printf("Speed = %d\n", speedStates[i]);
      ledcWrite(0, speedStates[i]); // Set the fan speed according to the current state
      ledcWrite(1, speedStates[i]); // Set the fan speed according to the current state
      digitalWrite(relayPin, HIGH); // Turn off the fan via the relay
      delay(10000); // Wait for 10 seconds before the next loop iteration
    }
    else{
    digitalWrite(relayPin, LOW); //Turn it on using relay
    Serial.printf("Speed = %d\n", speedStates[i]); // Output the current speed to the serial monitor
    ledcWrite(0, speedStates[i]); // Set the fan speed according to the current state
    ledcWrite(1, speedStates[i]); // Set the fan speed according to the current state
    delay(5000); // Wait for 5 seconds before changing to the next state
        }
      }
    }