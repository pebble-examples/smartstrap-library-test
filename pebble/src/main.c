/**
 * App to test the ArduinoPebbleSerial library
 */

#include <pebble.h>

#include "strap.h"

#include "windows/main_window.h"

static void timer_handler(void *context) {
  strap_request_data();
  app_timer_register(1000, timer_handler, NULL);
}

static void init() {
  strap_init();
  main_window_push();

  app_timer_register(1000, timer_handler, NULL);
}

static void deinit() { }

int main() {
  init();
  app_event_loop();
  deinit();
}
