# MISRA Guidelines

## Why Classification Matters

MISRA guidelines provide the foundation for embedded software safety,
robustness, and reliability. However, not all guidelines carry the same
importance or enforceability. To help teams prioritize compliance efforts
appropriately, MISRA classifies its guidelines into distinct categories.
Understanding these classifications is essential for effective enforcement,
resource allocation, and practical compliance management.

Embedded software projects, particularly those in safety-critical domains,
must address a large number of compliance requirements and coding constraints
simultaneously. Classifying MISRA guidelines provides a structured approach to
prioritizing those efforts according to each guideline's potential impact on
safety, quality, and regulatory acceptance. The classification system
specifically enables:

- Clear prioritization of resources toward the most critical rules.
- More efficient use of compliance verification tools and manual reviews.
- Simplified compliance assessment, documentation, and audit activities.
- Informed decision making regarding which deviations are permissible and
  which are not.

It is important to note that this classification is **orthogonal** to the
earlier distinctions between rules and directives, and between decidable and
undecidable rules. A single guideline can therefore be characterized along
several axes: for example, a rule may be both *decidable* and *mandatory*,
or a directive may be both *process-oriented* and *advisory*.

## Categories of MISRA Guidelines

MISRA guidelines are primarily organized into three categories: **mandatory**,
**required**, and **advisory**. Each carries a distinct meaning regarding
enforceability and priority.

### Mandatory Guidelines

Mandatory guidelines represent rules that organizations must always enforce
without exception. They are essential for ensuring software correctness,
safety, and regulatory compliance, and failure to comply typically indicates
a critical flaw with significant safety implications, regulatory
non-compliance risks, or potential software failure.

Mandatory guidelines:

- Require rigorous and comprehensive compliance verification.
- Permit **no deviations** under any circumstances.
- Demand strict documentation and audit evidence.

A representative example would be:

*"All code shall comply with the ISO standard for the C programming language
(syntax and semantics)."*

### Required Guidelines

Required guidelines represent rules that organizations are strongly expected
to enforce. They have a significant impact on software quality and
reliability, but deviations are permissible in exceptional, well-documented
circumstances, typically due to specific project constraints or hardware
limitations.

Required guidelines:

- Should be enforced consistently across the project.
- Permit **controlled deviations** under clearly documented and justified
  circumstances.
- Require thorough documentation of every deviation and periodic
  reassessment of its justification.

A representative example would be:

*"Explicitly signed or unsigned types shall be used in arithmetic operations
to avoid unexpected type promotion issues."*

### Advisory Guidelines

Advisory guidelines recommend best practices intended to improve software
maintainability, readability, and clarity. They carry the lowest safety risk
but contribute significantly to overall software quality when applied
consistently.

Advisory guidelines:

- Encourage best practices and improved coding clarity.
- Permit deviations, typically without extensive documentation requirements.
- Still contribute meaningfully to maintainability and code quality when
  followed across the project.

A representative example would be:

*"Use explicit parentheses in complex arithmetic expressions for clarity."*
