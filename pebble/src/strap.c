#include "strap.h"

static int s_bytes_read = 0, s_errors = 0, s_total = 0, s_notifs = 0;

// Declare an attribute pointer
SmartstrapAttribute *attribute;

static char* smartstrap_result_to_string(SmartstrapResult result) {
  switch(result) {
    case SmartstrapResultOk:                   return "SmartstrapResultOk";
    case SmartstrapResultInvalidArgs:          return "SmartstrapResultInvalidArgs";
    case SmartstrapResultNotPresent:           return "SmartstrapResultNotPresent";
    case SmartstrapResultBusy:                 return "SmartstrapResultBusy";
    case SmartstrapResultServiceUnavailable:   return "SmartstrapResultServiceUnavailable";
    case SmartstrapResultAttributeUnsupported: return "SmartstrapResultAttributeUnsupported";
    case SmartstrapResultTimeOut:              return "SmartstrapResultTimeOut";
    default: return "Not a SmartstrapResult value!";
  }
}

static void strap_availability_handler(SmartstrapServiceId service_id, bool is_available) {
  // A service's availability has changed
  APP_LOG(APP_LOG_LEVEL_INFO, "Service %d is %s available", (int)service_id, is_available ? "now" : "NOT");

  // If the raw service is available, the strap is connected
  main_window_set_connected_state(is_available && service_id == SMARTSTRAP_RAW_DATA_SERVICE_ID);
}

static void strap_read_handler(SmartstrapAttribute *attribute, SmartstrapResult result, const uint8_t *data, size_t length) {
  if(result == SmartstrapResultOk) {
    // Update counter
    s_bytes_read += length;

    // Read the data
    APP_LOG(APP_LOG_LEVEL_INFO, "Smartstrap says: %s", (char*)data);
  } else {
    s_errors++;
    APP_LOG(APP_LOG_LEVEL_ERROR, "Error receiving data from smartstrap!");
  }

  // Update UI
  s_total++;
  main_window_set_error_rate(s_errors, s_total);
  main_window_set_bytes_read(s_bytes_read);
}

static void strap_notify_handler(SmartstrapAttribute *attribute) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Smartstrap sent notification.");
  s_notifs++;
  main_window_set_notif_count(s_notifs);
}

void strap_init() {
  // Create the attribute, and allocate a buffer for its data
  attribute = smartstrap_attribute_create(SMARTSTRAP_RAW_DATA_SERVICE_ID, SMARTSTRAP_RAW_DATA_ATTRIBUTE_ID, STRAP_BUFFER_SIZE);

  smartstrap_subscribe((SmartstrapHandlers) {
    .availability_did_change = strap_availability_handler,
    .did_read = strap_read_handler,
    .notified = strap_notify_handler
  });
}

void strap_request_data() {
  // Declare a buffer to be used
  size_t buff_size;
  uint8_t *buffer;

  // Begin the write request, getting the buffer and its length
  smartstrap_attribute_begin_write(attribute, &buffer, &buff_size);

  // Store the data to be written to this attribute
  snprintf((char*)buffer, buff_size, "Hello, smartstrap!");

  // End the write request, and send the data, expecting a response
  SmartstrapResult r = smartstrap_attribute_end_write(attribute, strlen((char*)buffer), true);
  if(r != SmartstrapResultOk) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Smartstrap error: %s", smartstrap_result_to_string(r));
  }
}
