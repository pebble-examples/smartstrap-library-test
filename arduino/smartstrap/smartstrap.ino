#include <Arduino.h>
#include <ArduinoPebbleSerial.h>

#define DATA_PIN 8
static uint8_t s_data_buffer[128];
static long s_last_time = 0;

static const uint16_t s_service_ids[] = {0x0};
static const uint16_t s_attr_ids[] = {0x0};

void setup() {
  pinMode(13, OUTPUT);
  
  // Setup the Pebble smartstrap connection using one wire software serial
  ArduinoPebbleSerial::begin_software(DATA_PIN, s_data_buffer, sizeof(s_data_buffer),
                                      Baud57600, s_service_ids, 1);
}

void loop() {
  size_t length;
  bool is_read;
  uint16_t service_id;
  uint16_t attribute_id;
  
  // Check to see if a frame was received, and for which service and attribute
  if(ArduinoPebbleSerial::feed(&service_id, &attribute_id, &length, &is_read)) {
    // We got a frame!
    if((service_id == 0) && (attribute_id == 0)) {
      // This is a raw data service frame

      if(is_read) {
        // Write the data to send
        snprintf((char*)s_data_buffer, sizeof(s_data_buffer), "Hello, Pebble!");
    
        // Send the data
        ArduinoPebbleSerial::write(s_service_ids[0], s_attr_ids[0], true, s_data_buffer, strlen((char*)s_data_buffer)+1);
      }
    }
  }

  if (ArduinoPebbleSerial::is_connected()) {
    digitalWrite(13, HIGH);
    
    // Send the data to the watch every second
    long now = millis();
    if(now - s_last_time > 5000) {
      s_last_time = now;
      // Notify the watch
      ArduinoPebbleSerial::notify(s_service_ids[0], s_attr_ids[0]);
    }
  } else {
    digitalWrite(13, LOW);
  }
}
