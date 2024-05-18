/**
 * @file  mhz19b.h
 * @copyright Copyright (c) 2024
 */

#pragma once

#include <MHZ19.h>
#include <TickTwo.h>

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
  float get_temperature(TemperatureUnit unit);
  const char* get_air_quality();

 private:
  MHZ19 mhz19b;
  int co2;
  float temperature;
  TemperatureUnit unit;
  TickTwo* mhz19b_ticker;
};