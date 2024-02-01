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
#include "environment.h"
#include "oled.h"

#define OLED_SCL 22
#define OLED_SDA 21

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, OLED_SCL, OLED_SDA);
Environment env;  // environment data [temperature, humidity, CO2] (saved from environment task)

void setup() {
  Serial.begin(9600);

  init_environment();
  init_oled();
  delay(2000);
}

void loop() {
  if (isnan(env.temperature) || isnan(env.humidity)) {
    u8g2.clearBuffer();
    u8g2.drawStr(0, 10, "Failed to read from DHT sensor!");
    u8g2.sendBuffer();
    delay(2000);
    return;
  }

  display_main_screen(env.temperature, env.humidity, env.co2);

  delay(500);
}
