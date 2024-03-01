/**
 * @file  co2.h
 * @brief CO2 sensor (MQ-135) header file
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __CO2__H
#define __CO2__H

#include <Arduino.h>
#include <MQ135.h>

#define MQ135_PIN 33

MQ135 mq135_sensor = MQ135(MQ135_PIN, 58.0, 22.0);

/* function declarations */
void init_co2();
float get_co2(float temperature, float humidity);
void print_rzero();

/**
 * @brief Initialize CO2 sensor
 *
 */
void init_co2() {
  analogReadResolution(10);
  pinMode(MQ135_PIN, INPUT);
  Serial.println("MQ135 initiated");
}

/**
 * @brief Get CO2 concentration
 *
 * @return CO2 concentration in ppm
 */
float get_co2(float temperature, float humidity) {
  int val = analogRead(MQ135_PIN);
  Serial.print("raw = ");
  Serial.println(val);

  float zero = mq135_sensor.getRZero();
  Serial.print("rzero: ");
  Serial.println(zero);

  float corrected_rzero = mq135_sensor.getCorrectedRZero(temperature, humidity);
  Serial.print("corrected rzero: ");
  Serial.println(corrected_rzero);

  float ppm = mq135_sensor.getPPM();
  Serial.print("ppm: ");
  Serial.println(ppm);

  float corrected_ppm = mq135_sensor.getCorrectedPPM(temperature, humidity);
  Serial.print("corrected ppm: ");
  Serial.println(corrected_ppm);

  return corrected_ppm;
}

/**
 * @brief Print RZero value
 *
 */
void print_rzero() {
  Serial.print("RZero: ");
  Serial.println(mq135_sensor.getRZero());

  Serial.print("Corrected RZero: ");
  Serial.println(mq135_sensor.getCorrectedRZero(20, 50));
}

#endif