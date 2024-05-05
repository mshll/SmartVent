/**
 * @file  mhz19b.h
 * @copyright Copyright (c) 2024
 */

#pragma once

#include <Arduino.h>
#include <MHZ19.h>

#define CELSIUS_SYMBOL "°C"
#define FAHRENHEIT_SYMBOL "°F"

typedef enum {
  CELSIUS = 0,
  FAHRENHEIT = 1
} TemperatureUnit;

class MHZ19B {
 public:
  MHZ19B();
  void init();
  void loop();
  void update_data();
  void set_unit(TemperatureUnit unit);
  TemperatureUnit get_unit();
  int get_co2();
  float get_temperature();

 private:
  MHZ19 mhz19b;
  int co2;
  float temperature;
  TemperatureUnit unit;
};