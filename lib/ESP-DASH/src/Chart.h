#ifndef __CHART_H
#define __CHART_H

#include <functional>
#include "Arduino.h"
#include <vector>

#include "ESPDash.h"
#include "ArduinoJson.h"

// Default to Line Chart
enum {
  BAR_CHART,
};

struct ChartNames {
  int value;
  const char* type;
};

enum GraphAxisType { INTEGER, FLOAT, STRING };

// Forward Declaration
class ESPDash;

// Chart Class
class Chart {
  private:
    ESPDash *_dashboard;

    uint32_t _id;
    String _name;
    int   _type;
    bool  _changed;
    GraphAxisType _x_axis_type;
    GraphAxisType _y_axis_type;
    /* X-Axis */
    std::vector<int> _x_axis_i;
    std::vector<float> _x_axis_f;
    std::vector<String> _x_axis_s;
    /* Y-Axis */
    std::vector<int> _y_axis_i;
    std::vector<float> _y_axis_f;

  public:
    Chart(ESPDash *dashboard, const int type, const char* name);
    void updateX(int arr_x[], size_t x_size);
    void updateX(float arr_x[], size_t x_size);
    void updateX(String arr_x[], size_t x_size);
    void updateY(int arr_y[], size_t y_size);
    void updateY(float arr_y[], size_t y_size);
    ~Chart();

    // vector overloads
    void updateX(const std::vector<int>& x);
    void updateX(const std::vector<float>& x);
    void updateX(const std::vector<String>& x);
    void updateY(const std::vector<int>& y);
    void updateY(const std::vector<float>& y);

    friend class ESPDash;
};

#endif