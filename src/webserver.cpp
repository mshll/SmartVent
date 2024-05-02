/**
 * @file  webserver.cpp
 * @copyright Copyright (c) 2024
 */

#include "webserver.h"

/**
 * @brief Constructor for the WebServer class.
 * @param server The AsyncWebServer object.
 * @param heartbeat_ticker The TickTwo object for the heartbeat ticker.
 * @param check_devices_ticker The TickTwo object for the check devices ticker.
 */
WebServer::WebServer(AsyncWebServer* server, TickTwo* heartbeat_ticker, TickTwo* check_devices_ticker) {
  _server = server;
  oled_enabled = true;
  is_connected = false;
  device_id = String((uint32_t)ESP.getEfuseMac(), HEX);
  leader_id = "";
  is_leader = false;
  multicast_ip = IPAddress(226, 1, 1, 1);
  udp_port = 4096;
  hostname = String(HOSTNAME) + "-" + device_id;
  heartbeat_interval = 5000;
  heartbeat_timeout = 15000;
  check_devices_interval = 16000;
  last_log = 0;

  this->heartbeat_ticker = heartbeat_ticker;
  this->check_devices_ticker = check_devices_ticker;
}

void WebServer::init() {
  // clear configuration
  _server->on("/clear", HTTP_GET, [&](AsyncWebServerRequest* request) {
    Serial.println("Clearing configuration...");
    reset_wifi();
    request->send(200);
  });

  // add a rewrite which is only applicable in AP mode and STA mode, but not in Captive Portal mode
  _server->rewrite("/", "/dash").setFilter([](AsyncWebServerRequest* request) { return ESPConnect.getState() != ESPConnectState::PORTAL_STARTED; });

  if (get_device_index(device_id) == -1) devices.push_back({device_id, WiFi.localIP(), millis()});

  // network state listener for async mode
  ESPConnect.listen([&](ESPConnectState previous, ESPConnectState state) {
    JsonDocument doc;
    ESPConnect.toJson(doc.to<JsonObject>());
    serializeJsonPretty(doc, Serial);
    Serial.println();

    is_connected = false;

    switch (state) {
      case ESPConnectState::NETWORK_CONNECTED:
      case ESPConnectState::AP_STARTED:
        //*************************************************************
        udp.beginMulticast(multicast_ip, udp_port);
        is_connected = true;

        Serial.println("==========> NETWORK CONNECTED");
        send_election_message();
        determine_leader();
        setup_mdns();

        heartbeat_ticker->start();
        check_devices_ticker->start();

        _server->begin();
        break;
        //*************************************************************
      case ESPConnectState::NETWORK_DISCONNECTED:
        _server->end();
      default:
        break;
    }
  });

  ESPConnect.setAutoRestart(false);
  ESPConnect.setBlocking(false);
  ESPConnect.setCaptivePortalTimeout(86400);
  ESPConnect.setConnectTimeout(10);

  Serial.println("====> Trying to connect to saved WiFi or will start portal in the background...");
  ESPConnect.begin(_server, HOSTNAME, AP_SSID, AP_PASS);
}

/**
 * @brief Main loop for the web server.
 */
void WebServer::loop() {
  ESPConnect.loop();

  if (millis() - last_log > 5000) {
    Serial.println("ID: " + device_id + " // " + "Leader: " + leader_id + " // " + "URL: http://" + (is_leader ? HOSTNAME : hostname) + ".local");
    last_log = millis();
  }

  if (!is_connected) return;

  handle_incoming_packets();
  heartbeat_ticker->update();
  check_devices_ticker->update();
}

/**
 * @brief Sets up mDNS responder using the appropriate hostname depending on the device's role.
 */
void WebServer::setup_mdns() {
  String hostname_str = is_leader ? HOSTNAME : hostname;  // If the device is the leader, use the default hostname

  delay(500);
  if (MDNS.begin(hostname_str.c_str())) {
    MDNS.addService("http", "tcp", 80);
    Serial.println("mDNS responder started on " + hostname_str);
  } else {
    Serial.println("Error setting up MDNS responder!");
  }
}

/**
 * @brief Sends a heartbeat message to all devices in the network to indicate that the device is still active.
 */
void WebServer::send_heartbeat() {
  int device_index = get_device_index(device_id);
  String message = "HB:" + device_id + ":" + (is_leader ? "1" : "0");
  Serial.println("---> Sending heartbeat message: " + message);
  udp.beginMulticastPacket();
  udp.write((const uint8_t*)message.c_str(), message.length());
  udp.endPacket();
  devices.at(device_index).last_heartbeat = millis();
}

/**
 * @brief Sends an election message to all devices in the network to initiate an election for the leader.
 */
void WebServer::send_election_message() {
  String message = "ELECT:" + device_id;
  Serial.println("---> Sending election message: " + message);
  udp.beginMulticastPacket();
  udp.write((const uint8_t*)message.c_str(), message.length());
  udp.endPacket();
}

/**
 * @brief Handles incoming packets from the network.
 */
void WebServer::handle_incoming_packets() {
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
      if (device_id < sender_id) {
        send_election_message();
      }
      update_device(sender_id, false);
      // determine_leader();
    }
  }
}

/**
 * @brief Updates the device's information in the devices vector.
 * @param id The device's ID.
 * @param leader Whether the device is the leader or not.
 */
void WebServer::update_device(String id, bool leader) {
  bool found = false;
  for (auto& device : devices) {
    if (device.id == id) {
      device.last_heartbeat = millis();
      found = true;
      break;
    }
  }
  if (!found) {
    DeviceInfo new_device;
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
void WebServer::check_devices() {
  Serial.println("Checking devices...");
  uint32_t current_time = millis();
  bool leader_inactive = false;

  for (int i = devices.size() - 1; i >= 0; i--) {
    if (devices[i].id == device_id) continue;  // Skip own device

    if (current_time - devices[i].last_heartbeat > heartbeat_timeout) {
      Serial.println("Device " + devices[i].id + " is inactive.");
      if (devices[i].id == leader_id) {
        leader_inactive = true;
      }
      devices.erase(devices.begin() + i);  // Remove inactive device
    }
  }

  // If the leader went inactive, initiate an election
  if (leader_inactive) {
    send_election_message();
    determine_leader();
  }
  Serial.println("Devices count: " + String(devices.size()));
}

/**
 * @brief Determines the leader of the network based on the device IDs.
 */
void WebServer::determine_leader() {
  bool was_leader = is_leader;

  // Sort devices by ID
  std::sort(devices.begin(), devices.end(), [](const DeviceInfo& a, const DeviceInfo& b) { return a.id < b.id; });

  // Make the device with the smallest ID the leader
  leader_id = devices.front().id;
  is_leader = (device_id == leader_id);

  if (was_leader != is_leader) {  // Leader status changed
    MDNS.end();
  }
  setup_mdns();
}

/**
 * @brief Resets the WiFi configuration.
 */
void WebServer::reset_wifi() {
  ESPConnect.clearConfiguration();
  ESPConnect.end();
  MDNS.end();
  udp.stop();
  _server->end();
  delay(500);
  ESPConnect.begin(_server, HOSTNAME, AP_SSID, AP_PASS);
}

/**
 * @brief Gets the index of a device in the devices vector.
 * @param id The device's ID.
 * @return The index of the device in the devices vector.
 */
int WebServer::get_device_index(String id) {
  for (int i = 0; i < devices.size(); i++) {
    if (devices[i].id == id) {
      return i;
    }
  }
  return -1;
}

/**
 * @brief Serializes the devices vector to a JSON string.
 * @return The JSON string representing the devices vector.
 */
const String WebServer::serialize_devices() {
  String json = "\"devices\": [";
  for (int i = 0; i < devices.size(); i++) {
    bool leader = devices[i].id == leader_id;
    json += "{";
    json += "\"id\": \"" + devices[i].id + "\",";
    json += "\"name\": \"Smart Vent" + (devices.size() > 1 ? (" " + String(i + 1)) : "") + "\",";
    json += "\"url\": \"http://" + String(HOSTNAME) + (leader ? "" : "-" + devices[i].id) + ".local\",";
    json += "\"is_leader\": " + String(leader) + ",";
    json += "\"this_device\": " + String(devices[i].id == device_id);
    // json += "\"last_heartbeat\": " + String(devices[i].last_heartbeat);
    json += "}";
    if (i < devices.size() - 1) {
      json += ",";
    }
  }
  json += "]";
  // Serial.println("Devices JSON: \n" + json);
  return json;
}