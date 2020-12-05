import Adafruit_DHT
import paho.mqtt.publish as publish
import paho.mqtt.client as mqtt
import time
import logging

logger = logging.getLogger("dht22")
logger.setLevel(logging.INFO)

# create a file handler
handler = logging.FileHandler('iot.log')
handler.setLevel(logging.INFO)

# create a logging format
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
handler.setFormatter(formatter)

# add the handlers to the logger
logger.addHandler(handler)

DHT_SENSOR = Adafruit_DHT.DHT22
DHT_PIN = 4

auth = {
  'username':"<mqtt_user>",
  'password':"<mqtt_passwd>"
}

logger.info('Booting')
while True:
    humidity, temperature = Adafruit_DHT.read_retry(DHT_SENSOR, DHT_PIN)

    if humidity is not None and temperature is not None:
        temp='{"temperature": "'+'%.2f' %temperature+'","humidity": "'+'%.2f' %humidity+'"}'
        print(temp)
        logger.info(temp)
        publish.single("<topic>",
            payload=temp,
            hostname="<IP>",
            client_id="<ID>",
            auth=auth,
            port=1883)
    else:
#        print("Failed to retrieve data from DHT sensor")
         logger.info('Failed to retrieve data from humidity sensor')
    
    time.sleep(600)
