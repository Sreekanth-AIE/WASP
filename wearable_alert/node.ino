#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

// WiFi credentials and Adafruit IO details
struct Details {
    const char* ssid;
    const char* password;
    const char* adafruitUsername;
    const char* adafruitKey;
    const char* feedName;
    const int pin;
    const bool state;
};

Details Configs = {
    "your_ssid",
    "your_password",
    "your_adafruit_username",
    "your_adafruit_key",
    "wifi_name_feed",
    D1,
    false
};