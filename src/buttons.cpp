/**
 * @file  buttons.cpp
 * @copyright Copyright (c) 2024
 */

#include "buttons.h"
#include "common.h"
#include "oled.h"

void button_up_handler_wrapper(Button2 &btn);
void button_down_handler_wrapper(Button2 &btn);
void button_left_handler_wrapper(Button2 &btn);
void button_right_handler_wrapper(Button2 &btn);

extern OLED oled;

Buttons::Buttons() {
  button_ticker = new TickTwo(std::bind(&Buttons::button_ticker_handler, this), 1000);
}

void Buttons::init() {
  button_up.begin(BUTTON_UP_PIN);
  button_down.begin(BUTTON_DOWN_PIN);
  button_left.begin(BUTTON_LEFT_PIN);
  button_right.begin(BUTTON_RIGHT_PIN);

  button_up.setReleasedHandler(button_up_handler_wrapper);
  button_down.setReleasedHandler(button_down_handler_wrapper);
  button_left.setReleasedHandler(button_left_handler_wrapper);
  button_right.setReleasedHandler(button_right_handler_wrapper);

  button_ticker->start();
}

void Buttons::loop() {
  button_ticker->update();
  button_up.loop();
  button_down.loop();
  button_left.loop();
  button_right.loop();
}

void Buttons::button_up_handler(Button2 &btn) {
  Serial.println("Button up pressed");
  oled.main_screen_idle_ticker->start();
  if (oled.current_screen == MENU_SCREEN) {
    oled.curr_menu_item = (oled.curr_menu_item + oled.menu_items_count - 1) % oled.menu_items_count;
  }
}

void Buttons::button_down_handler(Button2 &btn) {
  Serial.println("Button down pressed");
  oled.main_screen_idle_ticker->start();
  if (oled.current_screen == MENU_SCREEN) {
    oled.curr_menu_item = (oled.curr_menu_item + 1) % oled.menu_items_count;
  }
}

void Buttons::button_left_handler(Button2 &btn) {
  Serial.println("Button left pressed");
  oled.main_screen_idle_ticker->start();
  if (oled.current_screen == MENU_SCREEN) {
    oled.current_screen = MAIN_SCREEN;
  }
}

void Buttons::button_right_handler(Button2 &btn) {
  Serial.println("Button right pressed");
  oled.main_screen_idle_ticker->start();
  if (oled.current_screen == MAIN_SCREEN) {
    oled.current_screen = MENU_SCREEN;
  }
}

void Buttons::button_ticker_handler() {
  // Serial.println("Button ticker");
}

// BUTTONS OBJECT (defined in main.cpp) //
extern Buttons buttons;

void button_up_handler_wrapper(Button2 &btn) {
  buttons.button_up_handler(btn);
}

void button_down_handler_wrapper(Button2 &btn) {
  buttons.button_down_handler(btn);
}

void button_left_handler_wrapper(Button2 &btn) {
  buttons.button_left_handler(btn);
}

void button_right_handler_wrapper(Button2 &btn) {
  buttons.button_right_handler(btn);
}