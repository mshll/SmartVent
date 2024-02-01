#include <Arduino.h>

// Fan 1
const int pwmPin1 = 12; // GPIO12

// Fan 2
const int pwmPin2 = 14; // GPIO14

const int speedStates[] = {64, 128, 192, 255, 0}; // Speed settings (25%, 50%, 75%, 100%, off)
const int numberOfStates = sizeof(speedStates) / sizeof(speedStates[0]);
const long interval = 5000; // Duration for each state (5000 milliseconds = 5 seconds)

void setup() {
    // Initialize serial communication at a baud rate of 115200
    Serial.begin(115200);
    
    // Fan 1 PWM setup
    ledcSetup(0, 10000, 8); // Channel 0, 10kHz frequency, 8-bit resolution
    ledcAttachPin(pwmPin1, 0); // Connect PWM pin to channel 0 for Fan 1

    // Fan 2 PWM setup
    ledcSetup(1, 10000, 8); // Channel 1, 10kHz frequency, 8-bit resolution
    ledcAttachPin(pwmPin2, 1); // Connect PWM pin to channel 1 for Fan 2
}

void loop() {
    for (int i = 0; i < numberOfStates; i++) {
        // Set the speed for both fans to the current state
        ledcWrite(0, speedStates[i]); // Control Fan 1
        ledcWrite(1, speedStates[i]); // Control Fan 2

        // Wait for 5 seconds before changing to the next state
        delay(interval);
    }
}
