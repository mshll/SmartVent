/**
 * @file  espdash.cpp
 * @copyright Copyright (c) 2024
 */

#include "common.h"
#include "espdash.h"
#include "fans.h"
#include "oled.h"
#include "webserver.h"

// #define DEBUG

#define DASH_REFRESH_INTERVAL 2000
#define LOG_SIZE 144
#ifndef DEBUG
#define LOG_INTERVAL 600000         // 10 minutes
#define RETRY_INTERVAL 5000         // 5 seconds
#define TIME_ADJUST_INTERVAL 30000  // 30 seconds
#else
#define LOG_INTERVAL 1000          // 1 second
#define RETRY_INTERVAL 1000        // 1 second
#define TIME_ADJUST_INTERVAL 1000  // 1 second
#endif

AsyncWebServer server(80);
extern WebServer webserver;
extern MHZ19B mhz19b;
extern OLED oled;
extern Fans fans;
ESPDash espdash(&server, true, "/dash");

Card temperature_card(&espdash, TEMPERATURE_CARD, "Temperature", "°C");
Card temperature_unit_btn(&espdash, BUTTON_CARD, "Toggle Temperature Unit", "Settings / Temperature (°C / °F)");
Card co2_card(&espdash, AIR_CARD, "CO2", "ppm");
Card air_quality_card(&espdash, STATUS_CARD, "Air Quality", "idle");
Card reset_wifi_btn(&espdash, PUSH_BUTTON_CARD, "Reset Wi-Fi Configuration", "Settings / Wi-Fi");
Card enable_oled_btn(&espdash, BUTTON_CARD, "Enable Physical Display", "Settings / Display");
Card fan_speed_card(&espdash, GENERIC_CARD, "Fans Status", "");
Card fans_override_btn(&espdash, BUTTON_CARD, "Force Fans Off", "Settings / Fans");
Chart co2_chart(&espdash, BAR_CHART, "CO2 History");
Chart temperature_chart(&espdash, BAR_CHART, "Temperature History");

Dashboard::Dashboard() {
  // dashboard = new ESPDash(&server, enable_stats, path);
  last_log = 0;
  dashboard_ticker = new TickTwo(std::bind(&Dashboard::dashboard_ticker_handler, this), DASH_REFRESH_INTERVAL, 0, MILLIS);
  data_log_ticker = new TickTwo(std::bind(&Dashboard::log_data, this), LOG_INTERVAL);
}

void Dashboard::init() {
  dashboard_ticker->start();
  data_log_ticker->start();
  set_callbacks();
}

void Dashboard::loop() {
  dashboard_ticker->update();
  data_log_ticker->update();
  reset_wifi_btn.update(true);
  enable_oled_btn.update(oled.is_enabled());
  fans_override_btn.update(fans.get_override());
  temperature_unit_btn.update(mhz19b.get_unit() == FAHRENHEIT);

  if (millis() - last_log > 5000) {
    // JsonDocument doc;
    // ESPConnect.toJson(doc.to<JsonObject>());
    // serializeJson(doc, Serial);
    // Serial.println();

    // mhz19b.set_unit(mhz19b.get_unit() == CELSIUS ? FAHRENHEIT : CELSIUS);

    last_log = millis();
  }
}

void Dashboard::update_air_quality_card(int co2) {
  if (co2 < 1000) {
    air_quality_card.update("Good", "success");
  } else if (co2 <= 1400) {
    air_quality_card.update("Fair", "warning");
  } else if (co2 > 1400) {
    air_quality_card.update("Unhealthy", "danger");
  } else {
    air_quality_card.update("Unknown", "idle");
  }
}

void Dashboard::set_callbacks() {
  reset_wifi_btn.attachCallback([&](bool value) {
    reset_wifi_btn.update(value);
    espdash.sendUpdates();
    webserver.reset_wifi();
  });

  enable_oled_btn.attachCallback([&](bool value) {
    enable_oled_btn.update(value);
    Serial.println("Toggling OELD!\n");
    oled.toggle();
    espdash.sendUpdates();
  });

  fans_override_btn.attachCallback([&](bool value) {
    fans_override_btn.update(value);
    fans.toggle_override();
    dashboard_ticker_handler();
    espdash.sendUpdates();
  });

  temperature_unit_btn.attachCallback([&](bool value) {
    temperature_unit_btn.update(value);
    mhz19b.set_unit(mhz19b.get_unit() == CELSIUS ? FAHRENHEIT : CELSIUS);
    espdash.sendUpdates();
  });
}

void Dashboard::dashboard_ticker_handler() {
  temperature_card.update(mhz19b.get_temperature(), mhz19b.get_unit() == FAHRENHEIT ? FAHRENHEIT_SYMBOL : CELSIUS_SYMBOL);
  fan_speed_card.update(fans.get_speed(0));
  co2_card.update(mhz19b.get_co2());
  update_air_quality_card(mhz19b.get_co2());
  espdash.updateDevices(webserver.serialize_devices());
  espdash.updateStats(webserver.serialize_stats());
  update_charts();
  espdash.sendUpdates();
}

void Dashboard::log_data() {
#ifndef DEBUG
  // Adjust the interval to log data exactly at 10-minute intervals
  if (webserver.get_time("%M").toInt() % 10 != 0) {
    data_log_ticker->interval(TIME_ADJUST_INTERVAL);
    return;
  }
#endif

  // Reset the interval back to the default
  if (data_log_ticker->interval() != LOG_INTERVAL) {
    data_log_ticker->interval(LOG_INTERVAL);
  }

  String timestamp = webserver.get_time("%Y-%m-%dT%H:%M:%S");
  int co2 = mhz19b.get_co2();
  float temperature_c = mhz19b.get_temperature(CELSIUS);
  float temperature_f = mhz19b.get_temperature(FAHRENHEIT);

  if (timestamp == "" || isnan(temperature_c) || isnan(temperature_f) || isnan(co2) || co2 < 100) {
    data_log_ticker->interval(RETRY_INTERVAL);
    Serial.println("Failed to log data. Retrying shortly...");
    return;
  }

  data_log.timestamps.push_back(timestamp);
  data_log.temperatures_c.push_back(temperature_c);
  data_log.temperatures_f.push_back(temperature_f);
  data_log.co2.push_back(co2);

  // Keep the log size to a maximum of LOG_SIZE
  if (data_log.co2.size() > LOG_SIZE) {
    data_log.co2.erase(data_log.co2.begin());
    data_log.timestamps.erase(data_log.timestamps.begin());
    data_log.temperatures_c.erase(data_log.temperatures_c.begin());
    data_log.temperatures_f.erase(data_log.temperatures_f.begin());
  }
}

void Dashboard::update_charts() {
  temperature_chart.updateX(data_log.timestamps);
  temperature_chart.updateY(mhz19b.get_unit() == FAHRENHEIT ? data_log.temperatures_f : data_log.temperatures_c);
  co2_chart.updateX(data_log.timestamps);
  co2_chart.updateY(data_log.co2);
}