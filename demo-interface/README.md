# Demo Interface

This directory contains a small, self-contained command interface that turns the
`safe_gpio` driver into an interactive serial console on the STM32F411. Its
purpose is not to be a production application, but to serve as a worked example
of how a validated driver can be exercised at runtime through a human-readable
protocol, while preserving the principles described in the rest of this
repository (strong typing, explicit status handling, fault tolerance, and
MISRA C alignment).

It builds directly on the [`safe-gpio`](../safe-gpio) example and is intended to
be the next in that series. Where the driver shows how a single pin operation is
made safe, this module shows how those operations are composed into a usable
front end without abandoning the same discipline: every command is parsed,
validated, and answered with a defined result.

## Source Layout

The pieces referenced throughout this document live at the following paths:

- [`app/Inc/gpio_demo.h`](app/Inc/gpio_demo.h) - the public interface: the
  command-line bound and the four entry points that drive the demonstration.
- [`app/Src/gpio_demo.c`](app/Src/gpio_demo.c) - the implementation: the command
  parsers, the per-command handlers, and the pin-tracking table.
- [`app/Src/main.c`](app/Src/main.c) - the application entry point that brings up
  the system and runs the demo loop.
- [`Makefile`](Makefile) - the build definition for the toolchain.
- [`tests/on-target`](tests/on-target) - a host-side script that drives the
  console from a computer over the serial link.

## Motivation

A validated driver is only half of the story; at some point a human, or a test,
needs to operate it. The naive way to do that is to scatter ad-hoc register
pokes through a throwaway `main`, which works until the demonstration grows and
the unchecked calls accumulate. By routing every interaction through a single
command interface, the same validation that protects the driver also protects
the demonstration: an out-of-range pin, an unconfigured port, or a write to an
input is rejected with a clear message rather than producing undefined hardware
behaviour.

The interface is deliberately narrow so that the mechanics, the line parsing,
the parameter conversion, and the status-checked handlers, remain visible rather
than being buried under application-specific complexity.

## The Command Interface

The board presents a console over USART2 (`PA2` transmit, `PA3` receive) at
`115200 8N1`. After reset it prints a banner and a help listing, then waits at a
`>` prompt. Each line typed by the operator is parsed into whitespace-separated
tokens, the first of which selects the handler:

| Command  | Parameters             | Effect                                   |
|----------|------------------------|------------------------------------------|
| `help`   | none                   | Print the command and parameter summary  |
| `init`   | `<port> <pin> <mode>`  | Configure a pin and start tracking it    |
| `deinit` | `<port> <pin>`         | Release a previously configured pin      |
| `write`  | `<port> <pin> <state>` | Drive an output pin high or low          |
| `read`   | `<port> <pin>`         | Report the current level of a pin        |
| `toggle` | `<port> <pin>`         | Invert the level of an output pin        |
| `list`   | none                   | List every pin currently configured      |

The parameters are accepted in the forms the parsers recognise:

- `port` - one of `A`, `B`, `C`, `D`, `E`, `H` (case-insensitive).
- `pin` - a decimal number from `0` to `15`.
- `mode` - `input`, `output`, `alternate`, or `analog`.
- `state` - `1`, `high`, or `on` for a logic high; `0`, `low`, or `off` for a
  logic low.

Unknown commands, malformed parameters, and operations on pins in the wrong
state are all reported as a single `Error:` line, and every handler returns a
`Status_t` so that the outcome is observable in code as well as on the console.

## Architecture

The module keeps a fixed-size table of configured pins, so no dynamic memory is
used and the maximum resource footprint is known at compile time. Three layers
are visible in the source:

- **Parsers** (`ParsePort`, `ParsePin`, `ParseMode`, `ParseState`) convert one
  token into a typed driver value, rejecting anything outside the allowed set.
- **Handlers** (`HandleInitCommand` and its siblings) validate the parameter
  count, run the parsers, check the pin's tracked state, and call the driver.
- **The runtime** (`GPIO_Demo_Init`, `GPIO_Demo_Run`) brings up the UART and the
  driver, then services one line per call from the main loop, leaving room for
  other periodic work.

## Building and Flashing

The build uses the `arm-none-eabi` toolchain and produces an ELF, a raw binary,
and a size report:

```sh
make          # build build/flash.elf and build/flash.bin
make flash    # program the board over J-Link
make size     # print the section sizes of the ELF
make symbols  # list the linked symbols ordered by size
```

## On-target Testing

With the firmware flashed and the board connected, the script under
[`tests/on-target`](tests/on-target) opens the serial console and replays the
commands that configure and drive a pin, printing the firmware's replies so the
result can be inspected. It is the smallest illustration of how the text
interface can be operated from a host rather than by hand, and a starting point
for automating the same checks later.
