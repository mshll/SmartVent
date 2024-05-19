/**
 * @file  oled.cpp
 * @copyright Copyright (c) 2024
 */

#include "oled.h"
#include "common.h"
#include "fans.h"
#include "mhz19b.h"

#define SPLASH_SCREEN_TIMEOUT 3000
#define LCDWidth u8g2->getDisplayWidth()
#define ALIGN_CENTER(t) ((LCDWidth - (u8g2->getUTF8Width(t))) / 2)
#define ALIGN_RIGHT(t) (LCDWidth - u8g2->getUTF8Width(t))
#define ALIGN_LEFT 0

static const unsigned char image_co2_bits[] U8X8_PROGMEM = {0x3c, 0x1c, 0x00, 0x66, 0x36, 0x00, 0x43, 0x63, 0x00, 0x03, 0x63, 0x02,
                                                            0x03, 0x63, 0x05, 0x43, 0x63, 0x04, 0x66, 0x36, 0x02, 0x3c, 0x1c, 0x07};
static const unsigned char image_temperature_bits[] U8X8_PROGMEM = {0x70, 0x00, 0x88, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8,
                                                                    0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0x24, 0x01, 0x72, 0x02,
                                                                    0xea, 0x02, 0xfa, 0x02, 0x72, 0x02, 0x04, 0x01, 0xf8, 0x00};
static const unsigned char image_fan_icon_bits[] U8X8_PROGMEM = {0xe0, 0x03, 0x98, 0x0c, 0x44, 0x11, 0x22, 0x22, 0x22, 0x22, 0x39,
                                                                 0x4e, 0x45, 0x51, 0x83, 0x60, 0x45, 0x51, 0x39, 0x4e, 0x22, 0x22,
                                                                 0x22, 0x22, 0x44, 0x11, 0x98, 0x0c, 0xe0, 0x03, 0x00, 0x00};
static const unsigned char image_wifi_on_bits[] U8X8_PROGMEM = {0xfe, 0x00, 0x01, 0x01, 0x7c, 0x00, 0x82, 0x00, 0x38, 0x00, 0x00, 0x00, 0x10, 0x00};
static const unsigned char image_wifi_off_bits[] U8X8_PROGMEM = {0xd6, 0x00, 0x11, 0x01, 0x54, 0x00, 0x92, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00};

extern MHZ19B mhz19b;
extern Fans fans;

/* helper functions prototypes */
void split_float(float number, String &int_part, String &dec_part);

OLED::OLED() {
  u8g2 = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE, OLED_SCL, OLED_SDA);
  current_screen = SPLASH_SCREEN;
  enabled = true;
  splash_screen_ticker = new TickTwo(std::bind(&OLED::splash_screen_ticker_callback, this), SPLASH_SCREEN_TIMEOUT, 1);
}

void OLED::init() {
  u8g2->begin();
  u8g2->clearBuffer();
  u8g2->setFont(u8g2_font_ncenB14_tr);

  splash_screen_ticker->start();
}

void OLED::loop() {
  u8g2->clearBuffer();

  if (enabled) {
    switch (current_screen) {
      case SPLASH_SCREEN:
        display_splash_screen();
        break;
      case MAIN_SCREEN:
        display_main_screen();
        break;
    }
  }

  u8g2->sendBuffer();
  splash_screen_ticker->update();
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
  const char *air_quality = mhz19b.get_air_quality();

  u8g2->setFontMode(1);
  u8g2->setBitmapMode(1);
  u8g2->drawXBM(118, 1, 9, 7, ESPConnect.getState() == ESPConnectState::NETWORK_CONNECTED ? image_wifi_on_bits : image_wifi_off_bits);
  u8g2->setFont(u8g2_font_profont29_tr);
  // u8g2_int_t co2_x = ALIGN_CENTER(String(co2).c_str()) - 10;
  u8g2_int_t co2_x = 10;
  u8g2_int_t co2_right_end = co2_x + u8g2->getUTF8Width(String(co2).c_str()) + 5;
  u8g2->drawStr(co2_x, 39, String(co2).c_str());
  u8g2->setFont(u8g2_font_6x10_tr);
  u8g2->drawStr(co2_right_end, 37, "ppm");
  u8g2->drawXBM(co2_right_end, 21, 19, 8, image_co2_bits);
  u8g2->setFont(u8g2_font_profont17_tr);
  // u8g2->drawStr(ALIGN_CENTER(air_quality), 14, air_quality);
  u8g2->drawStr(10, 14, air_quality);
  u8g2->setDrawColor(2);
  u8g2->drawBox(9, 2, u8g2->getUTF8Width(air_quality) + 2, 13);
  u8g2->setDrawColor(1);
  u8g2->drawXBM(9, 46, 11, 16, image_temperature_bits);
  u8g2->setFont(u8g2_font_profont22_tr);
  u8g2->drawStr(21, 61, temp_int.c_str());
  u8g2->setFont(u8g2_font_profont10_tr);
  u8g2->drawStr(43, 61, temp_dec.c_str());
  u8g2->drawEllipse(49, 48, 1, 1);
  u8g2->drawStr(53, 53, mhz19b.get_unit() == CELSIUS ? "C" : "F");
  u8g2->setFont(u8g2_font_timR14_tr);
  const char *fan_speed = fans.get_speed(1);
  u8g2->drawStr(ALIGN_RIGHT(fan_speed) - 10, 60, fan_speed);
  u8g2_int_t fan_speed_left_end = ALIGN_RIGHT(fan_speed) - 10 - 15 - 3;
  // u8g2->drawXBM(66, 46, 15, 16, image_fan_icon_bits);
  u8g2->drawXBM(fan_speed_left_end, 46, 15, 16, image_fan_icon_bits);
}

void OLED::display_splash_screen() {
  u8g2->setFontMode(1);
  u8g2->setBitmapMode(1);
  u8g2->setFont(u8g2_font_timR18_tf);
  u8g2->drawStr(ALIGN_CENTER("SmartVent"), 46, "SmartVent");
  u8g2->setFont(u8g2_font_helvB08_tf);
  u8g2->drawStr(ALIGN_CENTER("LaunchPilot"), 22, "LaunchPilot");
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

void OLED::splash_screen_ticker_callback() {
  if (current_screen == SPLASH_SCREEN) current_screen = MAIN_SCREEN;
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