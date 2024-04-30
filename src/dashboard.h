

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

#define HOSTNAME "smartvent"
#define AP_SSID "SmartVent AP"
#define AP_PASS "capstone"

typedef struct {
  String id;
  IPAddress ip;
  // String url;
  uint32_t last_heartbeat;
} Device;

AsyncWebServer server(80);
ESPDash dashboard(&server, true, "/dash");
uint32_t lastLog = 0;
bool oled_enabled = true;
bool is_connected = false;
WiFiUDP udp;
unsigned int udp_port = 4096;
String deviceID = String((uint32_t)ESP.getEfuseMac(), HEX);
String leaderID = "";
bool is_leader = false;
IPAddress multicastIP(226, 1, 1, 1);
String hostname = String(HOSTNAME) + "-" + deviceID;
std::vector<Device> devices;
unsigned int heartbeat_interval = 5000;
unsigned int heartbeat_timeout = 15000;
unsigned int check_devices_interval = 16000;

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
void reset_wifi();
void setup_server();
void set_dashboard_callbacks();
void setup_mdns();
void update_server();
void send_election_message();
void handle_incoming_packets();
void update_device(String id, bool leader);
void determine_leader();
void send_heartbeat();
void check_devices();

/* Tickers */
TickTwo dashboard_ticker(dashboard_ticker_handler, 5000, 0, MILLIS);
TickTwo heartbeat_ticker(send_heartbeat, heartbeat_interval);
TickTwo check_devices_ticker(check_devices, check_devices_interval);

/**
 * @brief Sends a heartbeat message to all devices in the network.
 */
void send_heartbeat() {
  String message = "HB:" + deviceID + ":" + (is_leader ? "1" : "0");
  Serial.println("---> Sending heartbeat: " + message);
  udp.beginMulticastPacket();
  udp.write((const uint8_t*)message.c_str(), message.length());
  udp.endPacket();
}

/**
 * @brief Sends an election message to all devices in the network.
 */
void send_election_message() {
  String message = "ELECT:" + deviceID;
  Serial.println("---> Sending election message: " + message);
  udp.beginMulticastPacket();
  udp.write((const uint8_t*)message.c_str(), message.length());
  udp.endPacket();
}

/**
 * @brief Handles incoming packets from the network.
 */
void handle_incoming_packets() {
  int packetSize = udp.parsePacket();
  if (packetSize) {  // If a packet is available
    char packet[255];
    int len = udp.read(packet, 255);
    if (len > 0) {
      packet[len] = 0;
    }
    String message = String(packet);
    Serial.println("Received packet: " + message);

    // Parse the message
    int sepertor = message.indexOf(':');
    int second_sepertor = message.indexOf(':', sepertor + 1);
    String msg_type = message.substring(0, sepertor);
    String sender_id = message.substring(sepertor + 1, second_sepertor);

    if (msg_type == "HB") {  // If the message is a heartbeat message, update the device's last heartbeat
      bool leader = message.substring(second_sepertor + 1) == "1";
      update_device(sender_id, leader);

    } else if (msg_type == "ELECT") {  // If the message is an election message, initiate an election for the leader
      if (deviceID < sender_id) {
        send_election_message();
      }
      update_device(sender_id, false);
      determine_leader();
    }
  }
}

/**
 * @brief Updates the device's last heartbeat and determines the leader.
 * @param id The device's ID.
 * @param leader Whether the device is the leader or not.
 */
void update_device(String id, bool leader) {
  bool found = false;
  for (auto& device : devices) {
    if (device.id == id) {
      device.last_heartbeat = millis();
      found = true;
      break;
    }
  }
  if (!found) {
    Device new_device;
    new_device.id = id;
    new_device.ip = udp.remoteIP();
    new_device.last_heartbeat = millis();
    devices.push_back(new_device);
    Serial.println("New device found: " + id);
  }

  if (leader || !found) {  // If the sender is the leader or the device is new
    determine_leader();
  }
}

/**
 * @brief Periodically checks the devices in the network and removes inactive devices.
 */
void check_devices() {
  uint32_t current_time = millis();
  bool leader_missing = false;

  for (int i = devices.size() - 1; i >= 0; i--) {
    if (current_time - devices[i].last_heartbeat > heartbeat_timeout) {
      Serial.println("Device " + devices[i].id + " is inactive.");
      if (devices[i].id == leaderID) {
        leader_missing = true;
      }
      devices.erase(devices.begin() + i);  // Remove inactive device
    }
  }

  // If the leader is missing, initiate leader election
  if (leader_missing) {
    send_election_message();
    determine_leader();
  }
  Serial.println("Devices count: " + String(devices.size()));
}

/**
 * @brief Determines the leader of the network based on the device IDs.
 */
void determine_leader() {
  bool was_leader = is_leader;
  is_leader = true;
  for (const auto& device : devices) {
    if (device.id > deviceID) {
      is_leader = false;
      leaderID = device.id;
      break;
    }
  }

  if (is_leader) {
    leaderID = deviceID;
    Serial.println("This device is now the leader.");

    if (!was_leader) {
      delay(500);
      setup_mdns();
    }
  } else {
    Serial.println("Leader is: " + leaderID);
    if (was_leader) {
      MDNS.end();
      delay(500);
      setup_mdns();
    }
  }
}

/**
 * @brief Server update loop.
 */
void update_server() {
  if (!is_connected) return;

  handle_incoming_packets();
  heartbeat_ticker.update();
  check_devices_ticker.update();
}

/**
 * @brief Sets up mDNS responder using the appropriate hostname depending on the device's role.
 */
void setup_mdns() {
  String hostname_str = is_leader ? HOSTNAME : hostname;

  if (MDNS.begin(hostname_str.c_str())) {
    MDNS.addService("http", "tcp", 80);
    Serial.println("mDNS responder started on " + hostname_str);

  } else {
    Serial.println("Error setting up MDNS responder!");
  }
}

/**
 * @brief Initializes the dashboard.
 */
void init_dashboard() {
  setup_server();

  // network state listener is required here in async mode
  ESPConnect.listen([](ESPConnectState previous, ESPConnectState state) {
    JsonDocument doc;
    ESPConnect.toJson(doc.to<JsonObject>());
    serializeJsonPretty(doc, Serial);
    Serial.println();

    is_connected = false;

    switch (state) {
      case ESPConnectState::NETWORK_CONNECTED:
        udp.beginMulticast(multicastIP, udp_port);
      case ESPConnectState::AP_STARTED:
        //*************************************************************
        is_connected = true;

        Serial.println("====> Network connected!!!!!!!!!!!!!!!!!!!!!!!!");
        send_election_message();
        delay(500);
        determine_leader();

        delay(500);
        setup_mdns();

        heartbeat_ticker.start();
        check_devices_ticker.start();

        server.begin();
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

/**
 * @brief Dashboard update loop.
 */
void update_dashboard() {
  dashboard_ticker.update();
  reset_wifi_btn.update(true);
  button_test.update(oled_enabled);

  ESPConnect.loop();

  if (millis() - lastLog > 5000) {
    // JsonDocument doc;
    // ESPConnect.toJson(doc.to<JsonObject>());
    // serializeJson(doc, Serial);
    // Serial.println();

    Serial.print("ID: ");
    Serial.println(deviceID);
    Serial.print("Leader: ");
    Serial.println(leaderID);
    Serial.print("Hostname: ");
    Serial.println(hostname);

    lastLog = millis();
  }
}

/**
 * @brief Periodically updates the dashboard with the latest environment data.
 */
void dashboard_ticker_handler() {
  temperature.update(env.temperature);
  humidity.update(env.humidity);
  co2.update(env.co2);
  dashboard.sendUpdates();

  Serial.println("Dashboard updated");
}

/**
 * @brief Resets the WiFi configuration.
 */
void reset_wifi() {
  ESPConnect.clearConfiguration();
  ESPConnect.end();
  ESPConnect.begin(&server, HOSTNAME, AP_SSID, AP_PASS);
}

/**
 * @brief Sets the dashboard callbacks for the buttons.
 */
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

/**
 * @brief Sets up the server.
 */
void setup_server() {
  // clear configuration
  server.on("/clear", HTTP_GET, [&](AsyncWebServerRequest* request) {
    Serial.println("Clearing configuration...");
    reset_wifi();
    request->send(200);
  });

  // add a rewrite which is only applicable in AP mode and STA mode, but not in Captive Portal mode
  server.rewrite("/", "/dash").setFilter([](AsyncWebServerRequest* request) { return ESPConnect.getState() != ESPConnectState::PORTAL_STARTED; });
}

#endif