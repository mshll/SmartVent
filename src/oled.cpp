/**
 * @file  oled.cpp
 * @copyright Copyright (c) 2024
 */

#include "oled.h"
#include "common.h"
#include "fans.h"
#include "mhz19b.h"

#define TICKER_INTERVAL 3000

extern MHZ19B mhz19b;
extern Fans fans;

/* helper functions prototypes */
void split_float(float number, String &int_part, String &dec_part);

OLED::OLED() {
  u8g2 = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE, OLED_SCL, OLED_SDA);
  current_screen = SPLASH_SCREEN;

  oled_ticker = new TickTwo(std::bind(&OLED::oled_ticker_callback, this), TICKER_INTERVAL, 1);
}

void OLED::init() {
  u8g2->begin();
  u8g2->clearBuffer();
  u8g2->setFont(u8g2_font_ncenB14_tr);

  oled_ticker->start();
}

void OLED::loop() {
  u8g2->clearBuffer();

  switch (current_screen) {
    case SPLASH_SCREEN:
      display_splash_screen();
      break;
    case MAIN_SCREEN:
      display_main_screen();
      break;
  }

  u8g2->sendBuffer();
  oled_ticker->update();
}

/**
 * @brief Display main screen on OLED
 *
 * @param temp temperature
 * @param hum humidity
 * @param co2 CO2 concentration
 */
void OLED::display_main_screen() {
  String temp_int, temp_dec;
  split_float(mhz19b.get_temperature(), temp_int, temp_dec);
  int co2 = mhz19b.get_co2();
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

  u8g2->setFontMode(1);
  u8g2->setBitmapMode(1);
  u8g2->drawXBM(114, 0, 9, 7, ESPConnect.getState() == ESPConnectState::NETWORK_CONNECTED ? image_wifi_on_bits : image_wifi_off_bits);
  u8g2->drawXBM(9, 17, 11, 16, image_temperature_bits);
  u8g2->drawXBM(68, 17, 11, 16, image_weather_humidity_bits);
  u8g2->setFont(u8g2_font_profont22_mf);
  u8g2->drawStr(22, 32, temp_int.c_str());
  u8g2->setFont(u8g2_font_profont10_mf);
  u8g2->drawStr(44, 32, temp_dec.c_str());
  u8g2->drawEllipse(50, 19, 1, 1);
  u8g2->drawStr(54, 24, "C");
  u8g2->setFont(u8g2_font_profont22_mf);
  // u8g2->drawStr(82, 32, hum_int.c_str());
  // u8g2->setFont(u8g2_font_profont10_mf);
  // u8g2->drawStr(104, 32, hum_dec.c_str());
  u8g2->setFont(u8g2_font_profont29_mf);
  u8g2->drawStr(35, 59, String(co2, 0).c_str());
  u8g2->drawXBM(109, 18, 9, 6, image_percent_bits);
  u8g2->setFont(u8g2_font_6x10_mf);
  u8g2->drawStr(14, 57, "ppm");
  u8g2->drawLine(5, 13, 5, 63);
  u8g2->drawXBM(13, 40, 19, 8, image_paint_0_bits);
  u8g2->drawLine(122, 13, 122, 63);
  u8g2->drawLine(121, 13, 6, 13);
  u8g2->drawLine(121, 63, 6, 63);
  u8g2->drawLine(121, 36, 6, 36);
  u8g2->drawLine(121, 35, 6, 35);
  u8g2->drawLine(121, 62, 6, 62);
  u8g2->drawLine(4, 63, 4, 13);
  u8g2->drawLine(123, 63, 123, 13);
  u8g2->drawLine(123, 12, 4, 12);
  u8g2->drawXBM(4, 0, 7, 7, image_Pin_star_bits);
  u8g2->setFont(u8g2_font_4x6_mf);
  u8g2->drawStr(15, 6, fans.get_speed(1));
}

void OLED::display_splash_screen() {
  u8g2->setFontMode(1);
  u8g2->setBitmapMode(1);
  u8g2->setFont(u8g2_font_timR18_tf);
  u8g2->drawStr(12, 46, "SmartVent");
  u8g2->setFont(u8g2_font_helvB08_tf);
  u8g2->drawStr(36, 22, "LaunchPilot");
}

void OLED::oled_ticker_callback() {
  switch (current_screen) {
    case SPLASH_SCREEN:
      current_screen = MAIN_SCREEN;
      break;
    case MAIN_SCREEN:
      break;
  }
}

void OLED::set_screen(Screen screen) {
  current_screen = screen;
}

void OLED::set_enabled(bool enabled) {
  this->enabled = enabled;
}

bool OLED::toggle() {
  enabled = !enabled;
  return enabled;
}

bool OLED::is_enabled() {
  return enabled;
}

/* helper functions */
/**
 * @brief Split float into integer and decimal parts
 *
 * @param number float number
 * @param int_part integer part
 * @param dec_part decimal part
 */
void split_float(float number, String &int_part, String &dec_part) {
  // Convert float to string with 2 decimal places
  char float_str[10];
  dtostrf(number, 0, 2, float_str);

  String float_string = String(float_str);
  int dotIndex = float_string.indexOf('.');

  int_part = float_string.substring(0, dotIndex);
  dec_part = float_string.substring(dotIndex);

  if (dec_part.length() == 1) {
    dec_part += "0";
  }
}