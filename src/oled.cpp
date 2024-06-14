/**
 * @file  oled.cpp
 * @copyright Copyright (c) 2024
 */

#include "oled.h"
#include "WiFi.h"
#include "WiFiSTA.h"
#include "buttons.h"
#include "common.h"
#include "fans.h"
#include "mhz19b.h"
#include "webserver.h"

#define SCREEN_IDLE_TIMEOUT 30000
#define LCDWidth u8g2->getDisplayWidth()
#define ALIGN_CENTER(t) ((LCDWidth - (u8g2->getUTF8Width(t))) / 2)
#define ALIGN_RIGHT(t) (LCDWidth - u8g2->getUTF8Width(t))
#define ALIGN_LEFT 0

extern MHZ19B mhz19b;
extern Fans fans;
extern Buttons buttons;
extern WebServer webserver;

/* helper functions prototypes */
void split_float(float number, String &int_part, String &dec_part);

OLED::OLED() {
  u8g2 = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE, OLED_SCL, OLED_SDA);
  current_screen = SPLASH_SCREEN;
  enabled = true;
  main_screen_idle_ticker = new TickTwo(std::bind(&OLED::screen_idle_ticker_callback, this), SPLASH_SCREEN_DUR, 1);
}

void OLED::init() {
  u8g2->begin();
  u8g2->clearBuffer();
  u8g2->setFont(u8g2_font_ncenB14_tr);

  main_screen_idle_ticker->start();
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
      case MENU_SCREEN:
        display_menu_screen();
        break;
      case MENU_ITEM_SCREEN:
        display_menu_item_screen();
    }
  }

  u8g2->sendBuffer();
  main_screen_idle_ticker->update();
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
  u8g2->drawStr(23, 61, temp_int.c_str());
  u8g2->setFont(u8g2_font_profont10_tr);
  u8g2->drawStr(45, 61, temp_dec.c_str());
  u8g2->drawEllipse(51, 48, 1, 1);
  u8g2->drawStr(55, 53, mhz19b.get_unit() == CELSIUS ? "C" : "F");
  u8g2->setFont(u8g2_font_profont22_tr);
  const char *fan_speed = fans.get_speed(1);
  u8g2->drawStr(ALIGN_RIGHT(fan_speed) - 1, 60, fan_speed);
  u8g2_int_t fan_speed_left_end = ALIGN_RIGHT(fan_speed) - 20;
  // u8g2->drawXBM(66, 46, 15, 16, image_fan_icon_bits);
  u8g2->drawXBM(fan_speed_left_end, 46, 15, 16, image_fan_icon_bits);
  u8g2->setFont(u8g2_font_4x6_tr);
  if (fans.get_override()) {
    u8g2->setDrawColor(2);
    u8g2->drawStr(105, 7, "OV");
    u8g2->drawBox(104, 1, 9, 7);
  }
}

void OLED::display_menu_screen() {
  // selected item background
  u8g2->drawXBMP(0, 22, 128, 21, bitmap_item_sel_outline);

  int prev_menu_item = decrement(curr_menu_item, menu_items_count);
  int next_menu_item = increment(curr_menu_item, menu_items_count);

  // draw previous item as icon + label
  u8g2->setFont(u8g_font_7x14);
  u8g2->drawStr(5, 15, menu_items[prev_menu_item]);

  // draw selected item as icon + label in bold font
  u8g2->setFont(u8g_font_7x14B);
  u8g2->drawStr(5, 15 + 20 + 2, menu_items[curr_menu_item]);

  // draw next item as icon + label
  u8g2->setFont(u8g_font_7x14);
  u8g2->drawStr(5, 15 + 20 + 20 + 2 + 2, menu_items[next_menu_item]);

  // draw scrollbar background
  u8g2->drawXBMP(128 - 8, 0, 8, 64, bitmap_scrollbar_background);

  // draw scrollbar handle
  u8g2->drawBox(125, 64 / menu_items_count * curr_menu_item, 3, 64 / menu_items_count);
}

void OLED::display_menu_item_screen() {
  u8g2->drawFrame(1, 1, 127, 18);  // draw selected menu item top frame
  // draw selected menu item as header
  u8g2->setFont(u8g_font_7x14B);
  String menu_item = menu_items[curr_menu_item];
  menu_item.replace("View ", "");  // remove "View " from menu item (View Wi-Fi Info -> Wi-Fi Info)
  u8g2->drawStr(ALIGN_CENTER(menu_item.c_str()), 15, menu_item.c_str());
  u8g2->setFont(u8g_font_7x14);

  if (curr_menu_item == TOGGLE_TEMP_UNIT) {
    const char *cur_unit = mhz19b.get_unit() == CELSIUS ? "Celcius" : "Farenheit";
    const char *other_unit = mhz19b.get_unit() == CELSIUS ? "Farenheit" : "Celcius";

    u8g2->drawStr(ALIGN_CENTER(cur_unit), 38, cur_unit);
    u8g2->setFont(u8g_font_7x14B);
    u8g2->drawStr(ALIGN_CENTER(other_unit), 53, other_unit);
    u8g2->drawFrame(18, 39, 92, 18);
  }

  if (curr_menu_item == FANS_OVERRIDE) {
    u8g2->setFont(u8g_font_7x14);
    const char *override_fan_speed;
    override_fan_speed = fans.get_speed(0, 1, get_speed_from_index(decrement(buttons.buf, 5)));
    u8g2->drawStr(ALIGN_CENTER(override_fan_speed), 32, (override_fan_speed));

    override_fan_speed = fans.get_speed(0, 1, get_speed_from_index(buttons.buf + 1));
    u8g2->drawStr(ALIGN_CENTER(override_fan_speed), 60, override_fan_speed);

    u8g2->setFont(u8g_font_7x14B);
    override_fan_speed = fans.get_speed(0, 1, get_speed_from_index(buttons.buf));
    u8g2->drawStr(ALIGN_CENTER(override_fan_speed), 46, override_fan_speed);
    u8g2->drawFrame(18, 34, 92, 15);
  }

  if (curr_menu_item == VIEW_WIFI_INFO) {
    if (ESPConnect.getState() == ESPConnectState::NETWORK_CONNECTED) {
      u8g2->setFont(u8g_font_7x14);
      const String ssid = WiFi.SSID();
      const String dashboard_url = webserver.get_hostname() + ".local";
      const String ip = webserver.get_device_ip();
      u8g2->drawStr(5, 32, ("SSID: " + ssid).c_str());
      u8g2->drawStr(5, 46, ("IP: " + ip).c_str());
      u8g2->drawStr(5, 60, (dashboard_url).c_str());
    } else {
      u8g2->clearBuffer();
      u8g2->setFont(u8g_font_5x8);
      u8g2->drawStr(0, 10, "Connect to");
      u8g2->drawStr(0, 22, String(AP_SSID).c_str());
      u8g2->drawStr(0, 34, ("PWD:" + String(AP_PASS)).c_str());
      u8g2->drawStr(0, 46, "to set Wi-Fi");
      u8g2->drawStr(0, 61, "OR SCAN QR >");
      u8g2->drawXBM(64, 0, 64, 64, smartvent_ap_qr_code);
    }
  }

  if (curr_menu_item == RESET_WIFI) {
    u8g2->setFont(u8g_font_7x14);
    u8g2->drawStr(ALIGN_CENTER("Are you sure you"), 32, "Are you sure you");
    u8g2->drawStr(ALIGN_CENTER("want to reset"), 46, "want to reset");
    u8g2->drawStr(ALIGN_CENTER("Wi-Fi settings?"), 60, "Wi-Fi settings?");
  }
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

void OLED::screen_idle_ticker_callback() {
  main_screen_idle_ticker->interval(SCREEN_IDLE_TIMEOUT);
  if (current_screen != MAIN_SCREEN) current_screen = MAIN_SCREEN;
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