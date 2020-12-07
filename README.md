# home-assistant.io & NodeMCU files
My home-assistant.io configuration & NodeMCU files 

<b>Home-assitant.io</b>
- configuration.yaml: Backup configuration file
- automations.yaml: Backup automatations file

<b>NodeMCU</b>
- dht22-mqtt.ino: sketch file for ESP8266 NodeMCU + DHT22 temperature sensor. MQTT for sending temperature values
- relay-mqtt.ino: sketch file for ESP8266 NodeMCU + relay. MQTT for turning ON/OFF the boiler
- ccs811_mqtt.ino: sketch file for ESP8266 NodeMCU + ccs811 air quality sensor(eCO2). MQTT for sending eCO2 ppm values

![NodeMCU pinout](https://github.com/dalvarezs/homeassistant-files/raw/master/nodemcu-gpio-pinout.jpg)

<b>Raspberry pi</b>
- pulsador.py: Turn on/off the boiler when the raspberrypi button is pressed
- dht22.py: DHT sensor reader for raspberry pi 3

![Raspberry pi pinout](https://github.com/dalvarezs/homeassistant-files/raw/master/raspberry23_pinout.png)
