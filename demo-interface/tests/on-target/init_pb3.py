"""
Configure PB3 as an output and drive it high on a board running the demo.

It opens the serial console, sends the two commands you would otherwise type
by hand, and prints whatever the firmware sends back so the result can be
inspected.

    init B 3 output
    write B 3 high

The serial port is /dev/ttyUSB0 and the console runs at 115200 8N1.
"""
import time

import serial

PORT = "/dev/ttyUSB0"
BAUDRATE = 115200


def send(console, command):
    for byte in (command + "\r").encode("ascii"):
        console.write(bytes([byte]))
        time.sleep(0.01)


with serial.Serial(PORT, BAUDRATE, timeout=0.2) as console:
    time.sleep(0.5)
    console.reset_input_buffer()
    console.reset_output_buffer_buffer()
    print("-- Init PB3")

    send(console, "init B 3 output")
    time.sleep(0.1)
    response = console.readline().decode("ascii", errors="ignore")
    print("Console:", response)

    send(console, "write B 3 high")
    time.sleep(0.1)
    response = console.readline().decode("ascii", errors="ignore")
    print("Console:", response)