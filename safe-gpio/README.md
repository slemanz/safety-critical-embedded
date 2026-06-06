# Safe GPIO

This directory contains a small, self-contained GPIO driver for the STM32F411
together with a reference application that exercises it. Its purpose is not to
be a feature-complete HAL, but to serve as a worked example of how the
principles described in the rest of this repository (strong typing, fault
tolerance, fail-safe behaviour, traceability, and MISRA C alignment) translate
into actual firmware for a peripheral as ordinary as a general-purpose I/O pin.

It is intended to be the first in a series of such examples. The driver is
deliberately narrow in scope so that the safety-oriented patterns remain
visible rather than being buried under hardware-specific complexity.

## Source Layout

The pieces referenced throughout this document live at the following paths:

- [`app/Inc/safe_gpio.h`](app/Inc/safe_gpio.h) - the public driver interface:
  the typed enumerations, the configuration structure, and the function
  declarations.
- [`app/Src/safe_gpio.c`](app/Src/safe_gpio.c) - the driver implementation,
  including the register access, validation helpers, and pin tracking.
- [`app/Inc/safe_status.h`](app/Inc/safe_status.h) - the shared `Status_t`
  status codes returned by every public function.
- [`app/Src/main.c`](app/Src/main.c) - the reference application that wires the
  driver to the LED, button, and actuator.
- [`Makefile`](Makefile) - the build definition for the toolchain.

## Motivation

A GPIO pin is the simplest peripheral most developers ever touch, which is
exactly why it is a good teaching vehicle. The naive approach, manipulating the
peripheral registers directly with raw bit shifts scattered across the
application, works perfectly well until it does not. There is nothing to stop a
caller from writing to a pin that was never configured as an output, from
selecting a port that does not exist on the package, or from leaving an output
in an undefined state after a fault. In a general-purpose program these are
minor bugs. In a safety-critical system they are exactly the class of latent
defect that the standards summarised under *Keys* and *Considerations* are
designed to eliminate.

The `safe_gpio` driver therefore wraps the raw register access behind a typed,
validated interface, and the example application shows how that interface is
meant to be consumed: every operation is checked, and every failure has a
defined consequence.

## Design Principles

The driver was built around the same characteristics that the repository
identifies as essential for safety-critical firmware. Each principle below maps
back to a concept documented elsewhere in this project.

### Strong Typing

Ports, pins, modes, output types, speeds, pull configurations, and pin states
are all expressed as dedicated enumerations rather than as raw integers. A
function that expects a port cannot silently accept an arbitrary number, and the
compiler participates in catching misuse before the code is ever run. This is a
direct application of the type-safety category of the MISRA C guidelines, where
the goal is to make the intent of each value explicit and to prevent the silent
conversions that hide defects.

### Input Validation

Every public function validates its arguments before touching hardware. Ports
are checked against the set of pins that physically exist on the device,
including the reserved entries that must never be selected; pins are bounded;
alternate-function numbers are range-checked; and output pointers are tested for
`NULL`. Invalid input produces a defined status code, never undefined
behaviour. This reflects the fault-tolerance characteristic: the system detects
bad input and reports it rather than propagating it.

### No Dynamic Memory

The driver uses only statically allocated state. Pin tracking is held in fixed
arrays sized at compile time, and the example application declares its pin
configurations as `static const` structures. There is no call to `malloc` or
any of its relatives anywhere in the module. This satisfies the dynamic-memory
category of the MISRA guidelines and supports the bounded-memory requirement
described under real-time deterministic behaviour.

### Explicit Status Reporting

No public function returns silently. Each one yields a `Status_t` value drawn
from a shared enumeration of status codes, so the caller always has a defined
way to learn whether the operation succeeded and, if not, why. This is what
makes the fail-safe pattern in the example possible: the application can react
to any error because every operation is in a position to report one.

### State Tracking for Operation Validation

The driver records which pins have been initialised and which of those are
configured as outputs. A later write or toggle is only permitted on a pin that
was actually configured as an output, and a read is only permitted on a pin that
was initialised at all. This prevents a whole class of ordering and
configuration mistakes that would otherwise be invisible until they manifested
as incorrect hardware behaviour.

## Driver Interface

The public surface of the driver is intentionally small and is declared in
[`safe_gpio.h`](app/Inc/safe_gpio.h), with the corresponding definitions in
[`safe_gpio.c`](app/Src/safe_gpio.c). It consists of seven functions:

- `GPIO_Init` brings the driver subsystem up and must be called before any other
  function. It clears the internal tracking state and marks the driver as ready.
- `GPIO_PinInit` configures a single pin from a `GPIO_Config_t` structure,
  enabling the relevant port clock and programming the mode, output type, speed,
  pull resistors, and alternate function as required.
- `GPIO_WritePin` drives an already-configured output pin to a requested state.
- `GPIO_ReadPin` reads the current state of an initialised pin into a
  caller-provided location.
- `GPIO_TogglePin` inverts the state of an already-configured output pin.
- `GPIO_DeInit` returns the driver to its uninitialised state, clearing the
  tracking arrays. It is the orderly counterpart to `GPIO_Init` and is used by
  the fail-safe handler to release the outputs.
- `GPIO_GetVersion` reports the driver version, which gives the application a
  way to record, at run time, exactly which build of the driver it is linked
  against, a small but useful contribution to traceability.

Configuration is passed by a single structure rather than by a long list of
parameters. This keeps call sites readable, allows configurations to be defined
once as constants, and makes it straightforward to review a pin's complete setup
in one place.

## Reference Application

The example in [`main.c`](app/Src/main.c) demonstrates the driver in a
realistic, if minimal, shape. It uses three pins on the target board:

- **PC13** drives a status LED that blinks on a fixed period. This is the
  "heartbeat" of the application: as long as it keeps blinking, the main loop is
  running and no fatal fault has been latched.
- **PA0** reads a user button. The button is wired active-low with an internal
  pull-up, so a pressed button reads as the reset state.
- **PB3** drives an actuator output that follows the button: it is asserted
  while the button is held and released when it is let go.

The application is structured to make the safety-oriented patterns explicit.

**Centralised, status-checked bring-up.** A single `app_gpio_init` routine
initialises the driver and every pin, checking the status of each step. If any
step fails, the system never proceeds to its main loop with a partially
configured set of I/O. The initial state of the actuator is written explicitly
rather than being left to chance, so the system has no implicit power-on
behaviour.

**Fail-safe error handling.** A dedicated handler, `app_safe_state`, is invoked
whenever any GPIO operation reports an error. It announces the cause, releases
the outputs through `GPIO_DeInit`, and then halts in a controlled manner. This
mirrors the fail-safe principle from the documentation: rather than continuing
in an unknown state, the system transitions deterministically into a known safe
one, from which a watchdog reset would recover it in a production design.

**Deterministic periodic execution.** The main loop is driven by the timebase
tick rather than by busy-waiting. The LED blink and the button-driven actuator
each run as bounded, non-blocking work inside the loop, which keeps the timing
behaviour predictable.

**Traceability.** The pin assignments are expressed as named constants and are
documented in the file header, so the mapping between the physical hardware and
the code is explicit and reviewable. The driver version is printed at start-up.

## Building

The module is built with the `arm-none-eabi` toolchain through the provided
[`Makefile`](Makefile). The driver sources live under `app/`, while the
supporting board drivers (clock, UART, timebase) live under `drivers/`. Because
the safe headers reside in [`app/Inc`](app/Inc), that directory is on the
include path alongside the CMSIS and driver headers.

```
make        # build the firmware image
make flash  # flash the target over J-Link
make clean  # remove build artefacts
```

## Scope and Future Work

This example is deliberately limited. The driver does not yet cover interrupt
configuration, atomic port-wide operations, or the full set of analogue and
alternate-function details that a production driver would need, and the
application does not include the deviation records, compliance summary, or
traceability matrix that a genuinely certifiable project would maintain. Those
process artefacts are described in *MISRA Fundamentals* and *MISRA Essential*
and are out of scope for a single illustrative module.

The intent here is narrower: to show, end to end, how an everyday peripheral can
be wrapped and consumed in a way that takes the surrounding safety documentation
seriously. Subsequent examples in this repository will build on the same
patterns for other peripherals.
