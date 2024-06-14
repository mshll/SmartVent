/**
 * @file  preferences_manager.cpp
 * @copyright Copyright (c) 2024
 */

#include "preferences_manager.h"

PreferencesManager::PreferencesManager(const char* namespace_name) {
  this->namespace_name = namespace_name;
}

PreferencesManager::~PreferencesManager() {
  prefs.end();
}

void PreferencesManager::init() {
  prefs.begin(namespace_name, false);  // false for read/write access
}

void PreferencesManager::end() {
  prefs.end();
}

void PreferencesManager::set_value(const char* key, int value) {
  prefs.putInt(key, value);
}

void PreferencesManager::set_value(const char* key, float value) {
  prefs.putFloat(key, value);
}

void PreferencesManager::set_value(const char* key, bool value) {
  prefs.putBool(key, value);
}

void PreferencesManager::set_value(const char* key, const String& value) {
  prefs.putString(key, value);
}

int PreferencesManager::get_value(const char* key, int default_value) {
  return prefs.getInt(key, default_value);
}

float PreferencesManager::get_value(const char* key, float default_value) {
  return prefs.getFloat(key, default_value);
}

bool PreferencesManager::get_value(const char* key, bool default_value) {
  return prefs.getBool(key, default_value);
}

String PreferencesManager::get_value(const char* key, const String& default_value) {
  return prefs.getString(key, default_value);
}
