/**
 * @file  webserver.h
 * @copyright Copyright (c) 2024
 */

#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__

#include <Arduino.h>
#include <ESPDash.h>
#include <ESPmDNS.h>
#include <MycilaESPConnect.h>
#include <TickTwo.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#define HOSTNAME "smartvent"
#define AP_SSID "SmartVent AP"
#define AP_PASS "capstone"

typedef struct {
  String id;
  IPAddress ip;
  uint32_t last_heartbeat;
} DeviceInfo;

class WebServer {
 public:
  WebServer(AsyncWebServer* server, TickTwo* heartbeat_ticker, TickTwo* check_devices_ticker);
  void init();
  void loop();
  void send_heartbeat();
  void check_devices();
  void reset_wifi();
  const String serialize_devices();

 private:
  AsyncWebServer* _server;
  bool oled_enabled;
  bool is_connected;
  WiFiUDP udp;
  uint udp_port;
  String device_id;
  String leader_id;
  bool is_leader;
  IPAddress multicast_ip;
  String hostname;
  std::vector<DeviceInfo> devices;
  uint heartbeat_interval;
  uint heartbeat_timeout;
  uint check_devices_interval;
  TickTwo* heartbeat_ticker;
  TickTwo* check_devices_ticker;
  uint32_t last_log;

  void setup_mdns();
  void send_election_message();
  void handle_incoming_packets();
  void update_device(String id, bool leader);
  void determine_leader();
  int get_device_index(String id);
};

#endif