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
  if (override) {
    set_speed(FAN_OFF);
    return;
  }

  if (co2 < CO2_THRESHOLD_1) {
    set_speed(FAN_OFF);
  } else if (co2 < CO2_THRESHOLD_2) {
    set_speed(FAN_LOW);
  } else if (co2 < CO2_THRESHOLD_3) {
    set_speed(FAN_MEDIUM);
  } else if (co2 < CO2_THRESHOLD_4) {
    set_speed(FAN_HIGH);
  } else {
    set_speed(FAN_MAX);
  }
}

void Fans::set_speed(FanSpeed speed) {
  ledcWrite(0, speed);
  current_speed = speed;
  digitalWrite(RELAY_PIN, speed == FAN_OFF ? LOW : HIGH);
}

FanSpeed Fans::get_speed() {
  return current_speed;
}

const char *Fans::get_speed(bool is_short) {
  if (override) return is_short ? "Off" : "Forced Off";

  switch (current_speed) {
    case FAN_OFF:
      return is_short ? "Off" : "Off";
    case FAN_LOW:
      return is_short ? "Low" : "Low";
    case FAN_MEDIUM:
      return is_short ? "Med" : "Medium";
    case FAN_HIGH:
      return is_short ? "High" : "High";
    case FAN_MAX:
      return is_short ? "Max" : "Max";
    default:
      return is_short ? "Off" : "Off";
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