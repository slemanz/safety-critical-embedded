# Safe UART

This directory contains a small, self-contained UART debug module for the
STM32F411 together with a reference application that exercises it. As with the
companion `safe-gpio` example, its purpose is not to be a feature-complete HAL,
but to serve as a worked example of how the principles described in the rest of
this repository (fault tolerance, fail-safe behaviour, explicit status
reporting, bounded memory, and MISRA C alignment) translate into actual firmware
for a peripheral that almost every embedded project ends up needing: a serial
console.

It is the second example in the series. Where `safe-gpio` wrapped the simplest
peripheral a developer touches, this module adds the next layer of realism: a
stateful, bidirectional peripheral whose operations can legitimately be *not
ready yet*, which forces the interface to express "in progress" as a
first-class, defined outcome rather than blocking the whole system.

## Source Layout

The pieces referenced throughout this document live at the following paths:

- [`app/Inc/uart_debug.h`](app/Inc/uart_debug.h) - the public module interface:
  the configuration structure and the function declarations, each documented
  with its full set of return codes.
- [`app/Src/uart_debug.c`](app/Src/uart_debug.c) - the implementation, including
  the pin and peripheral set-up, the transmit path, and the non-blocking receive
  line assembly.
- [`app/Inc/safe_status.h`](app/Inc/safe_status.h) - the shared `Status_t`
  status codes returned by every public function.
- [`app/Src/main.c`](app/Src/main.c) - the reference application: a tiny
  line-based console built entirely on the module.
- [`Makefile`](Makefile) - the build definition for the toolchain.

## Motivation

A serial console is usually the first thing brought up on a new board and the
last thing trusted in the field, which makes it an instructive peripheral to
wrap carefully. The naive approach, poking the `USART` registers directly and
relying on the C library's `printf` for output, works on the bench and hides
several properties that matter in a safety context: where memory comes from,
how long an operation can take, and what happens when the channel is not ready.

The `uart_debug` module wraps the raw register access behind a validated
interface, makes the cost of every operation explicit, and - importantly -
provides its own formatted-output function instead of leaning on the standard
library. The reasoning behind that last decision is significant enough to
deserve its own section below.

## Design Principles

The module was built around the same characteristics the repository identifies
as essential for safety-critical firmware. Each principle maps back to a concept
documented elsewhere in this project.

### Input Validation

Every public function validates its arguments before touching hardware.
`UART_Init` rejects an unsupported frame format (data bits, stop bits, parity)
and - the bug most easily overlooked - a zero baud rate, which would otherwise
cause a divide-by-zero in the baud-divider calculation. Output and string
pointers are tested for `NULL`. Invalid input produces a defined status code,
never undefined behaviour, which is a direct application of the fault-tolerance
characteristic.

### No Dynamic Memory

The module uses only statically allocated state. The receive line is assembled
in a fixed module-level buffer sized at compile time, and `UART_Printf` formats
into a fixed buffer on the stack. There is no call to `malloc` or any of its
relatives anywhere in the module. This satisfies the dynamic-memory category of
the MISRA guidelines and supports the bounded-memory requirement described under
real-time deterministic behaviour. It is also the central reason the module
ships its own `printf` - see below.

### Explicit Status Reporting

No public function returns silently. Each one yields a `Status_t` value, so the
caller always has a defined way to learn whether the operation succeeded and, if
not, why. Crucially, "no data yet" is *not* an error: the receive functions
return `STATUS_PENDING` to distinguish a perfectly normal not-ready-yet
condition from a genuine fault. This is what lets the reference application poll
the console inside a non-blocking loop without ever mistaking idleness for
failure.

### Non-Blocking Receive

`UART_GetChar` never spins waiting for a byte; it reports `STATUS_PENDING` when
the receive register is empty. `UART_GetLine` builds on it, accumulating
characters into the module's line buffer across many calls, handling backspace
and end-of-line, and returning `STATUS_OK` only once a complete line is ready.
The caller is therefore never blocked by the human at the keyboard, and the main
loop remains free to do other periodic work - the same deterministic,
bounded-iteration discipline used in the `safe-gpio` example.

(The transmit path, by contrast, *does* busy-wait on the transmit register. That
is a deliberate simplification for a debug channel; bounding those waits with a
timeout that returns `STATUS_ERROR_TIMEOUT` is noted under *Scope and Future
Work*.)

## Why a Custom `printf`

The most common way to get formatted output on an embedded target is to keep
the standard `printf` and "retarget" it: the C library ultimately calls the
`_write` system call, and you provide a `_write` (or `__io_putchar`) that pushes
bytes out of the UART. This repository even contains the conventional plumbing
for that approach in `drivers/`. It is convenient, and for a hobby project it is
fine. For a module that is meant to illustrate safety-oriented practice, it
quietly violates several of the very principles listed above:

- **It hides dynamic memory.** The reentrant `newlib` `printf` allocates its
  internal buffering and per-stream reentrancy state from the heap, through
  `_sbrk`. A module that has carefully avoided `malloc` everywhere else
  reintroduces it the moment it calls `printf`. Bounded, static memory is no
  longer something you can prove by inspection.

- **It is not deterministic or small.** The full `printf` machinery is large and
  its code path varies with the format string. Both the code footprint and the
  execution time become hard to bound - exactly the properties real-time,
  safety-critical code is required to pin down.

- **It cannot report failure to its caller.** `printf` returns a character
  count, not a domain status. If the underlying channel is unavailable there is
  no clean way for the caller to find out and react. Every other function in
  this module returns a `Status_t`; a formatted-output function that could not
  would be the odd one out.

- **It couples the whole program to one hidden channel.** Retargeting is global:
  every `printf` anywhere in the firmware is silently wired to the one UART that
  `_write` happens to drive. There is no way to address output explicitly or to
  reason about it locally.

`UART_Printf` takes the opposite, explicit route. It formats into a
fixed-size stack buffer with `vsnprintf` - which performs the conversion without
allocating for the format specifiers used here - bounds the result to that
buffer, checks the return value (a negative result is an encoding error and is
reported as `STATUS_ERROR_GENERIC` instead of transmitting indeterminate
memory), and then sends the bytes through the module's own `UART_SendString`.
The result is self-contained: formatting and transport are both visible in the
module, memory stays static and bounded, and the function returns a `Status_t`
like everything else.

Two honest caveats. First, the module still relies on the C library's
`vsnprintf` for the formatting itself; pulling in floating-point conversions in
particular will grow the code, so a fully certifiable build would use a
restricted or in-house formatter. Second, output longer than the buffer is
*truncated* rather than wrapped or overflowed - truncation is defined,
inspectable behaviour, but it is a limitation the caller must be aware of.

## Module Interface

The public surface is intentionally small and is declared in
[`uart_debug.h`](app/Inc/uart_debug.h), with the corresponding definitions in
[`uart_debug.c`](app/Src/uart_debug.c):

- `UART_Init` validates the requested frame format, configures the USART2 pins
  (PA2/PA3) and the peripheral, and marks the module ready. It must be called
  before any other function.
- `UART_SendString` transmits a null-terminated string, byte by byte.
- `UART_Printf` produces formatted output under the bounded, status-returning
  discipline described above.
- `UART_IsCharAvailable` reports, without consuming anything, whether a received
  byte is waiting.
- `UART_GetChar` reads a single byte if one is available, returning
  `STATUS_PENDING` otherwise.
- `UART_GetLine` assembles a complete line across repeated non-blocking calls,
  echoing input and handling backspace, and yields the line only when it is
  complete.
- `UART_DeInit` disables the transmitter, receiver, peripheral and its clock,
  clears the line state, and returns the module to its uninitialised state. It
  is the orderly counterpart to `UART_Init` and is used by the fail-safe handler.

Configuration is passed by a single `UART_Config_t` structure rather than by a
long parameter list, which keeps call sites readable and lets a console's
complete set-up be defined once as a constant.

## Reference Application

The example in [`main.c`](app/Src/main.c) demonstrates the module as a tiny
line-based console at 115200 baud, 8N1. It is structured to make the
safety-oriented patterns explicit.

**Status-checked bring-up.** `UART_Init` is checked before anything else
happens. Because the console is both the thing being demonstrated and the only
channel through which the application could report a problem, a failure here goes
straight to the fail-safe state rather than attempting to carry on.

**Fail-safe error handling.** A dedicated `app_safe_state` handler releases the
peripheral through `UART_DeInit` and halts in a controlled manner. It
deliberately prints nothing: the channel it would use to announce the fault is
the one that failed. In a production design a watchdog would recover the system
from this state.

**Non-blocking main loop.** The loop calls `UART_GetLine`, which returns
`STATUS_PENDING` until a full line has been typed. The application acts only on
`STATUS_OK`, ignores the pending case, and treats any other status as fatal. The
human at the keyboard therefore never stalls the system, and other periodic work
could be slotted into the same loop.

**Output through the module only.** Every byte the application emits goes through
`UART_SendString` or `UART_Printf`; the C library's `printf` is not used at all,
keeping the example consistent with the reasoning above.

## Building

The module is built with the `arm-none-eabi` toolchain through the provided
[`Makefile`](Makefile). The module sources live under `app/`, while the
supporting board drivers (clock, UART plumbing, timebase) live under `drivers/`.
Because the module headers reside in [`app/Inc`](app/Inc), that directory is on
the include path alongside the CMSIS and driver headers.

```
make        # build the firmware image
make flash  # flash the target over J-Link
make clean  # remove build artefacts
```

## Scope and Future Work

This example is deliberately limited. The transmit path busy-waits rather than
bounding its waits with a timeout (`STATUS_ERROR_TIMEOUT`); the receive path is
polled rather than interrupt- or DMA-driven; and the module covers a single
fixed USART instance. The frame-format validation also accepts a 7-data-bit, no
-parity combination that the STM32F4 hardware cannot represent literally (it has
no 7-bit word length), so that case is silently configured as 8 bits - a
limitation worth either rejecting or documenting in a production driver. None of
the process artefacts of a certifiable project (deviation records, compliance
summary, traceability matrix) are included here; those are described in *MISRA
Fundamentals* and *MISRA Essential* and are out of scope for a single module.

The intent here is narrower: to show, end to end, how a stateful, bidirectional
peripheral - and the formatted output that inevitably comes with it - can be
wrapped and consumed in a way that takes the surrounding safety documentation
seriously. Subsequent examples in this repository will build on the same
patterns for other peripherals.
