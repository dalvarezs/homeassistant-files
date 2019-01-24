
# @dalvarez_s
# 2019/01/05
import RPi.GPIO as GPIO
import time    
import shlex, subprocess
import logging

logger = logging.getLogger(__name__)
logger.setLevel(logging.INFO)

# create a file handler
handler = logging.FileHandler('pulsador.log')
handler.setLevel(logging.INFO)

# create a logging format
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
handler.setFormatter(formatter)

# add the handlers to the logger
logger.addHandler(handler)

# https://www.prometec.net/condicionales-y-pulsadores/

# 1 pin al GPIO 24 (PIN 18) y otro a GND por Pull UP

GPIO.setmode(GPIO.BCM) # Nomemclatura GPIO, en lugar de numero de PIN
GPIO.setup(24, GPIO.IN, pull_up_down = GPIO.PUD_UP)
logger.info('Booting')


while True:
    input_state = GPIO.input(24)
    if input_state == False:
        logger.info('Boton pulsado')
        time.sleep(.5)
        input_state = GPIO.input(24)
        if input_state == False:
            logger.info('Boton pulsado2')
            cmd_line = "/usr/bin/mosquitto_pub -h IP -u USER -P PASSWORD -t '/house/switchCalderaCmd/' -m '1'"
            args = shlex.split(cmd_line)
            subprocess.Popen(args)
            time.sleep(1)
GPIO.cleanup()
