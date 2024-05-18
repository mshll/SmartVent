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

#define OLED_SCL 22
#define OLED_SDA 23

#define FANS_PIN 18
#define FANS_FREQUENCY 25000
#define FANS_RESOLUTION 8
#define RELAY_PIN 19

#define HOSTNAME "smartvent"
#define AP_SSID "SmartVent AP"
#define AP_PASS "capstone"
#define DEVICE_NAME "Smart Vent"

#define FAN_THRESHOLD_OFF 800
#define FAN_THRESHOLD_LOW 1000
#define FAN_THRESHOLD_MEDIUM 1500
#define FAN_THRESHOLD_HIGH 2000