

#ifndef __DASHBOARD__H
#define __DASHBOARD__H

#include <Arduino.h>
#include <ESPDash.h>
#include <ESPmDNS.h>
#include <MycilaESPConnect.h>
#include <TickTwo.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "environment.h"
#include "fans.h"
#include "oled.h"
#include "webserver.h"

AsyncWebServer server(80);
ESPDash dashboard(&server, true, "/dash");
extern WebServer webserver;
bool oled_enabled = true;
uint32_t last_log = 0;

/*
  Dashboard Cards
  Format - (Dashboard Instance, Card Type, Card Name, Card Symbol(optional) )
*/
Card temperature(&dashboard, TEMPERATURE_CARD, "Temperature", "°C");
Card humidity(&dashboard, HUMIDITY_CARD, "Humidity", "%");
Card co2(&dashboard, AIR_CARD, "", "ppm");
Card reset_wifi_btn(&dashboard, PUSH_BUTTON_CARD, "Reset WiFi", "wifi");
Card button_test(&dashboard, BUTTON_CARD, "Toggle OLED", "test");
Card fan_speed(&dashboard, SLIDER_CARD, "Fan Speed", "speed");

/* helper functions prototypes */
void init_dashboard();
void update_dashboard();
void dashboard_ticker_handler();
void set_dashboard_callbacks();

/* Tickers */
TickTwo dashboard_ticker(dashboard_ticker_handler, 5000, 0, MILLIS);

/**
 * @brief Initializes the dashboard.
 */
void init_dashboard() {
  dashboard_ticker.start();
  set_dashboard_callbacks();
}

/**
 * @brief Dashboard update loop.
 */
void update_dashboard() {
  dashboard_ticker.update();
  reset_wifi_btn.update(true);
  button_test.update(oled_enabled);

  // if (millis() - last_log > 5000) {
  //   JsonDocument doc;
  //   ESPConnect.toJson(doc.to<JsonObject>());
  //   serializeJson(doc, Serial);
  //   Serial.println();
  //   last_log = millis();
  // }
}

/**
 * @brief Periodically updates the dashboard with the latest environment data.
 */
void dashboard_ticker_handler() {
  temperature.update(env.temperature);
  humidity.update(env.humidity);
  co2.update(env.co2);
  dashboard.sendUpdates();
}

/**
 * @brief Sets the dashboard callbacks for the buttons.
 */
void set_dashboard_callbacks() {
  reset_wifi_btn.attachCallback([&](bool value) {
    reset_wifi_btn.update(value);
    dashboard.sendUpdates();
    webserver.reset_wifi();
  });

  button_test.attachCallback([&](bool value) {
    button_test.update(value);
    dashboard.sendUpdates();
    Serial.println("Toggling OELD!\n");
    oled_enabled = !oled_enabled;
  });
}

#endif