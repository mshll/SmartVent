/**
 * @file    main.cpp
 * @author  Team 8:
 *            Abdulaziz Alateeqi,
 *            Meshal Almutairi,
 *            Fawzan Alfahad,
 *            Michael Fontaine,
 *            John Michael Mertz.
 * @brief   Smart Vent capstone project source code.
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <Arduino.h>
#include <DHTesp.h>
#include <MQ135.h>
#include <MQUnifiedsensor.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "climate.h"
#include "oled.h"

#define OLED_SCL 22
#define OLED_SDA 21

#define MQ135_PIN 33
#define board ("ESP-32")
#define Voltage_Resolution 3.3
#define ADC_Bit_Resolution 12   // For arduino UNO/MEGA/NANO
#define RatioMQ135CleanAir 3.6  // RS / R0 = 3.6 ppm

MQ135 mq135_sensor = MQ135(MQ135_PIN);
// MQUnifiedsensor MQ135(board, Voltage_Resolution, ADC_Bit_Resolution, MQ135_PIN, "MQ-135");
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, OLED_SCL, OLED_SDA);
TempAndHumidity climate;

/* function declarations */

void setup() {
  Serial.begin(9600);

  init_climate();
  init_oled();
  delay(2000);
}

void loop() {
  if (isnan(climate.temperature) || isnan(climate.humidity)) {
    u8g2.clearBuffer();
    u8g2.drawStr(0, 10, "Failed to read from DHT sensor!");
    u8g2.sendBuffer();
    delay(2000);
    return;
  }

  float ppm = -1;  // TODO

  display_main_screen(climate.temperature, climate.humidity, ppm);

  delay(500);
}

/* function definitions */
