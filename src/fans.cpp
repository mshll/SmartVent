/**
 * @file  fans.cpp
 * @copyright Copyright (c) 2024
 */

#include "fans.h"
#include "common.h"

Fans::Fans() {
  current_speed = FAN_OFF;
  override = false;
}

void Fans::init() {
  ledcSetup(0, FANS_FREQUENCY, FANS_RESOLUTION);  // Channel 0, 25kHz frequency, 8-bit resolution
  ledcAttachPin(FANS_PIN, 0);                     // Connect PWM pin to channel 0 for Fans

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  set_speed(FAN_OFF);
}

void Fans::update(int co2) {
  if (override) set_speed(FAN_OFF);

  if (co2 < FAN_THRESHOLD_OFF) {
    set_speed(FAN_OFF);
  } else if (co2 < FAN_THRESHOLD_LOW) {
    set_speed(FAN_LOW);
  } else if (co2 < FAN_THRESHOLD_MEDIUM) {
    set_speed(FAN_MEDIUM);
  } else if (co2 < FAN_THRESHOLD_HIGH) {
    set_speed(FAN_HIGH);
  } else {
    set_speed(FAN_MAX);
  }
}

void Fans::set_speed(FanSpeed speed) {
  ledcWrite(0, speed);
  current_speed = speed;

  if (speed == FAN_OFF) {
    digitalWrite(RELAY_PIN, LOW);
  } else {
    digitalWrite(RELAY_PIN, HIGH);
  }
}

FanSpeed Fans::get_speed() {
  return current_speed;
}

const char *Fans::get_speed(bool is_short) {
  if (override) return is_short ? "OFF" : "Force Off";

  switch (current_speed) {
    case FAN_OFF:
      return is_short ? "OFF" : "Off";
    case FAN_LOW:
      return is_short ? "LOW" : "Low";
    case FAN_MEDIUM:
      return is_short ? "MED" : "Medium";
    case FAN_HIGH:
      return is_short ? "HIGH" : "High";
    case FAN_MAX:
      return is_short ? "MAX" : "Max";
    default:
      return is_short ? "OFF" : "Off";
  }
}

void Fans::set_override(bool override) {
  this->override = override;
}

bool Fans::toggle_override() {
  override = !override;
  return override;
}

bool Fans::get_override() {
  return override;
}