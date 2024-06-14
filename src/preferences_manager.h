/**
 * @file  preferences_manager.h
 * @copyright Copyright (c) 2024
 */

#pragma once

#include <Arduino.h>
#include <Preferences.h>
#include <String.h>

class PreferencesManager {
 private:
  Preferences prefs;
  const char* namespace_name;

 public:
  PreferencesManager(const char* namespace_name);
  ~PreferencesManager();

  void init();
  void end();

  // Overloaded setters
  void set_value(const char* key, int value);
  void set_value(const char* key, float value);
  void set_value(const char* key, bool value);
  void set_value(const char* key, const String& value);

  // Overloaded getters
  int get_value(const char* key, int default_value = 0);
  float get_value(const char* key, float default_value = 0.0f);
  bool get_value(const char* key, bool default_value = false);
  String get_value(const char* key, const String& default_value = "");
};
