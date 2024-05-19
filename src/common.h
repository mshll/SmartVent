/**
 * @file  common.h
 * @copyright Copyright (c) 2024
 */

#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <MHZ19.h>
#include <MycilaESPConnect.h>
#include <Preferences.h>
#include <TickTwo.h>
#include <U8g2lib.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <time.h>
#include <map>

#define OLED_SCL SCL
#define OLED_SDA SDA

#define FANS_PIN 18
#define FANS_FREQUENCY 25000
#define FANS_RESOLUTION 8
#define RELAY_PIN 19

#define HOSTNAME "smartvent"
#define AP_SSID "SmartVent AP"
#define AP_PASS "capstone"
#define DEVICE_NAME "Smart Vent"

#define BUTTON_LEFT_PIN A5
#define BUTTON_RIGHT_PIN A0
#define BUTTON_UP_PIN SCK
#define BUTTON_DOWN_PIN A1

#define CO2_THRESHOLD_1 800
#define CO2_THRESHOLD_2 1400
#define CO2_THRESHOLD_3 2000
#define CO2_THRESHOLD_4 3000

#define AQ_THRESHOLD_1 "Good"
#define AQ_THRESHOLD_2 "Moderate"
#define AQ_THRESHOLD_3 "Poor"
#define AQ_THRESHOLD_4 "Unhealthy"
#define AQ_THRESHOLD_5 "Hazardous"

const std::map<std::string, std::string> air_quality_map = {
    {AQ_THRESHOLD_1, "success"}, {AQ_THRESHOLD_2, "warning"}, {AQ_THRESHOLD_3, "warning"}, {AQ_THRESHOLD_4, "danger"}, {AQ_THRESHOLD_5, "danger"}};
