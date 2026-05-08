# Key Safety-Critical Standards and Guidelines

Professionally developed embedded firmware for safety-critical applications must
comply with established international standards that enforce deterministic behavior,
traceability, and documented processes throughout the entire development lifecycle.

**ISO 26262 (Automotive)**

- Titled "Road Vehicles: Functional Safety", this standard defines functional safety
  requirements specifically for automotive electronic and electrical systems.
- It mandates systematic procedures covering requirements specification, design,
  implementation, verification, validation, and production.
- A key requirement is full traceability at each development step, ensuring every
  design decision can be linked back to a safety requirement.

**MISRA C / MISRA C++ Guidelines**

- Published by the Motor Industry Software Reliability Association (MISRA), these
  guidelines define coding rules for C and C++ respectively, targeting safe and
  reliable embedded software development.
- Their focus is on eliminating undefined, unspecified, and implementation-dependent
  behavior, which are common sources of subtle and hard-to-reproduce defects in C
  and C++.

**AUTOSAR C++ Guidelines**

- The AUTOSAR C++ guidelines provide detailed rules for the safe use of modern C++
  in automotive and related safety-critical systems, building upon and extending
  MISRA C++.
- Rules are clearly classified as required, recommended, or advisory, and are
  designed to support both automated static analysis tools and manual code reviews.

**DO-178C (Aerospace Software Certification)**

- Formally titled "Software Considerations in Airborne Systems and Equipment
  Certification", DO-178C is the primary aerospace standard governing software
  lifecycle processes for airborne systems.
- Adherence to DO-178C ensures meticulous requirements traceability, comprehensive
  verification coverage, and high levels of reliability across all defined software
  levels (DAL A through E).

**IEC 61508 (Industrial Functional Safety)**

- This foundational standard defines functional safety requirements for electrical,
  electronic, and programmable electronic safety-related systems across industries.
- It provides best practices for reliability engineering, systematic safety analysis,
  lifecycle management, and risk-based approaches using Safety Integrity Levels
  (SIL 1 through 4).

**IEC 62304 (Medical Device Software)**

- IEC 62304 specifies lifecycle processes for medical device software development,
  classifying software into safety classes (A, B, or C) based on the potential
  severity of a failure.
- It enforces stringent safety measures and structured management practices
  throughout software design, verification, and deployment.
