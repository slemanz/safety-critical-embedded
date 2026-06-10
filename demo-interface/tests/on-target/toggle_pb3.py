"""
Configure PB3 as an output and blink it by toggling six times on a board
running the demo.

This is a small on-target check: it opens the serial console, configures the
pin, then sends the toggle command six times with a 200 ms pause between them,
printing whatever the firmware sends back so the result can be inspected.

    init B 3 output
    toggle B 3   (six times, 200 ms apart)

The serial port is /dev/ttyUSB0 and the console runs at 115200 8N1.
"""
import time

import serial

PORT = "/dev/ttyUSB0"
BAUDRATE = 115200

TOGGLE_COUNT = 6
TOGGLE_DELAY = 0.2


def read_until_quiet(console):
    """Read and accumulate bytes until the board stops sending."""
    # console.read() returns after its timeout once no more bytes arrive, so an
    # empty chunk marks the end of the firmware's output for now.
    text = ""
    while True:
        chunk = console.read(256).decode("ascii", errors="ignore")
        if not chunk:
            break
        text += chunk
    return text


def send_command(console, command):
    """Send one command line and return the firmware's text response."""
    # Discard anything still pending (boot banner, previous prompt, ...).
    read_until_quiet(console)

    # The firmware reads one character per main-loop pass and has no buffered
    # receive, so send the line a byte at a time to avoid overruns, the way a
    # person typing would. The firmware ends a line on '\r' (see UART_GetLine).
    for byte in (command + "\r").encode("ascii"):
        console.write(bytes([byte]))
        time.sleep(0.005)

    return read_until_quiet(console)


def main():
    with serial.Serial(PORT, BAUDRATE, timeout=0.1) as console:
        # Let the board boot and discard the welcome banner before commanding.
        time.sleep(0.5)
        read_until_quiet(console)

        response = send_command(console, "init B 3 output")
        print("> init B 3 output")
        print(response)

        for _ in range(TOGGLE_COUNT):
            response = send_command(console, "toggle B 3")
            print("> toggle B 3")
            print(response)
            time.sleep(TOGGLE_DELAY)


if __name__ == "__main__":
    main()
