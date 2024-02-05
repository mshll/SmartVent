/**
 * @file  environment.h
 * @brief Environment abstraction to get temperature, humidity, and CO2
 *
 * @note Environment task is running periodically to get real-time data from the sensors and is pinned to core 1
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __ENVIRONMENT__H
#define __ENVIRONMENT__H

#include <Arduino.h>
#include <DHTesp.h>
#include <MQ135.h>
#include "climate.h"
#include "co2.h"

#define TASK_DELAY 3000  // in milliseconds

typedef struct {
  float temperature;
  float humidity;
  float co2;
} Environment;

TaskHandle_t env_task_handle = NULL;
extern Environment env;  // environment data [temperature, humidity, CO2] (periodically updated by environment task)

/* function declarations */
bool init_environment();
void environment_task(void *pvParameters);

/**
 * @brief Initialize environment
 *
 * @return true if successful
 *         false if failed
 */
bool init_environment() {
  init_climate();
  init_co2();

  // Start task to get environment data
  xTaskCreatePinnedToCore(environment_task,   /* Function to implement the task */
                          "environment_task", /* Name of the task */
                          4000,               /* Stack size in words */
                          NULL,               /* Task input parameter */
                          5,                  /* Priority of the task */
                          &env_task_handle,   /* Task handle. */
                          1);                 /* Core where the task should run */

  if (env_task_handle == NULL) {
    Serial.println("Failed to start environment task");
    return false;
  }

  Serial.println("Environment task started");
  return true;
}

/**
 * @brief Environment task to get temperature, humidity, and CO2
 *
 * @param pvParameters
 */
void environment_task(void *pvParameters) {
  while (1) {
    TempAndHumidity climate = get_climate();
    env.temperature = climate.temperature;
    env.humidity = climate.humidity;
    env.co2 = get_co2(climate.temperature, climate.humidity);

    Serial.println("Environment task running\n");
    vTaskDelay(pdMS_TO_TICKS(TASK_DELAY));
  }

  vTaskDelete(NULL);
}

#endif