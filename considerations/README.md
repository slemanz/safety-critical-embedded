# Engineering Considerations for Firmware

The following best practices have consistently proven essential when developing
safety-critical firmware.

## Rigorous Development Lifecycle

Safety-critical embedded systems demand a carefully controlled and well-documented
development lifecycle. Key phases include:

**Requirements Analysis**

Clear, structured, and detailed requirement documentation is produced to precisely
specify system behavior, expected inputs and outputs, timing constraints, and
safety goals. Ambiguity at this stage is a primary source of defects downstream.

**Architecture Design**

Modular, layered architectures are employed to isolate responsibilities and allow
independent verification at each abstraction layer, reducing the risk of cascading
failures across subsystems.

**Implementation**

Adherence to industry coding standards such as MISRA C/C++, AUTOSAR C++, and
CERT C/C++ is enforced alongside thorough peer review processes.

CERT C and CERT C++ are secure coding standards developed by the CERT Coordination
Center at Carnegie Mellon University's Software Engineering Institute (SEI). Unlike
MISRA and AUTOSAR, they are not specific to embedded systems; they apply broadly
across platforms and operating systems, with a focus on eliminating code patterns
that could introduce exploitable vulnerabilities.

**Verification and Validation (V&V)**

Methodical testing regimes are applied, including unit tests, static code analysis,
integration tests, system-level tests, fault injection, and stress tests. Each
technique targets a different class of defect, and together they provide the
coverage required by safety standards.

**Deployment and Maintenance**

Comprehensive documentation is maintained to support safe updates and maintenance
throughout the product lifecycle. Any change must be traced, reviewed, and
re-validated against the original safety requirements before deployment.

## Redundant and Diverse Fault Handling

Relying on a single protection mechanism is insufficient in safety-critical design.
Redundant and diverse fault handling approaches ensure that no single point of
failure can compromise overall system safety.

Redundancy refers to duplicating critical components or execution paths so that if
one fails, another takes over without interruption. Diversity adds an extra layer
of protection by using independently designed implementations of the same function,
ensuring that a systematic design fault in one unit is unlikely to affect the other.

Together, these strategies form multiple independent protection layers, each capable
of detecting and responding to faults in adjacent layers, a principle central to
standards such as IEC 61508 and ISO 26262.
