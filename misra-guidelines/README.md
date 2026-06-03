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

## Guideline Re-categorization Plans (GRP)

MISRA acknowledges that projects may have unique requirements that
necessitate adjusting or reclassifying certain guidelines to manage their
specific situations effectively. To handle this in a controlled way,
organizations may develop a **Guideline Re-categorization Plan (GRP)**,
which explicitly documents any deviation from the standard classification
of guidelines.

A well-structured GRP typically includes the following elements:

- **Original guideline classification**: whether the guideline was
  originally defined as mandatory, required, or advisory.
- **Proposed reclassification**: the new category being applied, for example
  moving a guideline from required to advisory, recorded clearly so the
  change is auditable.
- **Justification for the reclassification**: a detailed rationale covering
  the technical and project-specific reasons motivating the change.
- **Risk assessment and mitigation strategies**: an evaluation of the risks
  introduced by the reclassification and the measures applied to keep those
  risks under control.
- **Review and approval procedures**: documented approval by project leads,
  quality managers, or compliance authorities, confirming that the change
  has been formally accepted.
- **Periodic reassessment plan**: a clearly documented process for reviewing
  and re-justifying each reclassification at regular intervals, ensuring it
  remains valid as the project evolves.

A GRP enables an organization to adapt MISRA guidelines to its project-specific
context without compromising safety, reliability, or regulatory compliance.
Used responsibly, it is a controlled mechanism for engineering judgment;
used carelessly, it becomes a backdoor for weakening compliance, which is
exactly what disciplined documentation and review are meant to prevent.

## Practical Considerations for Implementing Guideline Classifications

To put MISRA guideline classifications into practice effectively, an
organization must translate the theoretical categories into concrete
process, tooling, and documentation decisions. The following four areas
cover the most important practical considerations.

### 1. Clearly Document Guideline Classifications

Maintain comprehensive records that clearly indicate which guidelines are
mandatory, required, or advisory in the context of the current project.
These classifications must be easily accessible to every member of the
development team, ideally embedded in the project's coding standard or a
linked reference document, so that developers can determine the status of
any rule without ambiguity.

### 2. Integrate Classifications into Compliance Verification Tools

Static analysis tools should be configured to clearly distinguish between
the different guideline classifications. This typically means:

- Automatically enforcing strict compliance checks for mandatory and
  required guidelines, treating their violations as build-breaking errors.
- Producing clear, distinguishable reports that indicate the classification
  of each violation, enabling targeted resource allocation and prioritization
  of corrective actions.

### 3. Prioritize Manual Reviews Accordingly

Schedule manual code reviews focusing primarily on undecidable or subjective
rules classified as mandatory or required, where automated tools cannot
reach a conclusive verdict on their own. Adherence to advisory guidelines
should be reviewed periodically as well, but with lower priority, in order
to maintain overall code quality without unnecessary expenditure of
engineering effort.

### 4. Maintain Thorough Documentation

All deviations from required guidelines must be documented thoroughly,
clearly articulating the justification, the risks introduced, and the
periodic review procedures applied to each one. For advisory guidelines,
the general adherence policy and any standing exceptions should also be
documented so that they are available for reference during audits and
inspections.

## Benefits of Proper Guideline Classification and Enforcement

Properly classifying and enforcing MISRA guidelines provides several critical
benefits that compound over the life of a safety-critical project.

**Improved Resource Efficiency**

Classification allows engineering effort and compliance verification
resources to be concentrated on the most safety-critical aspects of the
software, rather than spread evenly across rules of very different
importance.

**Enhanced Regulatory Compliance**

By implementing guideline classification and enforcement, an organization
can clearly demonstrate systematic prioritization and adherence to
safety-critical requirements, simplifying regulatory audits and certification
processes for standards such as ISO 26262, IEC 61508, and DO-178C.

**Reduced Risk of Software Failure**

Strict compliance with mandatory and required guidelines significantly
reduces the likelihood of severe software defects and system-critical
failures reaching production, where their consequences would be the most
costly to address.

**Improved Software Quality and Maintainability**

Consistent adherence even to advisory guidelines promotes readable,
maintainable, and reliable software, lowering long-term maintenance cost
and making the codebase easier to evolve safely.

Effective classification and enforcement of MISRA guidelines is foundational
to achieving sustainable, efficient, and meaningful compliance. Clearly
distinguishing mandatory, required, and advisory guidelines enables targeted
allocation of resources, streamlined verification processes, and simplified
deviation management, turning MISRA from a static set of rules into an
operational discipline integrated into the day-to-day work of the team.

## Overview of MISRA C Guidelines

This section examines several important MISRA C language-specific rules,
providing practical insight into their importance, the rationale behind
them, and strategies for effective compliance.

MISRA C rules specifically target unsafe, ambiguous, or undefined language
features that are prone to developer error and can lead to unintended
behavior or security vulnerabilities. The rules are grouped into the
following language-specific categories:

- **Type conversions and type safety**
- **Pointer and array handling**
- **Control flow and statement usage**
- **Expressions and arithmetic operations**
- **Preprocessor directives**
- **Dynamic memory management**

For each category, this section presents representative example rules,
explains the purpose of the category, and provides general compliance
recommendations.

> **Note on copyright:** The MISRA C and MISRA C++ documents are copyrighted
> publications. For that reason, this material does not reproduce the full
> rule set; only representative examples are presented under fair use to
> illustrate each category. Organizations applying MISRA in practice must
> obtain the official MISRA C and MISRA C++ documents to access the complete
> set of rules.