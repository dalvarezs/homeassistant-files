//EJ: 4 Relay Switch over WIFI using MQTT and Home Assistant with OTA Support
//EJ: The only way I know how to contribute to the HASS Community... you guys ROCK!!!!!
//EJ: Erick Joaquin :-) Australia

// Original sketch courtesy of ItKindaWorks
//ItKindaWorks - Creative Commons 2016
//github.com/ItKindaWorks
//
//Requires PubSubClient found here: https://github.com/knolleary/pubsubclient
//
//ESP8266 Simple MQTT switch controller


#include <PubSubClient.h>
#include <ESP8266WiFi.h>

void callback(char* topic, byte* payload, unsigned int length);

//EDIT THESE LINES TO MATCH YOUR SETUP
#define MQTT_SERVER "IP" 
const char* ssid = "SSID";
const char* password = "PASSWORD";

const int switchPin1 = D1;

char const* switchTopic1 = "/house/switchCalderaCmd/";

WiFiClient wifiClient;
PubSubClient client(MQTT_SERVER, PORT, callback, wifiClient);

void setup() {
  //initialize the switch as an output and set to LOW (off)
  pinMode(switchPin1, OUTPUT); // Relay Switch 1
  digitalWrite(switchPin1, LOW);

  //start the serial line for debugging
  Serial.begin(115200);
  delay(100);

  //start wifi subsystem
  WiFi.begin(ssid, password);
  //attempt to connect to the WIFI network and then connect to the MQTT server
  reconnect();

  //wait a bit before starting the main loop
      delay(2000);
}


void loop(){

  //reconnect if connection is lost
  if (!client.connected() && WiFi.status() == 3) {reconnect();}

  //maintain MQTT connection
  client.loop();

  //MUST delay to allow ESP8266 WIFI functions to run
  delay(10); 
}

void callback(char* topic, byte* payload, unsigned int length) {

  //convert topic to string to make it easier to work with
  String topicStr = topic; 

  //Print out some debugging info
  Serial.println("Callback update.");
  Serial.print("Topic: ");
  Serial.println(topicStr);

   if (topicStr == "/house/switchCalderaCmd/") 
    {

     //turn the switch on if the payload is '1' and publish to the MQTT server a confirmation message
     if(payload[0] == '1'){
       digitalWrite(switchPin1, HIGH);
       Serial.println("Payload: 1");
       client.publish("/house/switchCaldera/", "1");
       }
      //turn the switch off if the payload is '0' and publish to the MQTT server a confirmation message
     else if (payload[0] == '0'){
       digitalWrite(switchPin1, LOW);
       Serial.println("Payload: 0");
       client.publish("/house/switchCaldera/", "0");
       }
     }
}

void reconnect() {

  //attempt to connect to the wifi if connection is lost
  if(WiFi.status() != WL_CONNECTED){
    //debug printing
    Serial.print("Connecting to ");
    Serial.println(ssid);

    //loop while we wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    //print out some more debug once connected
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }

  //make sure we are connected to WIFI before attemping to reconnect to MQTT
  if(WiFi.status() == WL_CONNECTED){
  // Loop until we're reconnected to the MQTT server
    while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");

      // Generate client name based on MAC address and last 8 bits of microsecond counter
      String clientName;
      clientName += "esp8266-";
      uint8_t mac[6];
      WiFi.macAddress(mac);
      clientName += macToStr(mac);

      //if connected, subscribe to the topic(s) we want to be notified about
      if (client.connect((char*) clientName.c_str(),"USER", "PASSWORD")) {  //EJ: Update accordingly with your MQTT account 
        Serial.print("\tMQTT Connected");
        client.subscribe(switchTopic1);
        //EJ: Do not forget to replicate the above line if you will have more than the above number of relay switches
      }

      //otherwise print failed for debugging
      else{Serial.println("\tFailed."); abort();}
    }
  }
}

//generate unique name from MAC addr
String macToStr(const uint8_t* mac){

  String result;

  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);

    if (i < 5){
      result += ':';
    }
  }

  return result;
}
