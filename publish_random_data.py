import time
import random
import sys
import os
import subprocess

initial_val = random.uniform(-10, 60)


while True:
    initial_val += random.uniform(-2.3, 2.3);

    subprocess.call('mosquitto_pub -t "a/b" -m "' + str(initial_val) + '" -i 1', shell=True)

    print("Sending: " + str(initial_val))

    time.sleep(1)
