

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
Card air_quality(&dashboard, STATUS_CARD, "Air Quality", "idle");
Card reset_wifi_btn(&dashboard, PUSH_BUTTON_CARD, "Reset Wi-Fi Configuration", "Settings / Wi-Fi");
Card enable_oled_btn(&dashboard, BUTTON_CARD, "Enable Physical Display", "Settings / Display");
Card fan_speed(&dashboard, GENERIC_CARD, "Fans Status", "");

/* helper functions prototypes */
void init_dashboard();
void update_dashboard();
void dashboard_ticker_handler();
void set_dashboard_callbacks();
void update_air_quality_card(float co2);

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
  enable_oled_btn.update(oled_enabled);

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
  fan_speed.update(get_fan_speed_str(0));
  co2.update(env.co2);
  update_air_quality_card(env.co2);
  dashboard.updateDevices(webserver.serialize_devices());
  dashboard.updateStats(webserver.serialize_stats());
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

  enable_oled_btn.attachCallback([&](bool value) {
    enable_oled_btn.update(value);
    dashboard.sendUpdates();
    Serial.println("Toggling OELD!\n");
    oled_enabled = !oled_enabled;
  });
}

void update_air_quality_card(float co2) {
  if (co2 < 1000) {
    air_quality.update("Good", "success");
  } else if (co2 <= 1400) {
    air_quality.update("Average", "warning");
  } else if (co2 > 1400) {
    air_quality.update("Unhealthy", "danger");
  } else {
    air_quality.update("Unknown", "idle");
  }
}

#endif