

#ifndef __DASHBOARD__H
#define __DASHBOARD__H

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPDash.h>
#include <ESPmDNS.h>
#include <MycilaESPConnect.h>
#include <TickTwo.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "environment.h"
#include "fans.h"
#include "oled.h"

void dashboard_ticker_handler();
void udp_listener();
void broadcast_mac();

#define HOSTNAME "smartvent"
#define AP_SSID "SmartVent AP"
#define AP_PASS "capstone"

AsyncWebServer server(80);
ESPDash dashboard(&server, true, "/dash");
// TickTwo dashboard_ticker(update_dashboard, 1000, 5);
TickTwo dashboard_ticker(dashboard_ticker_handler, 5000, 0, MILLIS);
uint32_t lastLog = 0;

bool oled_enabled = true;
String hostname = HOSTNAME;
String mac = WiFi.macAddress();
String lowest_mac = mac;
bool is_host = true;
unsigned long long uptime = 0;

WiFiUDP udp;
unsigned int local_port = 4210;
const char* broadcast_address = "255.255.255.255";
TickTwo broadcast_ticker(broadcast_mac, 1000, 0, MILLIS);
TickTwo listener_ticker(udp_listener, 1000, 0, MILLIS);

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
void reset_wifi();
void setup_server();
void set_dashboard_callbacks();
void setup_mdns(bool is_host);
void check_host();

void udp_listener() {
  // Serial.println("Listening for UDP packets...");
  int packetSize = udp.parsePacket();
  if (packetSize) {
    char packet[255];
    int len = udp.read(packet, 255);
    if (len > 0) {
      packet[len] = 0;
    }
    String received_mac = String(packet);

    if (received_mac < lowest_mac) {
      lowest_mac = received_mac;
      is_host = (mac == lowest_mac);
      Serial.println("LOWEST MAC: " + lowest_mac);
    }

    Serial.println("=====================================");
    Serial.println("Received packet: " + String(packet));
    Serial.println("=====================================");

    if (is_host) Serial.println("I am the host!");
  }
}

void broadcast_mac() {
  udp_listener();
  // Serial.println("Broadcasting MAC address...");
  udp.beginPacket(IPAddress(255, 255, 255, 255), local_port);
  udp.write((const uint8_t*)mac.c_str(), mac.length());
  udp.endPacket();
}

void setup_mdns(bool is_host) {
  hostname = HOSTNAME;
  String short_mac = mac.substring(9);
  short_mac.replace(":", "");
  if (!is_host) {
    hostname += "-" + short_mac;
  }

  if (MDNS.begin(hostname)) {
    Serial.println("mDNS responder started");

    // Remove existing services to avoid confusion
    MDNS.addService("http", "tcp", 80);

  } else {
    Serial.println("Error setting up MDNS responder!");
  }
}

void check_host() {
  broadcast_ticker.update();
  listener_ticker.update();

  static bool was_host = is_host;
  if (was_host != is_host) {
    MDNS.end();
    setup_mdns(is_host);
    was_host = is_host;
    Serial.println("Host status changed, new hostname: " + hostname);
  }
}

void init_dashboard() {
  setup_server();

  // network state listener is required here in async mode
  ESPConnect.listen([](ESPConnectState previous, ESPConnectState state) {
    JsonDocument doc;
    ESPConnect.toJson(doc.to<JsonObject>());
    serializeJsonPretty(doc, Serial);
    Serial.println();

    mac = WiFi.macAddress();

    switch (state) {
      case ESPConnectState::NETWORK_CONNECTED:
      case ESPConnectState::AP_STARTED:
        //*************************************************************
        server.begin();

        udp.begin(local_port);
        delay(1000);
        broadcast_mac();
        udp_listener();
        broadcast_ticker.start();
        listener_ticker.start();

        setup_mdns(is_host);

        dashboard_ticker.start();
        set_dashboard_callbacks();
        break;
        //*************************************************************

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

  Serial.print("ESP MAC Address:  ");
  Serial.println(WiFi.macAddress());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
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

    Serial.print("MAC: ");
    Serial.println(mac);
    Serial.print("Host: ");
    Serial.println(is_host);
    Serial.print("Hostname: ");
    Serial.println(hostname);
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

void set_dashboard_callbacks() {
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

void setup_server() {
  // clear configuration
  server.on("/clear", HTTP_GET, [&](AsyncWebServerRequest* request) {
    Serial.println("Clearing configuration...");
    reset_wifi();
    request->send(200);
  });

  // add a rewrite which is only applicable in AP mode and STA mode, but not in Captive Portal mode
  // server.rewrite("/", "/dash").setFilter([](AsyncWebServerRequest* request) { return ESPConnect.getState() != ESPConnectState::PORTAL_STARTED; });
}

#endif