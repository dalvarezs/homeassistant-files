/*
   MQTT Sensor - Temperature and Humidity (DHT22) for Home-Assistant - NodeMCU (ESP8266)
   https://home-assistant.io/components/sensor.mqtt/

   Libraries :
    - ESP8266 core for Arduino : https://github.com/esp8266/Arduino
    - PubSubClient : https://github.com/knolleary/pubsubclient
    - DHT : https://github.com/adafruit/DHT-sensor-library
    - ArduinoJson6: https://github.com/bblanchon/ArduinoJson


   Samuel M. - v1.1 - 08.2016
   https://github.com/mertenats/open-home-automation
   
   @dalvarez_s - 20181214
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHTesp.h"
#include <ArduinoJson.h>

#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY!)
#error Select ESP8266 board.
#endif
#define MQTT_VERSION MQTT_VERSION_3_1_1

// Wifi: SSID and password
const char* WIFI_SSID = "SSID";
const char* WIFI_PASSWORD = "PASSWORD";

// MQTT: ID, server IP, port, username and password
const PROGMEM char* MQTT_CLIENT_ID = "CLIENT";
const PROGMEM char* MQTT_SERVER_IP = "IP";
const PROGMEM uint16_t MQTT_SERVER_PORT = PORT;
const PROGMEM char* MQTT_USER = "USER";
const PROGMEM char* MQTT_PASSWORD = "USER_PASSWORD";

// MQTT: topic
const PROGMEM char* MQTT_SENSOR_TOPIC = "house/salon";

// sleeping time
const PROGMEM uint16_t SLEEPING_TIME_IN_SECONDS = 600; // 10 minutes x 60 seconds -- 600

// DHT - D1/GPIO5
#define DHTPIN D1
#define DHTTYPE DHT22

DHTesp dht;
WiFiClient wifiClient;
PubSubClient client(wifiClient);

// function called to publish the temperature and the humidity
void publishData(float p_temperature, float p_humidity) {
  // create a JSON object
  // doc : https://github.com/bblanchon/ArduinoJson/wiki/API%20Reference
  const size_t CAPACITY = JSON_OBJECT_SIZE(128); //MQTT_MAX_PACKET_SIZE
  StaticJsonDocument<CAPACITY> jsonBuffer;
  JsonObject root = jsonBuffer.to<JsonObject>();
  
  // INFO: the data must be converted into a string; a problem occurs when using floats...
  root["temperature"] = (String)p_temperature;
  root["humidity"] = (String)p_humidity;
  serializeJsonPretty(root, Serial);
  Serial.println("");
  /*
     {
        "temperature": "23.20" ,
        "humidity": "43.70"
     }
  */
  char data[128]; //MQTT_MAX_PACKET_SIZE
  serializeJson(root, data, measureJson(root) + 1);
  client.publish(MQTT_SENSOR_TOPIC, data, true);
  yield();
}

// function called when a MQTT message arrived
void callback(char* p_topic, byte* p_payload, unsigned int p_length) {
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("INFO: Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("INFO: connected");
    } else {
      Serial.print("ERROR: failed, rc=");
      Serial.print(client.state());
      Serial.println("DEBUG: try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  // init the serial
  Serial.begin(115200);

  dht.setup(DHTPIN, DHTesp::DHT22);

  // init the WiFi connection
  Serial.println();
  Serial.println();
  Serial.print("INFO: Connecting to ");
  WiFi.mode(WIFI_STA);
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("INFO: WiFi connected");
  Serial.println("INFO: IP address: ");
  Serial.println(WiFi.localIP());

  // init the MQTT connection
  client.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.getHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.getTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("ERROR: Failed to read from DHT sensor!");
    return;
  } else {
    Serial.println(h);
    Serial.println(t);
    publishData(t, h);
  }

  Serial.println("INFO: Closing the MQTT connection");
  client.disconnect();

  Serial.println("INFO: Closing the Wifi connection");
  WiFi.disconnect();

  ESP.deepSleep(600e6);  // sleeping time 10min -> 600 (60 sec x 10)
  delay(500); // wait for deep sleep to happen
}
