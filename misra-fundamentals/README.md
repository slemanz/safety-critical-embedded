# MISRA Fundamentals

## Rules vs Directives

Before adopting MISRA guidelines within a software development workflow, it is
important to understand several fundamental concepts that underpin MISRA
compliance. These include the distinction between rules and directives, the
difference between decidable and undecidable rules, and how the scope of code
is considered during compliance assessment.

A clear understanding of these concepts significantly improves a team's
ability to apply MISRA standards effectively in real-world safety-critical
software projects.

MISRA compliance is built on two primary forms of guidance: **rules** and
**directives**. Both categories are essential and complementary, even though
they serve distinct purposes.

### Rules

Rules are prescriptive, clearly defined coding requirements that apply
directly to the source code. They typically address specific language
constructs and aim to remove ambiguity, dangerous coding patterns, and
undefined behavior. Rules form the backbone of MISRA compliance by providing
clear and unambiguous criteria for writing safe code.

Representative examples of MISRA rules include:

- *"Pointers shall not be used to access objects beyond their defined
  boundaries."*
- *"Signed integer arithmetic shall not lead to overflow."*
- *"Unreachable code shall not be present in the project."*

### Directives

Directives, on the other hand, are broader, process-oriented guidelines that
affect the software development lifecycle rather than the code itself. They
specify how the software should be designed, documented, or tested to ensure
clarity, correctness, and maintainability. As a result, directives typically
require a well-defined and documented development process.

Representative examples of MISRA directives include:

- *"All source code shall be traceable back to clearly defined requirements."*
- *"All deviations from MISRA guidelines shall be documented and justified."*
- *"Static analysis tools shall be configured and used to support MISRA
  compliance."*

Understanding the distinction between rules and directives is essential for
the effective implementation of MISRA compliance: rules govern *what* the
code may do, while directives govern *how* the development process must be
carried out.

## Decidable vs Undecidable Rules

Within MISRA rules, there is an additional important distinction between
**decidable** and **undecidable** rules. This classification relates to
whether a given rule can be automatically checked with certainty by tooling
or whether it requires human judgment to evaluate.

### Decidable Rules

Decidable rules are those whose compliance can be unambiguously verified
through static analysis tools. A typical example would be:

*"The size of all array types shall be explicitly defined at the point of
declaration."*

A rule of this nature is straightforward for automated tools to enforce.
Static analysis can reliably determine, for any piece of source code,
whether the rule is followed or violated.

### Undecidable Rules

Undecidable rules are those that static analysis tools alone cannot verify
conclusively. They typically require manual review, human judgment, or
additional context that lies outside the immediate code under analysis.

A typical example would be:

*"All code shall be structured for clarity and ease of review."*

While a static analysis tool may identify certain structural concerns, the
subjective nature of clarity and ease of review ultimately requires human
assessment to determine compliance.

### Combining Both in Practice

Effective MISRA compliance combines automated static analysis with structured
manual reviews. Automated tools provide efficiency and complete coverage of
decidable rules, while human reviewers handle the contextual and subjective
aspects of undecidable rules. Together they achieve the thoroughness required
for genuine, demonstrable compliance.

## Single Translation Unit vs System-Wide Scope

Another foundational concept in MISRA is understanding how compliance is
assessed in terms of code scope, specifically the difference between
assessment at the **single translation unit** level and across the
**system-wide** context.

### Single Translation Unit Scope

A single translation unit refers to one source file together with all of its
included headers, compiled independently by the compiler. MISRA rules checked
at this level focus on correctness and safety within the isolated context of
that compilation unit and typically address concerns such as data types, type
conversions, and local variable usage.

A representative example would be:

*"All automatic variables shall have explicitly defined initial values."*

This rule can be fully evaluated by examining a single file in isolation,
without needing knowledge of any other part of the system.

### System-Wide Scope

System-wide compliance requires evaluating code across multiple translation
units or the entire software project. This broader perspective is necessary
to ensure consistency between modules, correct linkage between components,
controlled use of global variables, and well-defined cross-module interfaces.

A representative example would be:

*"External objects shall be declared consistently across all translation
units."*

A rule of this nature cannot be evaluated from a single file alone, as it
inherently depends on how declarations and definitions interact across the
whole project.

### Defining Scope in Practice

Effective MISRA compliance strategies require clearly identifying which rules
apply at each scope and systematically ensuring their enforcement throughout
the software lifecycle. Misclassifying a system-wide rule as a local concern
is a common source of compliance gaps, as the tool may report no violations
within a single file while the project as a whole remains non-compliant.