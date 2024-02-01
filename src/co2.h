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
#include <MQUnifiedsensor.h>

#define MQ135_PIN 33
#define board ("ESP-32")
#define Voltage_Resolution 3.3
#define ADC_Bit_Resolution 12   // For arduino UNO/MEGA/NANO
#define RatioMQ135CleanAir 3.6  // RS / R0 = 3.6 ppm

MQ135 mq135_sensor = MQ135(MQ135_PIN);
// MQUnifiedsensor MQ135(board, Voltage_Resolution, ADC_Bit_Resolution, MQ135_PIN, "MQ-135");

/* function declarations */
void init_co2();
float get_co2(float temperature, float humidity);
float getCorrectionFactor(float t, float h);

/**
 * @brief Initialize CO2 sensor
 *
 * @return true if successful
 *         false if failed
 */
void init_co2() {
  // MQ135.setRegressionMethod(1);  //_PPM =  a*ratio^b
  // MQ135.setA(110.47);
  // MQ135.setB(-2.862);  // Configure the equation to to calculate C02 concentration
  // MQ135.init();

  // Serial.print("Calibrating please wait.");
  // float calcR0 = 0;
  // for (int i = 1; i <= 10; i++) {
  //   MQ135.update();  // Update data, the arduino will read the voltage from the analog pin
  //   calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
  //   Serial.print(".");
  // }
  // MQ135.setR0(calcR0 / 10);
  // Serial.println("  done!.");

  // if (isinf(calcR0)) {
  //   Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply");
  // }
  // if (calcR0 == 0) {
  //   Serial.println("Warning: Conection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply");
  // }
  // MQ135.serialDebug(true);

  Serial.println("MQ135 initiated");
}

/**
 * @brief Get CO2 concentration
 *
 * @return CO2 concentration in ppm
 */
float get_co2(float temperature, float humidity) {
  // float cFactor = 0;
  // if (!isnan(climate.temperature) && !isnan(climate.humidity)) cFactor = getCorrectionFactor(climate.temperature, climate.humidity);
  // Serial.print("Correction Factor: ");
  // Serial.println(cFactor);
  // MQ135.update();  // Update data, the arduino will read the voltage from the analog pin
  // float ppm = MQ135.readSensor(false, cFactor);
  // MQ135.serialDebug();  // Will print the table on the serial port

  float RZero = mq135_sensor.getRZero();
  float RZero_corrected = mq135_sensor.getCorrectedRZero(temperature, humidity);  // corrects RZero
  float ppm = mq135_sensor.getPPM();
  float ppm_corrected = mq135_sensor.getCorrectedPPM(temperature, humidity);  // corrects PPM

  Serial.print("RZero: ");
  Serial.println(RZero);
  Serial.print("RZero_corrected: ");
  Serial.println(RZero_corrected);
  Serial.print("PPM: ");
  Serial.println(ppm);
  Serial.print("PPM_corrected: ");
  Serial.println(ppm_corrected);
  Serial.println();

  return ppm_corrected;
}

/**
 * @brief Get the correction factor to correct for temperature and humidity
 *
 * @param t The ambient air temperature
 * @param h The relative humidity
 * @return The calculated correction factor
 */
float getCorrectionFactor(float t, float h) {
  return CORA * t * t - CORB * t + CORC - (h - 33.) * CORD;
}

#endif