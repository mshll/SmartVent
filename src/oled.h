/**
 * @file oled.h
 * @brief OLED display header file
 *
 * Note: Using https://lopaka.app tool to design the UI
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __OLED__H
#define __OLED__H

#include <Arduino.h>
#include <MQ135.h>
#include <U8g2lib.h>

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

/* function declarations */
void init_oled();
void display_main_screen(float temp, float hum, float co2);

/**
 * @brief Initialize OLED display
 *
 */
void init_oled() {
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.setCursor(0, 32);
  u8g2.print("Smart Vent");
  u8g2.sendBuffer();
}

/**
 * @brief Display main screen on OLED
 *
 * @param temp temperature
 * @param hum humidity
 * @param co2 CO2 concentration
 */
void display_main_screen(float temp, float hum, float co2) {
  u8g2.clearBuffer();

  static const unsigned char image_Temperature_16x16_bits[] U8X8_PROGMEM = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x40, 0x01, 0x40, 0x01, 0x40,
                                                                            0x01, 0x40, 0x01, 0x40, 0x01, 0x40, 0x01, 0x20, 0x02, 0xe0, 0x03,
                                                                            0xe0, 0x03, 0xc0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  static const unsigned char image_Layer_11_bits[] U8X8_PROGMEM = {0x01, 0x01};
  static const unsigned char image_Layer_12_bits[] U8X8_PROGMEM = {0x08, 0x14, 0x14, 0x22, 0x22, 0x41, 0x41, 0x41, 0x41, 0x22, 0x1c};
  static const unsigned char image_Layer_24_bits[] U8X8_PROGMEM = {0x02, 0x05, 0x02};
  static const unsigned char image_Layer_25_bits[] U8X8_PROGMEM = {0x02, 0x05, 0x02};

  int temp_int = (int)temp;
  int temp_dec = (int)(temp * 100) % 100;
  int hum_int = (int)hum;
  int hum_dec = (int)(hum * 100) % 100;

  u8g2.setBitmapMode(1);
  u8g2.setFontMode(1);
  u8g2.drawXBMP(8, 13, 16, 16, image_Temperature_16x16_bits);
  u8g2.setFont(u8g2_font_profont22_tr);
  u8g2.drawStr(21, 28, String(temp_int).c_str());
  u8g2.setFont(u8g2_font_5x7_tf);
  u8g2.drawStr(44, 28, ("." + String(temp_dec)).c_str());
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(53, 20, "C");
  u8g2.setFont(u8g2_font_profont22_tr);
  u8g2.drawStr(80, 28, String(hum_int).c_str());
  u8g2.setFont(u8g2_font_5x7_tf);
  u8g2.drawStr(103, 28, ("." + String(hum_dec)).c_str());
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(123, 7, "%");
  u8g2.setFont(u8g2_font_profont22_tr);
  u8g2.drawStr(35, 51, String(co2, 1).c_str());
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.drawStr(13, 44, "CO");
  u8g2.drawXBMP(72, 21, 1, 2, image_Layer_11_bits);
  u8g2.drawXBMP(70, 15, 7, 11, image_Layer_12_bits);
  u8g2.setFont(u8g2_font_4x6_tr);
  u8g2.drawStr(29, 45, "2");
  u8g2.drawFrame(9, 9, 112, 46);
  u8g2.drawLine(10, 32, 119, 32);
  u8g2.drawLine(9, 31, 119, 31);
  u8g2.drawLine(9, 8, 120, 8);
  u8g2.drawLine(121, 54, 121, 9);
  u8g2.drawLine(9, 55, 120, 55);
  u8g2.drawLine(8, 9, 8, 54);
  u8g2.setFont(u8g2_font_5x8_tr);
  u8g2.drawStr(15, 51, "ppm");
  u8g2.drawLine(0, 0, 0, 0);
  u8g2.drawLine(116, 13, 107, 19);
  u8g2.drawXBMP(107, 13, 3, 3, image_Layer_24_bits);
  u8g2.drawXBMP(114, 17, 3, 3, image_Layer_24_bits);
  u8g2.drawXBMP(48, 13, 3, 3, image_Layer_25_bits);

  u8g2.setFont(u8g2_font_04b_03_tr);
  float rzero = mq135_sensor.getRZero();
  float corrected_rzero = mq135_sensor.getCorrectedRZero(temp, hum);
  u8g2.drawStr(10, 5, ("r0: " + String(rzero, 2) + "   r0c: " + String(corrected_rzero, 2)).c_str());

  u8g2.sendBuffer();
}

#endif