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
        time.sleep(0.1)


with serial.Serial(PORT, BAUDRATE, timeout=1) as console:
    # Let the board boot, then flush both directions so the boot banner and
    # any stale transmit bytes are gone before we start commanding.
    time.sleep(0.5)
    console.reset_input_buffer()
    console.reset_output_buffer()
    print(console.read(console.in_waiting).decode("ascii", errors="ignore"))

    print("reset")

    send(console, "init B 3 output")
    time.sleep(0.1)
    print(console.read(console.in_waiting).decode("ascii", errors="ignore"))

    send(console, "write B 3 high")
    time.sleep(0.1)
    print(console.read(console.in_waiting).decode("ascii", errors="ignore"))
    
    console.reset_input_buffer()
    console.reset_output_buffer()
