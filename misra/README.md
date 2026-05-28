# MISRA

## Introduction

MISRA originated in the automotive industry, developed collaboratively by
automotive manufacturers and software reliability specialists in the United
Kingdom. The acronym stands for Motor Industry Software Reliability Association.

Initially formulated to reduce risks associated with embedded software in
automotive systems, MISRA guidelines have since grown into a widely adopted
standard applied across many other industries, including aerospace, medical
devices, and industrial automation.

At its core, MISRA aims to enhance software safety by defining clear, structured
coding practices that eliminate risky and undefined behaviors inherent in C and
C++ programming. By following these guidelines, development teams systematically
reduce the possibility of dangerous or unpredictable runtime outcomes, ultimately
improving the safety and reliability of the resulting firmware.

## Why MISRA Matters

Although MISRA was initially focused on the automotive industry, its impact now
spans multiple sectors, including aerospace, medical devices, industrial
automation, railway systems, defense, and even consumer electronics. The broad
adoption of MISRA guidelines across these domains highlights both their
versatility and their relevance to modern embedded development.

**Enhanced Safety and Reliability**

MISRA guidelines are carefully formulated to eliminate language constructs known
to cause errors or unintended behavior. Adhering to them significantly reduces
the rate of software faults and ensures systems perform predictably under
real-world conditions.

**Improved Readability and Maintainability**

MISRA-compliant code fosters clarity and consistency, resulting in more readable
and maintainable codebases. Code that is easier to understand also enables
smoother collaboration across teams and accelerates the onboarding of new
developers.

**Simpler Verification and Certification**

The use of MISRA simplifies the verification and validation processes required
by many safety-critical standards, such as ISO 26262, IEC 61508, and DO-178C.
Achieving MISRA compliance provides a strong foundation for the certification
activities mandated by these standards.

**Reduced Legal and Regulatory Exposure**

From a legal and regulatory perspective, documented adherence to MISRA
guidelines can significantly mitigate liability concerns. Organizations that
implement and demonstrate rigorous compliance with recognized safety standards
can effectively limit their exposure should an incident occur.

## Real-World Impact

Several use cases demonstrate the practical effectiveness of MISRA across
different industries.

**Automotive**

MISRA guidelines underpin the software that controls braking systems, airbags,
power steering, and engine management. By rigorously adhering to these
guidelines, automotive manufacturers have substantially reduced the number of
software-induced vehicle recalls reported each year.

**Aerospace**

Embedded avionics systems developed under MISRA exhibit increased reliability,
which is crucial for safety-critical functions such as navigation, autopilot
control, and sensor data processing. Many aerospace companies now mandate MISRA
compliance as part of their standard software development lifecycle.

**Medical Devices**

In devices such as insulin pumps and cardiac pacemakers, MISRA compliance helps
ensure that embedded software meets the high safety standards required to
operate continuously and predictably inside or alongside the human body.

**Industrial Automation**

In factory automation, robotics, and safety-critical machinery, MISRA provides
a clear framework for writing robust, fault-resistant embedded software capable
of operating reliably in environments where failures can cause serious injury
or significant economic loss.

## Overview of Rules and Directives

MISRA guidelines are organized into two main categories: **rules** and
**directives**. Understanding the distinction between them is essential to
applying MISRA correctly in practice.

**Rules**

Rules are technical restrictions directly associated with specific language
constructs in C or C++. They are further classified as:

- **Decidable**: can be fully verified by static analysis tools. A compliant
  tool can determine with certainty whether the rule is followed for any given
  piece of source code.
- **Undecidable**: cannot be reliably verified by static analysis alone and
  generally require manual review, additional context, or runtime evidence
  to confirm compliance.

**Directives**

Directives are broader requirements that affect the software development
process itself rather than language constructs exclusively. They address
practices such as configuration management, documentation, traceability, and
the use of compilers and tooling.

Together, rules and directives form a comprehensive approach to safety: rules
constrain what the code may do, while directives constrain how the code is
written, tested, and documented. Applying both is what makes MISRA effective
as a safety framework rather than just a coding style guide.

## Overview of Compliance and Enforcement

To effectively achieve MISRA compliance, organizations must develop structured
processes that go well beyond simply running a static analysis tool over the
codebase. A robust MISRA compliance framework typically involves the following
elements:

- **Clearly defined coding standards**: a documented, project-specific
  interpretation of which MISRA rules and directives apply, including any
  permitted deviations and their rationale.
- **Rigorous static analysis tool usage**: certified or qualified tools such
  as LDRA, Parasoft, or Helix QAC are integrated into the build pipeline to
  automatically flag violations as code is written.
- **Manual review processes**: peer reviews and structured inspections cover
  the undecidable rules and contextual concerns that automated tools cannot
  evaluate on their own.
- **Deviation management systems**: every deviation from a rule is formally
  recorded, justified, reviewed, and approved, ensuring that exceptions are
  controlled rather than accidental.
- **Comprehensive documentation**: traceable records linking requirements,
  source code, analysis results, deviations, and approvals provide the
  evidence required during audits and certification activities.

## Historical Evolution

The evolution of embedded software development has been remarkable, driven by
the steady increase in system complexity and stricter safety requirements
across nearly every industry. As a consequence, clearly defined coding
standards have become an essential part of the development lifecycle, and
MISRA has emerged as the leading standard for creating robust and safe
embedded software.

### Origins

In the early 1990s, the UK automotive industry faced significant challenges
related to software reliability. To address these issues, a consortium known
as MISRA (Motor Industry Software Reliability Association) was formed, with
the initial goal of producing coding guidelines for automotive applications.

### MISRA C

- **MISRA C:1998**: the first edition of the standard, addressing crucial
  vulnerabilities inherent in the C programming language and establishing the
  foundation of MISRA coding discipline.
- **MISRA C:2004**: expanded the rule set and clarified ambiguities present
  in the original edition, gaining widespread acceptance well beyond the
  automotive sector. The guidelines became instrumental in safety-critical
  industries such as aerospace, medical devices, industrial automation, and
  defense.
- **MISRA C:2023**: the current consolidated version, published in May 2023.
  It incorporates all previous amendments and updates, including support for
  features introduced in C11 and C18.

### MISRA C++

- **MISRA C++:2008**: the first edition of the C++ guidelines, targeting the
  language features available at the time.
- **MISRA C++:2023**: a significant revision aligned with modern C++ standards
  and integrating the AUTOSAR C++14 guidelines, effectively unifying two of
  the most influential automotive C++ coding standards into a single coherent
  reference.

## Distinguishing Between MISRA C and MISRA C++

While MISRA C and MISRA C++ share the same overarching goal of preventing
dangerous coding practices and ensuring software reliability, they differ
significantly in their specific guidelines and coverage due to the fundamental
differences between the two languages.

### MISRA C

MISRA C provides rules tailored to the characteristics of the C language,
organized around the following areas:

- **Type safety and arithmetic operations**: rules designed to prevent
  unintended consequences of type conversions, signed/unsigned arithmetic,
  and integer overflow.
- **Pointers and arrays**: restrictions on pointer arithmetic, dereferencing
  invalid pointers, and ensuring array accesses remain within their defined
  bounds.
- **Control flow integrity**: rules governing loops, conditional statements,
  and `goto` usage to prevent undefined behavior or infinite loops.
- **Preprocessor usage**: restrictions on macro definitions to prevent
  unexpected side effects and improve code clarity.
- **Dynamic memory allocation**: recommendations to avoid or strictly manage
  dynamic allocation in order to ensure deterministic runtime behavior.

MISRA C is highly prescriptive and widely adopted for real-time,
resource-constrained systems, with guidelines explicitly designed to be
enforced by static analysis tools.

### MISRA C++

MISRA C++ addresses the additional complexity introduced by object-oriented
programming, templates, exception handling, and modern C++ language features:

- **Resource management**: rules for the safe use of RAII (Resource
  Acquisition Is Initialization) to manage resources deterministically and
  prevent leaks.
- **Inheritance and polymorphism**: restrictions on complex inheritance
  hierarchies, virtual function usage, and polymorphic behavior to ensure
  predictable runtime semantics.
- **Templates and generic programming**: rules ensuring templates are used
  safely, avoiding excessive code bloat and preserving deterministic
  behavior.
- **Exception handling**: clear guidelines on whether exceptions are
  permitted in a given project and, when permitted, how to use them safely.
  Alternative deterministic error-handling approaches are also covered.
- **Modern language constructs**: rules and recommendations covering modern
  C++ features such as strongly typed enums (`enum class`), `constexpr`,
  `noexcept`, and smart pointers.

MISRA C++ recognizes both the strengths and the potential pitfalls of modern
C++, and provides carefully structured rules that allow teams to leverage
advanced features without compromising system safety or reliability.

### Shared Objectives

Despite their language-specific differences, MISRA C and MISRA C++ share the
following common objectives:

1. **Defect prevention**: eliminating ambiguous, undefined, or risky coding
   practices at the source.
2. **Consistency and maintainability**: enforcing coding practices that
   increase reliability, maintainability, and ease of verification.
3. **Facilitating certification**: providing a foundational component for
   regulatory compliance and certification activities in safety-critical
   industries.

### Choosing Between MISRA C and MISRA C++

Selecting the appropriate standard depends on the specific project
constraints, the expertise of the development team, the hardware
capabilities of the target platform, and the language features actually
required.

In practice, choosing between MISRA C and MISRA C++ is equivalent to
choosing between Embedded C and Embedded C++:

- **Embedded C with MISRA C** is typically preferred for projects with
  simpler hardware and stricter resource constraints, where the additional
  language complexity of C++ would not justify its overhead.
- **Embedded C++ with MISRA C++** is typically adopted for larger and more
  complex systems that benefit from object-oriented programming and require
  more flexible abstraction mechanisms.