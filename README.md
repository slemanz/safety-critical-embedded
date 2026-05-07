# Safety-Critical Embedded Systems

A safety-critical embedded system is a specialized computing system in which
failure or malfunction can directly lead to severe consequences, such as:

- Loss of life or serious injury
- Significant environmental harm
- Major financial loss or property damage
- Disruption to critical infrastructure

Unlike general-purpose systems, safety-critical systems are not designed for
convenience — they exist to preserve human life and operational integrity under
all foreseeable conditions.

The focus is therefore on deterministic behavior, fault tolerance, rigorous
verification, and strict adherence to industry-specific standards and guidelines.

## Real-World Domains of safety-critical systems

1. Aerospace and Defense:
    - Aerospace systems are perhaps the most stringent examples of safety
    critical embedded applications.
    - Systems such as avionics, flight controllers, navigation, engine
    management, and communication must function flawlessly.
    - Boeing's flight systems employ deeply embedded, safety critical firmware,
    adhering strictly to the DO-178 standards to ensure consistent reliability
    under extreme conditions.

2. Automotive Systems
    - Modern cars heavily depend on embedded software to control crucial
    subsystems, including the anti-lock braking systems (ABS), the airbag
    control unit, the engine control unit, and the advanced Driver Assistance
    Systems (ADAS).
    - The automotive industry follows rigorous standards such as the ISO 26262,
    the Misra C and C plus, plus, and other standards including Autosar Autosar
    standard for the Automotive Open System architecture.
    - These guidelines are used to ensure functional safety in every line of
    embedded code they produce.

3. Medical Devices
    - Embedded software in medical devices such as pacemakers, insulin pumps and
    MRI machines must operate without fail. Lives depend on it.
    - Such systems must adhere strictly to medical standards like the IEC 62,304
    and the FDA regulatory compliance, to minimize risks associated with
    embedded software malfunction

4. Industrial Automation
    - Industrial automation controllers, including PL robotic arms and high
    speed machining centers, manage heavy equipment and hazardous materials.
    - A minor error could result in significant human injury or costly downtime.
    Over here, standards such as the IEC 61 508 is relevant for developing and
    certifying such critical industrial automation systems.

## Contents