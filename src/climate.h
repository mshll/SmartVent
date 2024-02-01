/**
 * @file  climate.h
 * @brief Temperature and humidity sensor header file
 *
 * @note Climate task is running periodically to get temperature and humidity and is pinned to core 1
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __CLIMATE__H
#define __CLIMATE__H

#include <Arduino.h>
#include <DHTesp.h>

#define DHT_PIN 19
#define DHT_TYPE DHTesp::DHT11

/* function declarations */
void climate_task(void *pvParameters);
bool get_climate();

TaskHandle_t climate_task_handle = NULL;
DHTesp dht;
extern TempAndHumidity climate;

/**
 * @brief Initialize climate sensor
 *
 * @return true if successful
 *         false if failed
 */
bool init_climate() {
  dht.setup(DHT_PIN, DHT_TYPE);  // Initialize temperature and humidity sensor
  Serial.println("DHT initiated");

  // Start task to get temperature and humidity
  xTaskCreatePinnedToCore(climate_task,         /* Function to implement the task */
                          "climate_task",       /* Name of the task */
                          4000,                 /* Stack size in words */
                          NULL,                 /* Task input parameter */
                          5,                    /* Priority of the task */
                          &climate_task_handle, /* Task handle. */
                          1);                   /* Core where the task should run */

  if (climate_task_handle == NULL) {
    Serial.println("Failed to start task for temperature update");
    return false;
  }
  Serial.println("\nClimate task started\n");
  return true;
}

/**
 * @brief Task to get temperature and humidity from DHT11 sensor
 *
 * @param pvParameters
 */
void climate_task(void *pvParameters) {
  while (1) {
    get_climate();
    Serial.println("\n-- Climate task running --\n");
    vTaskDelay(pdMS_TO_TICKS(3000));  // Delay for 20 seconds
  }

  vTaskDelete(NULL);
}

/**
 * @brief Read temperature and humidity from DHT11 sensor
 *
 * @return true if successful
 *         false if failed
 */
bool get_climate() {
  // Reading temperature and humidity takes about 250 milliseconds
  // Sensor readings may also be up to 2 seconds 'old'
  TempAndHumidity newValues = dht.getTempAndHumidity();

  // Check if any reads failed and exit early (to try again).
  if (dht.getStatus() != 0) {
    Serial.println("DHT11 error status: " + String(dht.getStatusString()));
    return false;
  }

  float heatIndex = dht.computeHeatIndex(newValues.temperature, newValues.humidity);
  float dewPoint = dht.computeDewPoint(newValues.temperature, newValues.humidity);
  // float cr = dht.getComfortRatio(cf, newValues.temperature, newValues.humidity);

  climate = newValues;
  Serial.println(" T:" + String(climate.temperature) + " H:" + String(climate.humidity) + " I:" + String(heatIndex) + " D:" + String(dewPoint));
  return true;
}

#endif