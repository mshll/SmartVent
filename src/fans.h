#ifndef __FANS__H
#define __FANS__H

#include <Arduino.h>

#define FAN1_PIN 17
#define FAN2_PIN 21
#define FANS_FREQUENCY 25000  // PWM frequency
#define FANS_RESOLUTION 8     // PWM resolution

typedef enum {
  FAN_OFF = 0,
  FAN_LOW = 64,
  FAN_MEDIUM = 128,
  FAN_HIGH = 192,
  FAN_MAX = 255
} FanSpeed;

const int fans_speeds[] = {0, 64, 128, 192, 255};  // Speed settings (off, 25%, 50%, 75%, 100%)
const int fans_states = sizeof(fans_speeds) / sizeof(fans_speeds[0]);
FanSpeed current_fan_speed = FAN_OFF;

/* function declarations */
void init_fans();
void set_fan_speed(FanSpeed speed);
FanSpeed get_fan_speed();
const char *get_fan_speed_str();
void update_fans(int co2);

/**
 * @brief Update fan speed based on CO2 level
 *
 * @param co2 CO2 level
 */
void update_fans(int co2) {
  if (co2 < 800) {
    set_fan_speed(FAN_OFF);
  } else if (co2 < 1000) {
    set_fan_speed(FAN_LOW);
  } else if (co2 < 1500) {
    set_fan_speed(FAN_MEDIUM);
  } else if (co2 < 2000) {
    set_fan_speed(FAN_HIGH);
  } else {
    set_fan_speed(FAN_MAX);
  }
}

void init_fans() {
  // Fan 1 PWM setup
  ledcSetup(0, FANS_FREQUENCY, FANS_RESOLUTION);  // Channel 0, 25kHz frequency, 8-bit resolution
  ledcAttachPin(FAN1_PIN, 0);                     // Connect PWM pin to channel 0 for Fan 1

  // Fan 2 PWM setup
  ledcSetup(1, FANS_FREQUENCY, FANS_RESOLUTION);  // Channel 1, 25kHz frequency, 8-bit resolution
  ledcAttachPin(FAN2_PIN, 1);                     // Connect PWM pin to channel 1 for Fan 2

  set_fan_speed(FAN_OFF);
}

void set_fan_speed(FanSpeed speed) {
  ledcWrite(0, speed);  // Control Fan 1
  ledcWrite(1, speed);  // Control Fan 2
  current_fan_speed = speed;

  // Serial.print("Fan speed set to ");
  // Serial.println(speed);
}

FanSpeed get_fan_speed() {
  return current_fan_speed;
}

const char *get_fan_speed_str() {
  switch (current_fan_speed) {
    case FAN_OFF:
      return "OFF";
    case FAN_LOW:
      return "LOW";
    case FAN_MEDIUM:
      return "MED";
    case FAN_HIGH:
      return "HIGH";
    case FAN_MAX:
      return "MAX";
    default:
      return "UNKNOWN";
  }
}

#endif
