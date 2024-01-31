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
#include <DHT.h>
#include <MQ135.h>
#include <U8g2lib.h>
#include <Wire.h>

#define OLED_SCL 22
#define OLED_SDA 21
#define MQ135_PIN 34
#define DHT_PIN 19
#define DHT_TYPE DHT11

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, OLED_SCL, OLED_SDA);
DHT dht(DHT_PIN, DHT_TYPE);
MQ135 gasSensor = MQ135(MQ135_PIN);

/* function declarations */

void setup() {
  Serial.begin(115200);

  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setCursor(30, 32);
  u8g2.print("Smart Vent");
  u8g2.sendBuffer();
  delay(2000);

  dht.begin();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    u8g2.clearBuffer();
    u8g2.drawStr(0, 10, "Failed to read from DHT sensor!");
    u8g2.sendBuffer();
    delay(2000);
    return;
  }

  float rzero = gasSensor.getRZero();
  float correctedRZero = gasSensor.getCorrectedRZero(temperature, humidity);
  float resistance = gasSensor.getResistance();
  float ppm = gasSensor.getPPM();
  float correctedPPM = gasSensor.getCorrectedPPM(temperature, humidity);
  Serial.print("MQ135 RZero: ");
  Serial.print(rzero);
  Serial.print("\t Corrected RZero: ");
  Serial.print(correctedRZero);
  Serial.print("\t Resistance: ");
  Serial.print(resistance);
  Serial.print("\t PPM: ");
  Serial.print(ppm);
  Serial.print("\t Corrected PPM: ");
  Serial.print(correctedPPM);
  Serial.println(" ppm");

  // display on OLED
  u8g2.clearBuffer();
  u8g2.setCursor(0, 15);
  u8g2.print("Temp: ");
  u8g2.print(temperature);
  u8g2.print(" C");

  u8g2.setCursor(0, 30);
  u8g2.print("Hum: ");
  u8g2.print(humidity);
  u8g2.print("%");

  u8g2.setCursor(0, 45);
  u8g2.print("CO2: ");
  u8g2.print(ppm);
  u8g2.print(" ppm");

  u8g2.sendBuffer();

  delay(500);
}

/* function definitions */
