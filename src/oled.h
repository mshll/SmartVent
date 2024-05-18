/**
 * @file  oled.h
 * @copyright Copyright (c) 2024
 */

#pragma once

#include <TickTwo.h>
#include <U8g2lib.h>

typedef enum {
  SPLASH_SCREEN = 0,
  MAIN_SCREEN = 1,
} Screen;

class OLED {
 public:
  OLED();
  void init();
  void loop();
  void display_main_screen();
  void display_splash_screen();
  void set_screen(Screen screen);
  void set_enabled(bool enabled);
  bool toggle();
  bool is_enabled();

 private:
  U8G2_SSD1306_128X64_NONAME_F_HW_I2C *u8g2;
  Screen current_screen;
  bool enabled;
  TickTwo *oled_ticker;
  void oled_ticker_callback();
};
