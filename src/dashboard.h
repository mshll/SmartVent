/**
 * @file  dashboard.h
 * @copyright Copyright (c) 2024
 */

#pragma once

#include <Arduino.h>
#include <ESPDASH.h>
#include <TickTwo.h>

typedef struct {
  std::vector<String> timestamps;
  std::vector<float> temperatures_c;
  std::vector<float> temperatures_f;
  std::vector<float> co2;
} DataLog;

class Dashboard {
 public:
  Dashboard();
  void init();
  void loop();

 private:
  uint32_t last_log;
  TickTwo *dashboard_ticker;
  TickTwo *data_log_ticker;
  DataLog data_log;

  void set_callbacks();
  void dashboard_ticker_handler();
  void update_air_quality_card(int co2);
  void update_charts();
  void log_data();
};