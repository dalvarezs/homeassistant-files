import Adafruit_DHT
import paho.mqtt.publish as publish
import paho.mqtt.client as mqtt
import time

DHT_SENSOR = Adafruit_DHT.DHT22
DHT_PIN = 4

auth = {
  'username':"<mqtt_user>",
  'password':"<mqtt_passwd>"
}

while True:
    humidity, temperature = Adafruit_DHT.read_retry(DHT_SENSOR, DHT_PIN)

    if humidity is not None and temperature is not None:
#        print("Temp={0:0.1f}*C  Humidity={1:0.1f}%".format(temperature, humidity))
        temp='{"temperature": "'+str(temperature)+'","humidity": "'+str(humidity)+'"}'
        publish.single("<topic>",
            payload=temp,
            hostname="<IP>",
            client_id="<ID>",
            auth=auth,
            port=1883)
    else:
#        print("Failed to retrieve data from DHT sensor")
    
    time.sleep(600)
