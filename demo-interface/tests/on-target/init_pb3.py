"""
Configure PB3 as an output and drive it high on a board running the demo.

This is the smallest possible on-target check: it opens the serial console,
sends the two commands you would otherwise type by hand, and prints whatever
the firmware sends back so the result can be inspected.

    init B 3 output
    write B 3 high

The serial port is /dev/ttyUSB0 and the console runs at 115200 8N1.
"""
import time

import serial

PORT = "/dev/ttyUSB0"
BAUDRATE = 115200


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

        for command in ("init B 3 output", "write B 3 high"):
            response = send_command(console, command)
            print("> " + command)
            print(response)


if __name__ == "__main__":
    main()
