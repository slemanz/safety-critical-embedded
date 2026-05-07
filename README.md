# Safety-Critical Embedded Systems

A safety-critical embedded system is a specialized computing system in which
failure or malfunction can directly lead to severe consequences, such as:

- Loss of life or serious injury
- Significant environmental harm
- Major financial loss or property damage
- Disruption to critical infrastructure

Unlike general-purpose systems, safety-critical systems are not designed for
convenience. They exist to preserve human life and operational integrity under
all foreseeable conditions.

The focus is therefore on deterministic behavior, fault tolerance, rigorous
verification, and strict adherence to industry-specific standards and guidelines.

## Real-World Domains of Safety-Critical Systems

1. **Aerospace and Defense**
   - Aerospace is arguably the most demanding domain for safety-critical embedded
     systems, where failures at altitude are often catastrophic and irreversible.
   - Core systems such as avionics, flight control computers, navigation, engine
     management, and communication links must operate flawlessly under extreme
     temperature, vibration, and electromagnetic interference.
   - Development is governed by **DO-178C** (*Software Considerations in Airborne
     Systems and Equipment Certification*), which defines software levels (DAL A–E)
     based on the severity of a potential failure's effect on the aircraft and its
     occupants.

2. **Automotive Systems**
   - Modern vehicles depend on embedded software to control safety-critical
     subsystems, including Anti-lock Braking Systems (ABS), airbag control units,
     Engine Control Units (ECUs), and Advanced Driver Assistance Systems (ADAS).
   - The primary governing standard is **ISO 26262** (*Road Vehicles: Functional
     Safety*), which introduces Automotive Safety Integrity Levels (ASIL A–D) to
     classify and manage risk throughout the development lifecycle. Coding practices
     follow **MISRA C** and **MISRA C++** guidelines to prevent undefined behavior,
     while **AUTOSAR** (*AUTomotive Open System ARchitecture*) provides a
     standardized software architecture for ECU development across vendors.

3. **Medical Devices**
   - Embedded software in devices such as pacemakers, insulin pumps, and MRI
     machines must operate without fail, as patient lives depend directly on their
     correctness.
   - **IEC 62304** (*Medical Device Software: Software Life Cycle Processes*)
     classifies software by safety class (A, B, or C) and mandates corresponding
     lifecycle requirements for design, testing, and maintenance. Devices marketed
     in the United States must also comply with **FDA 21 CFR Part 820**, the Quality
     System Regulation covering design controls and risk management.

4. **Industrial Automation**
   - Industrial controllers, including PLCs (Programmable Logic Controllers),
     robotic arms, and high-speed machining centers, manage heavy equipment and
     hazardous materials in environments where a minor software fault can cause
     serious injury or costly unplanned downtime.
   - **IEC 61508** (*Functional Safety of Electrical/Electronic/Programmable
     Electronic Safety-Related Systems*) is the foundational standard for this
     domain. It defines Safety Integrity Levels (SIL 1–4) as the basis for
     developing and certifying safety-related industrial systems across all sectors.

## Contents