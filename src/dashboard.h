

#ifndef __DASHBOARD__H
#define __DASHBOARD__H

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPDash.h>
#include <ESPmDNS.h>
#include <MycilaESPConnect.h>
#include <TickTwo.h>
#include <WiFi.h>
#include "environment.h"
#include "fans.h"
#include "oled.h"

void init_dashboard();
void update_dashboard();
void dashboard_ticker_handler();

#define HOSTNAME "smartvent"
#define AP_SSID "SmartVent AP"
#define AP_PASS "capstone"

AsyncWebServer server(80);
ESPDash dashboard(&server, true, "/dash");
// TickTwo dashboard_ticker(update_dashboard, 1000, 5);
TickTwo dashboard_ticker(dashboard_ticker_handler, 5000, 0, MILLIS);
uint32_t lastLog = 0;

bool oled_enabled = true;

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
void reset_wifi();

void init_dashboard() {
  // clear configuration
  server.on("/clear", HTTP_GET, [&](AsyncWebServerRequest* request) {
    Serial.println("Clearing configuration...");
    reset_wifi();
    request->send(200);
  });

  // add a rewrite which is only applicable in AP mode and STA mode, but not in Captive Portal mode
  server.rewrite("/", "/dash").setFilter([](AsyncWebServerRequest* request) { return ESPConnect.getState() != ESPConnectState::PORTAL_STARTED; });

  // network state listener is required here in async mode
  ESPConnect.listen([](ESPConnectState previous, ESPConnectState state) {
    JsonDocument doc;
    ESPConnect.toJson(doc.to<JsonObject>());
    serializeJsonPretty(doc, Serial);
    Serial.println();

    switch (state) {
      case ESPConnectState::NETWORK_CONNECTED:
      case ESPConnectState::AP_STARTED:
        server.begin();
        MDNS.begin(HOSTNAME);
        MDNS.addService("http", "tcp", 80);
        break;

      case ESPConnectState::NETWORK_DISCONNECTED:
        server.end();
      default:
        break;
    }
  });

  ESPConnect.setAutoRestart(false);
  ESPConnect.setBlocking(false);
  ESPConnect.setCaptivePortalTimeout(86400);
  ESPConnect.setConnectTimeout(10);

  Serial.println("====> Trying to connect to saved WiFi or will start portal in the background...");

  ESPConnect.begin(&server, HOSTNAME, AP_SSID, AP_PASS);

  //*************************************************************

  Serial.print("ESP MAC Address:  ");
  Serial.println(WiFi.macAddress());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  dashboard_ticker.start();

  reset_wifi_btn.attachCallback([&](bool value) {
    reset_wifi_btn.update(value);
    dashboard.sendUpdates();
    reset_wifi();
  });

  button_test.attachCallback([&](bool value) {
    button_test.update(value);
    dashboard.sendUpdates();
    Serial.println("Toggling OELD!\n");
    oled_enabled = !oled_enabled;
  });
}

void update_dashboard() {
  dashboard_ticker.update();
  reset_wifi_btn.update(true);
  button_test.update(oled_enabled);

  ESPConnect.loop();

  if (millis() - lastLog > 5000) {
    JsonDocument doc;
    ESPConnect.toJson(doc.to<JsonObject>());
    serializeJson(doc, Serial);
    Serial.println();
    lastLog = millis();
  }
}

void dashboard_ticker_handler() {
  temperature.update(env.temperature);
  humidity.update(env.humidity);
  co2.update(env.co2);
  dashboard.sendUpdates();

  Serial.println("Dashboard updated");
}

void reset_wifi() {
  ESPConnect.clearConfiguration();
  ESPConnect.end();
  ESPConnect.begin(&server, HOSTNAME, AP_SSID, AP_PASS);
}

#endif