/**
 * @file  fans.cpp
 * @copyright Copyright (c) 2024
 */

#include "fans.h"
#include "common.h"

Fans::Fans() {
  current_speed = FAN_OFF;
  override_speed = FAN_OFF;
  override = false;
}

void Fans::init() {
  ledcSetup(0, FANS_FREQUENCY, FANS_RESOLUTION);  // Channel 0, 25kHz frequency, 8-bit resolution
  ledcAttachPin(FANS_PIN, 0);                     // Connect PWM pin to channel 0 for Fans

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  override = pref_manager.get_value("override", false);
  override_speed = get_speed_from_index(pref_manager.get_value("override_speed", 0));

  set_speed(FAN_OFF);
}

void Fans::update(int co2) {
  if (override) {
    set_speed(override_speed);
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
  current_speed = speed;
  ledcWrite(0, speed == FAN_OFF ? FAN_MEDIUM : speed);  // When off set speed to medium but turn off relay (avoid some weird behavior)
  digitalWrite(RELAY_PIN, speed == FAN_OFF ? LOW : HIGH);
}

FanSpeed Fans::get_speed() {
  return current_speed;
}

const char *Fans::get_speed(bool is_short, bool force, FanSpeed speed) {
  FanSpeed cur_speed = force ? speed : current_speed;
  switch (cur_speed) {
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
  pref_manager.set_value("override", override);
}

bool Fans::toggle_override() {
  override = !override;
  pref_manager.set_value("override", override);
  return override;
}

bool Fans::get_override() {
  return override;
}

FanSpeed get_speed_from_index(int index) {
  switch (index) {
    case 0:
      return FAN_OFF;
    case 1:
      return FAN_LOW;
    case 2:
      return FAN_MEDIUM;
    case 3:
      return FAN_HIGH;
    case 4:
      return FAN_MAX;
    default:
      return FAN_OFF;
  }
}

int get_index_from_speed(FanSpeed speed) {
  switch (speed) {
    case FAN_OFF:
      return 0;
    case FAN_LOW:
      return 1;
    case FAN_MEDIUM:
      return 2;
    case FAN_HIGH:
      return 3;
    case FAN_MAX:
      return 4;
    default:
      return 0;
  }
}