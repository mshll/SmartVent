/**
 * @file  webserver.h
 * @copyright Copyright (c) 2024
 */

#pragma once

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPDash.h>
#include <TickTwo.h>

typedef struct {
  String id;
  IPAddress ip;
  uint32_t last_heartbeat;
} DeviceInfo;

class WebServer {
 public:
  WebServer(AsyncWebServer* server);
  ~WebServer();
  void init();
  void loop();
  void reset_wifi();
  const String serialize_devices();
  const String serialize_stats();
  const String get_time(const char* format);

 private:
  AsyncWebServer* _server;
  bool is_connected;
  WiFiUDP udp;
  uint udp_port;
  String device_id;
  String device_name;
  String leader_id;
  bool is_leader;
  IPAddress multicast_ip;
  String hostname;
  std::vector<DeviceInfo> devices;
  uint heartbeat_timeout;
  TickTwo* heartbeat_ticker;
  TickTwo* check_devices_ticker;
  uint32_t last_log;

  void setup_mdns();
  void send_election_message();
  void send_heartbeat();
  void check_devices();
  void handle_incoming_packets();
  void update_device(String id, bool leader);
  void determine_leader();
  int get_device_index(String id);
  void setup_time();
};
