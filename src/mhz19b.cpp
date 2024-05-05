/**
 * @file  mhz19b.cpp
 * @copyright Copyright (c) 2024
 */

#include "mhz19b.h"
#include <TickTwo.h>

#define READING_INTERVAL 2000

void update_data();

HardwareSerial mhz19b_serial(1);
TickTwo mhz19b_ticker(update_data, READING_INTERVAL);

MHZ19B::MHZ19B() {
  mhz19b = MHZ19();
  co2 = 0;
  temperature = 0.0;
  unit = CELSIUS;
}

void MHZ19B::init() {
  mhz19b_serial.begin(9600);
  mhz19b.begin(mhz19b_serial);

  mhz19b.autoCalibration(true);
  mhz19b_ticker.start();
}

void MHZ19B::loop() {
  mhz19b_ticker.update();
}

void MHZ19B::update_data() {
  int co2 = mhz19b.getCO2();
  float temperature = mhz19b.getTemperature();

  if (co2 > 0) this->co2 = co2;
  if (temperature > 0.0) this->temperature = temperature;

  Serial.println("-------------------- MH-Z19B --------------------");
  Serial.println("CO2: " + String(co2) + " ppm // Temperature: " + String(temperature) + " °C");
  Serial.println("-------------------------------------------------");
}

void MHZ19B::set_unit(TemperatureUnit unit) {
  this->unit = unit;
}

TemperatureUnit MHZ19B::get_unit() {
  return unit;
}

int MHZ19B::get_co2() {
  return co2;
}

float MHZ19B::get_temperature() {
  if (unit == FAHRENHEIT) return temperature * 1.8 + 32;
  return temperature;
}

extern MHZ19B mhz19b;

void update_data() {
  mhz19b.update_data();
}