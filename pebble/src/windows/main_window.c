#include "main_window.h"

static Window *s_main_window;
static TextLayer *s_connected_state_layer, *s_bytes_read_layer, 
                 *s_error_rate_layer, *s_notif_count_layer;

static TextLayer* text_layer_init(GRect bounds, char* font, GTextAlignment align, GColor color) {
  TextLayer *this = text_layer_create(bounds);
  text_layer_set_font(this, fonts_get_system_font(font));
  text_layer_set_text_alignment(this, align);
  text_layer_set_text_color(this, color);
  text_layer_set_background_color(this, GColorClear);
  layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(this));
  return this;
}

void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_connected_state_layer = text_layer_init(
    GRect(0, 0, bounds.size.w, MAIN_WINDOW_STATE_HEIGHT),
    FONT_KEY_GOTHIC_18_BOLD, GTextAlignmentCenter, GColorWhite);

  s_bytes_read_layer = text_layer_init(
    GRect(3, MAIN_WINDOW_STATE_HEIGHT, bounds.size.w, 28),
    FONT_KEY_GOTHIC_24_BOLD, GTextAlignmentLeft, GColorWhite);

  s_error_rate_layer = text_layer_init(
    GRect(3, (2 * MAIN_WINDOW_STATE_HEIGHT), bounds.size.w, 28),
    FONT_KEY_GOTHIC_24_BOLD, GTextAlignmentLeft, GColorWhite);

  s_notif_count_layer = text_layer_init(
    GRect(3, (3 * MAIN_WINDOW_STATE_HEIGHT), bounds.size.w, 28),
    FONT_KEY_GOTHIC_24_BOLD, GTextAlignmentLeft, GColorWhite);

  // Init UI
  main_window_set_connected_state(false);
  main_window_set_bytes_read(0);
  main_window_set_error_rate(0, 0);
  main_window_set_notif_count(0);
}

void main_window_unload(Window *window) {
  text_layer_destroy(s_connected_state_layer);
  text_layer_destroy(s_bytes_read_layer);
  text_layer_destroy(s_error_rate_layer);
  text_layer_destroy(s_notif_count_layer);

  window_destroy(s_main_window);
}

void main_window_push() {
  if(!s_main_window) {
    s_main_window = window_create();
    window_set_background_color(s_main_window, GColorBlack);
    window_set_window_handlers(s_main_window, (WindowHandlers) {
      .load = main_window_load,
      .unload = main_window_unload,
    });
  }
  window_stack_push(s_main_window, true);
}

void main_window_set_connected_state(bool is_connected) {
  text_layer_set_text(s_connected_state_layer, is_connected ? "CONNECTED" : "NOT CONNECTED");
  text_layer_set_background_color(s_connected_state_layer, is_connected ? GColorGreen : GColorRed);
}

void main_window_set_bytes_read(int bytes) {
  static char s_buff[32];
  snprintf(s_buff, sizeof(s_buff), "Read: %dB", bytes);
  text_layer_set_text(s_bytes_read_layer, s_buff);
}

void main_window_set_error_rate(int errors, int total) {
  static char s_buff[32];
  snprintf(s_buff, sizeof(s_buff), "Errors: %d/%d", errors, total);
  text_layer_set_text(s_error_rate_layer, s_buff);
}

void main_window_set_notif_count(int notification_count) {
  static char s_buff[32];
  snprintf(s_buff, sizeof(s_buff), "Notifs: %d", notification_count);
  text_layer_set_text(s_notif_count_layer, s_buff);
}
