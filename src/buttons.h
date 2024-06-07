/**
 * @file  buttons.h
 * @copyright Copyright (c) 2024
 */

#pragma once

#include <Arduino.h>
#include <Button2.h>
#include <TickTwo.h>
#include <U8g2lib.h>

class Buttons {
 public:
  Buttons();
  void init();
  void loop();
  void button_up_handler(Button2 &btn);
  void button_down_handler(Button2 &btn);
  void button_left_handler(Button2 &btn);
  void button_right_handler(Button2 &btn);
  void button_left_long_press_handler(Button2 &btn);
  int buf;

 private:
  Button2 button_up;
  Button2 button_down;
  Button2 button_left;
  Button2 button_right;
  TickTwo *button_ticker;
  void button_ticker_handler();
};