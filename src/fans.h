/**
 * @file  fans.h
 * @copyright Copyright (c) 2024
 */

#pragma once

#include <Arduino.h>

typedef enum {
  FAN_OFF = 0,
  FAN_LOW = 64,
  FAN_MEDIUM = 128,
  FAN_HIGH = 192,
  FAN_MAX = 255
} FanSpeed;

class Fans {
 public:
  Fans();
  void init();
  void update(int co2);
  FanSpeed get_speed();
  const char *get_speed(bool is_short);
  void set_override(bool override);
  bool toggle_override();
  bool get_override();

 private:
  FanSpeed current_speed;
  bool override;
  void set_speed(FanSpeed speed);
};