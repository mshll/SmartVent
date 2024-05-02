/**
 * @file  climate.h
 * @brief Temperature and humidity sensor header file
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __CLIMATE__H
#define __CLIMATE__H

#include <Arduino.h>
#include <DHTesp.h>

#define DHT_PIN 27  // 19
#define DHT_TYPE DHTesp::DHT11

DHTesp dht;

/* function declarations */
void init_climate();
TempAndHumidity get_climate();

/**
 * @brief Initialize climate sensor
 *
 * @return true if successful
 *         false if failed
 */
void init_climate() {
  dht.setup(DHT_PIN, DHT_TYPE);
  Serial.println("DHT initiated");
}

/**
 * @brief Read temperature and humidity from DHT11 sensor
 *
 * @return temperature and humidity data
 */
TempAndHumidity get_climate() {
  // Reading temperature and humidity takes about 250 milliseconds
  // Sensor readings may also be up to 2 seconds 'old'
  TempAndHumidity climate = dht.getTempAndHumidity();

  // Check if any reads failed
  if (dht.getStatus() != 0) {
    Serial.println("DHT11 error status: " + String(dht.getStatusString()));
  }

  float heatIndex = dht.computeHeatIndex(climate.temperature, climate.humidity);
  float dewPoint = dht.computeDewPoint(climate.temperature, climate.humidity);
  // float cr = dht.getComfortRatio(cf, climate.temperature, climate.humidity);

  // Serial.println(" T:" + String(climate.temperature) + " H:" + String(climate.humidity) + " I:" + String(heatIndex) + " D:" + String(dewPoint));

  return climate;
}

#endif