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

// List of known Access-Point MAC addresses
const char* knownMACAddresses[] = {
    "00:11:22:33:44:55",
    "66:77:88:99:AA:BB",
    "CC:DD:EE:FF:00:11"
};

// Create an ESP8266 WiFiClient class to connect to the MQTT server
WiFiClient client;
// Setup the MQTT client class by passing in the WiFi client and MQTT server details
Adafruit_MQTT_Client mqtt(&client, "io.adafruit.com", 1883, Configs.adafruitUsername, Configs.adafruitKey);
// Setup a feed to publish to
Adafruit_MQTT_Publish wifiNameFeed = Adafruit_MQTT_Publish(&mqtt, (String(Configs.adafruitUsername) + "/feeds/" + Configs.feedName).c_str());

void connectToWiFi(const Details& wifiDetails) {
    Serial.println("Connecting to WiFi...");
    WiFi.begin(wifiDetails.ssid, wifiDetails.password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("Connected to WiFi");
}

bool isKnownMACAddress(const String& macAddress) {
    for (const char* knownMAC : knownMACAddresses) {
        if (macAddress.equalsIgnoreCase(knownMAC)) {
            return true;
        }
    }
    return false;
}

String getHighestStrengthWiFi() {
    Serial.println("Scanning for WiFi networks...");
    int n = WiFi.scanNetworks();
    int maxRSSI = -1000;  // Set to a very low value initially
    String bestSSID = "";
    for (int i = 0; i < n; ++i) {
        String currentMAC = WiFi.BSSIDstr(i);
        if (isKnownMACAddress(currentMAC) && WiFi.RSSI(i) > maxRSSI) {
            maxRSSI = WiFi.RSSI(i);
            bestSSID = WiFi.SSID(i);
        }
    }
    if (bestSSID != "") {
        Serial.print("Best NodeMCU WiFi: ");
        Serial.print(bestSSID);
        Serial.print(" with RSSI: ");
        Serial.println(maxRSSI);
    } else {
        Serial.println("No known NodeMCU devices found.");
    }
    return bestSSID;
}

void connectToMQTT() {
    int8_t ret;
    // Stop if already connected
    if (mqtt.connected()) {
        return;
    }
    Serial.print("Connecting to MQTT... ");
    while ((ret = mqtt.connect()) != 0) {
        Serial.println(mqtt.connectErrorString(ret));
        Serial.println("Retrying MQTT connection in 5 seconds...");
        mqtt.disconnect();
        delay(5000);
    }
    Serial.println("MQTT Connected!");
}

void uploadJSONDataToAdafruitIO(const String& wifiName) {
    connectToMQTT();
    if (!wifiNameFeed.publish(wifiName.c_str())) {
        Serial.println("Failed to publish to Adafruit IO");
    } else {
        Serial.println("Successfully published to Adafruit IO");
    }
}

void setup() {
    Serial.begin(115200);
    connectToWiFi(Configs);
    pinMode(Configs.pin, INPUT_PULLUP);
}

void loop() {
    if (digitalRead(Configs.pin) == Configs.state) {
        String highestStrengthWiFi = getHighestStrengthWiFi();
        if (highestStrengthWiFi != "") {
            uploadJSONDataToAdafruitIO(highestStrengthWiFi);
        } else {
            Serial.println("No known NodeMCU WiFi networks to upload.");
        }
    }
    mqtt.processPackets(10000);  // Allow the MQTT client to process incoming packets
    mqtt.ping();                 // Keep the connection alive
}
