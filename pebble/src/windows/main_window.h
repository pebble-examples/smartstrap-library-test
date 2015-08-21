#pragma once

#include <pebble.h>

#define MAIN_WINDOW_STATE_HEIGHT 20

void main_window_push();

void main_window_set_connected_state(bool is_connected);

void main_window_set_bytes_read(int bytes);

void main_window_set_error_rate(int errors, int total);

void main_window_set_notif_count(int notification_count);
