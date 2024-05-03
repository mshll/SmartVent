/**
 * @file wifiFix.cpp
 * @brief Fixes the WiFiClient flush method.
 *        Solution by zotanmew from: https://github.com/espressif/arduino-esp32/issues/6129#issuecomment-1418051304
 */

#include "wifiFix.h"
#include <lwip/sockets.h>

#define WIFI_CLIENT_FLUSH_BUFFER_SIZE (1024)

void WiFiClientFixed::flush() {
  int res;
  size_t a = available(), toRead = 0;
  if (!a) {
    return;  // nothing to flush
  }
  auto *buf = (uint8_t *)malloc(WIFI_CLIENT_FLUSH_BUFFER_SIZE);
  if (!buf) {
    return;  // memory error
  }
  while (a) {
    // override broken WiFiClient flush method, ref https://github.com/espressif/arduino-esp32/issues/6129#issuecomment-1237417915
    res = read(buf, min(a, (size_t)WIFI_CLIENT_FLUSH_BUFFER_SIZE));
    if (res < 0) {
      log_e("fail on fd %d, errno: %d, \"%s\"", fd(), errno, strerror(errno));
      stop();
      break;
    }
    a -= res;
  }
  free(buf);
}