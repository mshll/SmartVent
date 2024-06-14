/**
 *    _____                      __     _    __           __
 *   / ___/____ ___  ____ ______/ /_   | |  / /__  ____  / /_
 *   \__ \/ __ `__ \/ __ `/ ___/ __/   | | / / _ \/ __ \/ __/
 *  ___/ / / / / / / /_/ / /  / /_     | |/ /  __/ / / / /_
 * /____/_/ /_/ /_/\__,_/_/   \__/     |___/\___/_/ /_/\__/
 *
 * @file    main.cpp
 * @author  Team 8:
 *            Abdulaziz Alateeqi,
 *            Meshal Almutairi,
 *            Fawzan Alfahad,
 *            Michael Fontaine,
 *            John Michael Mertz.
 * @brief   Smart Vent capstone project source code.
 * @copyright Copyright (c) 2024
 *
 */

#include "buttons.h"
#include "common.h"
#include "dashboard.h"
#include "fans.h"
#include "mhz19b.h"
#include "oled.h"
#include "preferences_manager.h"
#include "webserver.h"

/* global variables */
extern AsyncWebServer server;  // defined in dashboard.cpp
WebServer webserver(&server);
Dashboard dashboard;
MHZ19B mhz19b;
OLED oled;
Fans fans;
Buttons buttons;
PreferencesManager pref_manager("smartvent");

void core1_task(void* parameter);

void setup() {
  Serial.begin(9600);
  pref_manager.init();
  oled.init();
  webserver.init();
  mhz19b.init();
  fans.init();
  dashboard.init();
  buttons.init();
  xTaskCreatePinnedToCore(core1_task, "core1_task", 10000, NULL, 1, NULL, 1);
}

void loop() {
  webserver.loop();
  mhz19b.loop();
  dashboard.loop();
  fans.update(mhz19b.get_co2());
}

void core1_task(void* parameter) {
  for (;;) {
    // Run buttons task on core 1 to avoid conflicts with other tasks
    oled.loop();
    buttons.loop();
    vTaskDelay(10);
  }
}

/**
 * @brief Decrement a value with wrap-around.
 */
int decrement(int val, int size) {
  return (val + size - 1) % size;
}

/**
 * @brief Increment a value with wrap-around.
 */
int increment(int val, int size) {
  return (val + 1) % size;
}