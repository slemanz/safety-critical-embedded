# Case Studies

The following case studies examine the architecture and design decisions of
well-known safety-critical systems, illustrating how the principles covered
throughout this repository are applied in practice.

## NASA Mars Rovers (Curiosity and Perseverance)

NASA's Mars rovers represent some of the most demanding safety-critical embedded
systems ever deployed. Operating millions of kilometers from Earth, with
communication delays of up to 24 minutes each way, they must make autonomous
decisions and recover from faults entirely without human intervention.

**Layered Redundancy and Fault Recovery**

Curiosity and Perseverance are designed with layered redundancy and robust fault
recovery mechanisms. Software fault tolerance is built in to handle both transient
faults, caused by radiation-induced bit flips, and permanent hardware failures.
Fault injection testing is applied during development to verify reliability under
the extreme conditions of the Martian environment.

**Deterministic Timing and Power Management**

Deterministic timing is critical for coordinating rover operations, ensuring
precise control of tasks such as navigation, instrument sequencing, and
communication windows. Power management is rigorously implemented to optimize
energy consumption from the rover's Radioisotope Thermoelectric Generator (RTG),
which provides a continuous but strictly limited power budget.

**Compliance with NASA Guidelines**

Development follows NASA's stringent software safety requirements, documented in
NPR 7150.2 (NASA Software Engineering Requirements) and the NASA Software
Engineering Handbook. An independent Verification and Validation (IV&V) program
provides an objective assessment that safety-critical systems operate reliably
and meet their specifications.

**Testing Under Simulated Conditions**

Extensive testing under simulated extreme conditions is a core part of NASA's
validation approach. Environmental simulations cover temperature extremes,
radiation exposure, and terrain challenges representative of the Martian surface,
ensuring that failure modes are identified and addressed well before launch.
