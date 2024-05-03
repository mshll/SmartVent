#include "Chart.h"

/*
  Constructor
*/
Chart::Chart(ESPDash *dashboard, const int type, const char* name){
  _dashboard = dashboard;
  #if defined(ESP8266)
    _id = RANDOM_REG32;
  #elif defined(ESP32)
    _id = esp_random();
  #endif
  _type = type;
  _name = name;
  _dashboard->add(this);
}

/*
  Value update methods
*/
void Chart::updateX(int arr_x[], size_t x_size){
  _x_axis_type = GraphAxisType::INTEGER;
  if(!_x_axis_i.empty())
    _x_axis_i.clear();
  if(!_x_axis_f.empty())
    _x_axis_f.clear();
  if(!_x_axis_s.empty())
    _x_axis_s.clear();

  for(int i=0; i < x_size; i++){
    _x_axis_i.push_back(arr_x[i]);
  }
  _changed = true;
}

void Chart::updateX(float arr_x[], size_t x_size){
  _x_axis_type = GraphAxisType::FLOAT;
  if(!_x_axis_i.empty())
    _x_axis_i.clear();
  if(!_x_axis_f.empty())
    _x_axis_f.clear();
  if(!_x_axis_s.empty())
    _x_axis_s.clear();

  for(int i=0; i < x_size; i++){
    _x_axis_f.push_back(arr_x[i]);
  }
  _changed = true;
}

void Chart::updateX(String arr_x[], size_t x_size){
  _x_axis_type = GraphAxisType::STRING;
  if(!_x_axis_i.empty())
    _x_axis_i.clear();
  if(!_x_axis_f.empty())
    _x_axis_f.clear();
  if(!_x_axis_s.empty())
    _x_axis_s.clear();

  for(int i=0; i < x_size; i++){
    _x_axis_s.push_back(arr_x[i].c_str());
  }
  _changed = true;
}

void Chart::updateY(int arr_y[], size_t y_size){
  _y_axis_type = GraphAxisType::INTEGER;
  if(!_y_axis_i.empty())
    _y_axis_i.clear();
  if(!_y_axis_f.empty())
    _y_axis_f.clear();

  for(int i=0; i < y_size; i++){
    _y_axis_i.push_back(arr_y[i]);
  }
  _changed = true;
}

void Chart::updateY(float arr_y[], size_t y_size){
  _y_axis_type = GraphAxisType::FLOAT;
  if(!_y_axis_i.empty())
    _y_axis_i.clear();
  if(!_y_axis_f.empty())
    _y_axis_f.clear();

  for(int i=0; i < y_size; i++){
    _y_axis_f.push_back(arr_y[i]);
  }
  _changed = true;
}

/*
  Destructor
*/
Chart::~Chart(){
  _dashboard->remove(this);
}


// vector overloads
void Chart::updateX(const std::vector<int>& x){
  _x_axis_type = GraphAxisType::INTEGER;
  _x_axis_i = x;
  _changed = true;
}

void Chart::updateX(const std::vector<float>& x){
  _x_axis_type = GraphAxisType::FLOAT;
  _x_axis_f = x;
  _changed = true;
}

void Chart::updateX(const std::vector<String>& x){
  _x_axis_type = GraphAxisType::STRING;
  _x_axis_s = x;
  _changed = true;
}

void Chart::updateY(const std::vector<int>& y){
  _y_axis_type = GraphAxisType::INTEGER;
  _y_axis_i = y;
  _changed = true;
}

void Chart::updateY(const std::vector<float>& y){
  _y_axis_type = GraphAxisType::FLOAT;
  _y_axis_f = y;
  _changed = true;
}

