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
#include <MycilaESPConnect.h>
#include <U8g2lib.h>

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

/* function declarations */
void init_oled();
void display_main_screen(float temp, float hum, float co2);
void display_splash_screen();
void split_float(float f, String &int_part, String &dec_part);

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

  String temp_int, temp_dec, hum_int, hum_dec;
  split_float(temp, temp_int, temp_dec);
  split_float(hum, hum_int, hum_dec);

  //********************
  static const unsigned char image_wifi_on_bits[] U8X8_PROGMEM = {0xfe, 0x00, 0x01, 0x01, 0x7c, 0x00, 0x82, 0x00, 0x38, 0x00, 0x00, 0x00, 0x10, 0x00};
  static const unsigned char image_wifi_off_bits[] U8X8_PROGMEM = {0xd6, 0x00, 0x11, 0x01, 0x54, 0x00, 0x92,
                                                                   0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00};
  static const unsigned char image_temperature_bits[] U8X8_PROGMEM = {0x70, 0x00, 0x88, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8,
                                                                      0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0x24, 0x01, 0x72, 0x02,
                                                                      0xea, 0x02, 0xfa, 0x02, 0x72, 0x02, 0x04, 0x01, 0xf8, 0x00};
  static const unsigned char image_weather_humidity_bits[] U8X8_PROGMEM = {0x20, 0x00, 0x20, 0x00, 0x30, 0x00, 0x70, 0x00, 0x78, 0x00, 0xf8,
                                                                           0x00, 0xfc, 0x01, 0xfc, 0x01, 0x7e, 0x03, 0xfe, 0x02, 0xff, 0x06,
                                                                           0xff, 0x07, 0xfe, 0x03, 0xfe, 0x03, 0xfc, 0x01, 0xf0, 0x00};
  static const unsigned char image_percent_bits[] U8X8_PROGMEM = {0xc2, 0x00, 0x65, 0x00, 0x32, 0x00, 0x98, 0x00, 0x4c, 0x01, 0x86, 0x00};
  static const unsigned char image_paint_0_bits[] U8X8_PROGMEM = {0x3c, 0x1c, 0x00, 0x66, 0x36, 0x00, 0x43, 0x63, 0x00, 0x03, 0x63, 0x02,
                                                                  0x03, 0x63, 0x05, 0x43, 0x63, 0x04, 0x66, 0x36, 0x02, 0x3c, 0x1c, 0x07};
  static const unsigned char image_Pin_star_bits[] U8X8_PROGMEM = {0x49, 0x2a, 0x1c, 0x7f, 0x1c, 0x2a, 0x49};

  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);
  u8g2.drawXBM(114, 0, 9, 7, ESPConnect.getState() == ESPConnectState::NETWORK_CONNECTED ? image_wifi_on_bits : image_wifi_off_bits);
  u8g2.drawXBM(9, 17, 11, 16, image_temperature_bits);
  u8g2.drawXBM(68, 17, 11, 16, image_weather_humidity_bits);
  u8g2.setFont(u8g2_font_profont22_mf);
  u8g2.drawStr(22, 32, temp_int.c_str());
  u8g2.setFont(u8g2_font_profont10_mf);
  u8g2.drawStr(44, 32, temp_dec.c_str());
  u8g2.drawEllipse(50, 19, 1, 1);
  u8g2.drawStr(54, 24, "C");
  u8g2.setFont(u8g2_font_profont22_mf);
  u8g2.drawStr(82, 32, hum_int.c_str());
  u8g2.setFont(u8g2_font_profont10_mf);
  u8g2.drawStr(104, 32, hum_dec.c_str());
  u8g2.setFont(u8g2_font_profont29_mf);
  u8g2.drawStr(35, 59, String(co2, 0).c_str());
  u8g2.drawXBM(109, 18, 9, 6, image_percent_bits);
  u8g2.setFont(u8g2_font_6x10_mf);
  u8g2.drawStr(14, 57, "ppm");
  u8g2.drawLine(5, 13, 5, 63);
  u8g2.drawXBM(13, 40, 19, 8, image_paint_0_bits);
  u8g2.drawLine(122, 13, 122, 63);
  u8g2.drawLine(121, 13, 6, 13);
  u8g2.drawLine(121, 63, 6, 63);
  u8g2.drawLine(121, 36, 6, 36);
  u8g2.drawLine(121, 35, 6, 35);
  u8g2.drawLine(121, 62, 6, 62);
  u8g2.drawLine(4, 63, 4, 13);
  u8g2.drawLine(123, 63, 123, 13);
  u8g2.drawLine(123, 12, 4, 12);
  u8g2.drawXBM(4, 0, 7, 7, image_Pin_star_bits);
  u8g2.setFont(u8g2_font_4x6_mf);
  u8g2.drawStr(13, 6, get_fan_speed_str());

  //********************

  u8g2.setFont(u8g2_font_04b_03_tr);
  float rzero = mq135_sensor.getRZero();
  float corrected_rzero = mq135_sensor.getCorrectedRZero(temp, hum);
  u8g2.drawStr(10, 5, ("r0: " + String(rzero, 2) + "   r0c: " + String(corrected_rzero, 2)).c_str());

  u8g2.sendBuffer();
}

void display_splash_screen() {
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);
  u8g2.setFont(u8g2_font_timR18_tf);
  u8g2.drawStr(12, 46, "SmartVent");
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.drawStr(36, 22, "LaunchPilot");
  u8g2.sendBuffer();
}

/**
 * @brief Split float into integer and decimal parts
 *
 * @param f float number
 * @param int_part integer part
 * @param dec_part decimal part
 */
void split_float(float number, String &intPart, String &decPart) {
  // Convert float to string with 2 decimal places
  char floatStr[10];
  dtostrf(number, 0, 2, floatStr);

  // Convert char array to String object for easier manipulation
  String floatString = String(floatStr);

  // Find position of decimal point
  int dotIndex = floatString.indexOf('.');

  // Extract integer part and decimal part based on position of decimal point
  intPart = floatString.substring(0, dotIndex);
  decPart = floatString.substring(dotIndex);

  // Add leading zero to decimal part if necessary (to ensure 2 decimal places)
  if (decPart.length() == 1) {
    decPart += "0";
  }
}

#endif