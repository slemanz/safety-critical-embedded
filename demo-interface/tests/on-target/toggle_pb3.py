"""
Configure PB3 as an output and blink it by toggling six times on a board
running the demo.

It opens the serial console, sends the commands you would otherwise type by
hand, and prints whatever the firmware sends back so the result can be
inspected.

    init B 3 output
    toggle B 3   (six times, 200 ms apart)

The serial port is /dev/ttyUSB0 and the console runs at 115200 8N1.
"""
import time

import serial

PORT = "/dev/ttyUSB0"
BAUDRATE = 115200

TOGGLE_COUNT = 7
TOGGLE_DELAY = 0.5


def send(console, command):
    for byte in (command + "\r").encode("ascii"):
        console.write(bytes([byte]))
        time.sleep(0.01)


with serial.Serial(PORT, BAUDRATE, timeout=0.5) as console:
    time.sleep(0.5)
    console.reset_input_buffer()
    console.reset_output_buffer()
    print("-- Toggle PB3")

    send(console, "init B 3 output")
    response = console.readline().decode("ascii", errors="ignore")
    time.sleep(0.1)
    response = console.readline().decode("ascii", errors="ignore")
    print("Console:", response)

    for _ in range(TOGGLE_COUNT):
        send(console, "toggle B 3")
        response = console.readline().decode("ascii", errors="ignore")
        response = console.readline().decode("ascii", errors="ignore")
        response = console.readline().decode("ascii", errors="ignore")
        print("Console:", response)
        time.sleep(TOGGLE_DELAY)
