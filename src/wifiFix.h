/**
 * @file wifiFix.h
 * @brief Fixes the WiFiClient flush method.
 *        Solution by zotanmew from: https://github.com/espressif/arduino-esp32/issues/6129#issuecomment-1418051304
 */

#pragma once
#include <WiFi.h>

class WiFiClientFixed : public WiFiClient {
 public:
  void flush() override;
};