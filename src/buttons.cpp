/**
 * @file  buttons.cpp
 * @copyright Copyright (c) 2024
 */

#include "buttons.h"
#include "common.h"
#include "fans.h"
#include "oled.h"
#include "webserver.h"

void button_up_handler_wrapper(Button2 &btn);
void button_down_handler_wrapper(Button2 &btn);
void button_left_handler_wrapper(Button2 &btn);
void button_right_handler_wrapper(Button2 &btn);
void button_left_long_press_handler_wrapper(Button2 &btn);
int buf = 0;

extern OLED oled;
extern MHZ19B mhz19b;
extern Fans fans;
extern WebServer webserver;

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

  button_left.setLongClickTime(3000);
  button_left.setLongClickDetectedHandler(button_left_long_press_handler_wrapper);

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
  if (!oled.is_enabled()) return;  // Ignore button press if OLED is disabled.
  oled.main_screen_idle_ticker->start();

  if (oled.current_screen == MENU_SCREEN) {
    oled.curr_menu_item = decrement(oled.curr_menu_item, oled.menu_items_count);
  }
  // Selected Menu Item
  if (oled.current_screen == MENU_ITEM_SCREEN) {
    if (oled.curr_menu_item == TOGGLE_TEMP_UNIT) {
      buf = decrement(buf, 2);
    }
    if (oled.curr_menu_item == FANS_OVERRIDE) {
      buf = decrement(buf, 5);
    }
  }
}

void Buttons::button_down_handler(Button2 &btn) {
  Serial.println("Button down pressed");
  if (!oled.is_enabled()) return;  // Ignore button press if OLED is disabled.
  oled.main_screen_idle_ticker->start();

  if (oled.current_screen == MENU_SCREEN) {
    oled.curr_menu_item = increment(oled.curr_menu_item, oled.menu_items_count);
  }
  // Modifying Selected Menu Item
  if (oled.current_screen == MENU_ITEM_SCREEN) {
    // Termperature Unit
    if (oled.curr_menu_item == TOGGLE_TEMP_UNIT) {
      buf = increment(buf, 2);
    }
    if (oled.curr_menu_item == FANS_OVERRIDE) {
      buf = increment(buf, 5);
    }
  }
}

void Buttons::button_left_handler(Button2 &btn) {
  Serial.println("Button left pressed");
  if (!oled.is_enabled()) return;  // Ignore button press if OLED is disabled.
  oled.main_screen_idle_ticker->start();

  if (oled.current_screen == MENU_SCREEN) {
    oled.current_screen = MAIN_SCREEN;
  }
  // Discard Changed to Items
  else if (oled.current_screen == MENU_ITEM_SCREEN) {
    if (oled.curr_menu_item == TOGGLE_TEMP_UNIT) {
      Serial.printf("Discarding Changes.\n");
    }
    if (oled.curr_menu_item == FANS_OVERRIDE) {
      fans.set_override(false);
    }
    if (oled.curr_menu_item == VIEW_WIFI_INFO) {
      Serial.printf("Closing WiFi Details\n");
    }
    if (oled.curr_menu_item == RESET_WIFI) {
      Serial.printf("Wifi reset Aborted!\n");
    }

    oled.current_screen = MENU_SCREEN;
  }
}

void Buttons::button_right_handler(Button2 &btn) {
  Serial.println("Button right pressed");
  if (!oled.is_enabled()) return;  // Ignore button press if OLED is disabled.
  oled.main_screen_idle_ticker->start();

  if (oled.current_screen == MAIN_SCREEN) {
    oled.current_screen = MENU_SCREEN;
  } else if (oled.current_screen == MENU_SCREEN) {
    oled.current_screen = MENU_ITEM_SCREEN;

    if (oled.curr_menu_item == TOGGLE_TEMP_UNIT) {
      buf = (int)mhz19b.get_unit();
    }
    if (oled.curr_menu_item == FANS_OVERRIDE) {
      buf = get_index_from_speed(fans.override_speed);
    }
    if (oled.curr_menu_item == VIEW_WIFI_INFO) {
      Serial.printf("Showing WiFi Details\n");
    }
    if (oled.curr_menu_item == RESET_WIFI) {
      Serial.printf("Reset Wifi?\n");
    }
  }
  // Saving Changed Items
  else if (oled.current_screen == MENU_ITEM_SCREEN) {
    if (oled.curr_menu_item == TOGGLE_TEMP_UNIT) {
      mhz19b.set_unit((buf % 2) == 1 ? FAHRENHEIT : CELSIUS);
      Serial.printf("Setting Temperature Unit to %s.\n", ((buf % 2) == 1 ? "Fahrenheit" : "Celsius"));
    }
    if (oled.curr_menu_item == FANS_OVERRIDE) {
      fans.set_override(true);  // Keep the Override on save.
      fans.override_speed = get_speed_from_index(buf);
      pref_manager.set_value("override_speed", buf);
    }
    if (oled.curr_menu_item == VIEW_WIFI_INFO) {
      Serial.printf("Closing WiFi Details\n");
    }
    if (oled.curr_menu_item == RESET_WIFI) {
      webserver.reset_wifi();
      Serial.printf("Wifi has been reset\n");
    }
    oled.current_screen = MAIN_SCREEN;
  }
}

void Buttons::button_left_long_press_handler(Button2 &btn) {
  Serial.println("Button left long press");
  if (oled.current_screen == MAIN_SCREEN) {
    oled.toggle();
  }
}

void Buttons::button_ticker_handler() {
  // Serial.println("Button ticker");
}

/* BUTTONS OBJECT (defined in main.cpp) & WRAPPERS FOR BUTTON HANDLERS */
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

void button_left_long_press_handler_wrapper(Button2 &btn) {
  buttons.button_left_long_press_handler(btn);
}
