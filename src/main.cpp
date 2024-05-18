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

#include "common.h"
#include "dashboard.h"
#include "fans.h"
#include "mhz19b.h"
#include "oled.h"
#include "webserver.h"

/* global variables */
extern AsyncWebServer server;  // defined in dashboard.h
WebServer webserver(&server);
Dashboard dashboard;
MHZ19B mhz19b;
OLED oled;
Fans fans;

void setup() {
  Serial.begin(9600);
  oled.init();
  webserver.init();
  mhz19b.init();
  fans.init();
  dashboard.init();
}

void loop() {
  oled.loop();
  webserver.loop();
  mhz19b.loop();
  dashboard.loop();
  // update_buttons();
}
