/**
 * @file  dashboard.h
 * @copyright Copyright (c) 2024
 */

#pragma once

#include <Arduino.h>
#include <ESPDash.h>
#include <ESPmDNS.h>
#include <MycilaESPConnect.h>
#include <TickTwo.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "environment.h"
#include "fans.h"
#include "mhz19b.h"
#include "oled.h"
#include "webserver.h"

#define LOG_SIZE 100
#define LOG_INTERVAL 10000
#define RETRY_INTERVAL 3000
#define DASH_REFRESH_INTERVAL 2000

void log_data();

AsyncWebServer server(80);
ESPDash dashboard(&server, true, "/dash");
extern WebServer webserver;
extern MHZ19B mhz19b;
bool oled_enabled = true;
uint32_t last_log = 0;

TickTwo data_log_ticker(log_data, LOG_INTERVAL);
std::vector<String> log_timestamps;
std::vector<float> log_temperatures;
std::vector<float> log_humidity;
std::vector<float> log_co2;

Card temperature(&dashboard, TEMPERATURE_CARD, "Temperature", "°C");
Card humidity(&dashboard, HUMIDITY_CARD, "Humidity", "%");
Card co2(&dashboard, AIR_CARD, "CO2", "ppm");
Card air_quality(&dashboard, STATUS_CARD, "Air Quality", "idle");
Card reset_wifi_btn(&dashboard, PUSH_BUTTON_CARD, "Reset Wi-Fi Configuration", "Settings / Wi-Fi");
Card enable_oled_btn(&dashboard, BUTTON_CARD, "Enable Physical Display", "Settings / Display");
Card fan_speed(&dashboard, GENERIC_CARD, "Fans Status", "");
Card fans_override_btn(&dashboard, BUTTON_CARD, "Force Fans Off", "Settings / Fans");
Chart co2_chart(&dashboard, BAR_CHART, "CO2 History");
Chart temperature_chart(&dashboard, BAR_CHART, "Temperature History");

/* helper functions prototypes */
void init_dashboard();
void update_dashboard();
void dashboard_ticker_handler();
void set_dashboard_callbacks();
void update_air_quality_card(float co2);
void update_charts();

/* Tickers */
TickTwo dashboard_ticker(dashboard_ticker_handler, DASH_REFRESH_INTERVAL, 0, MILLIS);

/**
 * @brief Initializes the dashboard.
 */
void init_dashboard() {
  dashboard_ticker.start();
  data_log_ticker.start();
  set_dashboard_callbacks();
}

/**
 * @brief Dashboard update loop.
 */
void update_dashboard() {
  dashboard_ticker.update();
  data_log_ticker.update();
  reset_wifi_btn.update(true);
  enable_oled_btn.update(oled_enabled);
  fans_override_btn.update(fans_override);

  if (millis() - last_log > 5000) {
    // JsonDocument doc;
    // ESPConnect.toJson(doc.to<JsonObject>());
    // serializeJson(doc, Serial);
    // Serial.println();

    mhz19b.set_unit(mhz19b.get_unit() == CELSIUS ? FAHRENHEIT : CELSIUS);

    last_log = millis();
  }
}

/**
 * @brief Periodically updates the dashboard with the latest environment data.
 */
void dashboard_ticker_handler() {
  temperature.update(mhz19b.get_temperature(), mhz19b.get_unit() == FAHRENHEIT ? FAHRENHEIT_SYMBOL : CELSIUS_SYMBOL);
  humidity.update(0);
  fan_speed.update(get_fan_speed_str(0));
  co2.update(mhz19b.get_co2());
  update_air_quality_card(mhz19b.get_co2());
  dashboard.updateDevices(webserver.serialize_devices());
  dashboard.updateStats(webserver.serialize_stats());
  update_charts();
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

  fans_override_btn.attachCallback([&](bool value) {
    fans_override_btn.update(value);
    dashboard.sendUpdates();
    fans_override = !fans_override;
    dashboard_ticker_handler();
  });
}

/**
 * @brief Updates the air quality card based on the CO2 level.
 *
 * @param co2 The CO2 level.
 */
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

/**
 * @brief Logs the environment data for visualization.
 */
void log_data() {
  // If the interval is set to RETRY_INTERVAL, set it back to LOG_INTERVAL
  if (data_log_ticker.interval() == RETRY_INTERVAL) {
    data_log_ticker.interval(LOG_INTERVAL);
  }

  String timestamp = webserver.get_time("%H:%M");
  int co2 = mhz19b.get_co2();
  float temperature = mhz19b.get_temperature();

  if (timestamp == "" || isnan(temperature) || isnan(co2)) {
    data_log_ticker.interval(RETRY_INTERVAL);
    Serial.println("Failed to log data. Retrying shortly...");
    return;
  }

  log_timestamps.push_back(timestamp);
  log_temperatures.push_back(temperature);
  // log_humidity.push_back(humidity);
  log_co2.push_back(co2);
  Serial.println("Logged data: " + timestamp + ", " + String(temperature) + ", " + String(co2));

  // Keep the log size to a maximum of LOG_SIZE
  if (log_temperatures.size() > LOG_SIZE) {
    log_temperatures.erase(log_temperatures.begin());
    log_humidity.erase(log_humidity.begin());
    log_timestamps.erase(log_timestamps.begin());
    log_co2.erase(log_co2.begin());
  }
}

/**
 * @brief Updates the charts with the latest data.
 */
void update_charts() {
  temperature_chart.updateX(log_timestamps);
  temperature_chart.updateY(log_temperatures);
  co2_chart.updateX(log_timestamps);
  co2_chart.updateY(log_co2);
}