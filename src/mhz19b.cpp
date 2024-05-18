/**
 * @file  mhz19b.cpp
 * @copyright Copyright (c) 2024
 */

#include "mhz19b.h"
#include "common.h"

#define READING_INTERVAL 2000

HardwareSerial mhz19b_serial(1);

MHZ19B::MHZ19B() {
  mhz19b = MHZ19();
  co2 = 0;
  temperature = 0.0;
  unit = CELSIUS;
  mhz19b_ticker = new TickTwo(std::bind(&MHZ19B::update_data, this), READING_INTERVAL);
}

void MHZ19B::init() {
  mhz19b_serial.begin(9600);
  mhz19b.begin(mhz19b_serial);

  mhz19b.autoCalibration(true);
  mhz19b_ticker->start();
}

void MHZ19B::loop() {
  mhz19b_ticker->update();
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

const char* MHZ19B::get_air_quality() {
  if (co2 < 1000) return "Good";
  if (co2 <= 1400) return "Fair";
  if (co2 > 1400) return "Unhealthy";
  return "Unknown";
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

float MHZ19B::get_temperature(TemperatureUnit unit) {
  if (unit == FAHRENHEIT) return temperature * 1.8 + 32;
  return temperature;
}