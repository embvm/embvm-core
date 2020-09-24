# Quality Requirement: Extensibility

* Name: Extensibility
* Reference Number: 0004
* Parent: [Resilience](0003-resilience.md)
* Children: N/A
* Category: Evolution Qualities

**Table of Contents:**

1. [Description](#description)
2. [Scenario](#scenario)
3. [Test Cases](#test-cases)
4. [Landing Zones](#landing-zones)
5. [Context](#context)
6. [Tradeoff Notes](#tradeoff-notes)
7. [Related Qualities](#related-qualities)

## Description

Extensibility is a quality which takes future growth into consideration by providing the ability to extend a system. Extensions can happen through the addition of new functionality or through modification of existing functionality.

Extensibility accepts that not everything can be designed in advance. Each chunk of the system will be workable with any changes, and the idea of change through addition is the center of the whole system design.

## Scenario

* The framework should be able to support new harware platforms within 80 hours of development time
* The framework should be able to support new harware platform variations within 20 hours of development time
* The framework should be able to support new driver API types within 8 hours of development time
* The framework should be able to support new drivers in an existing category within 20 hours of development time
* The framework should be able to be extended with optional features without requiring existing framework components to be re-written
* The framework should be able to support new RTOSes or operating systems within 20 hours of development time

## Test Cases

TBD

### Landing Zones

| Quality | Measurement | Minimum | Target | Oustanding |
|---------|-------------|---------|--------|------------|
| Custom [SPM](../components/subsystems/system_power_manager.md) scheme | minutes | 120 | 30 | 10 |
| Custom Boot Sequence | minutes | 240 | 60 | 20 |

## Context

Software systems are long lived (much longer than anyone expects when creating them), and they will need to be modified to support new features and functionalities over time. Extensiblity supports this ongoing need.

The framework’s main contribution to the end application is the architecture we define. Therefore, it’s imperative to design the framework to be as flexible and extensible as possible.

We don't want to limit the kinds of structures and programs that result from the framework. We want users to be able to come up with new structures and new ways of doing things. Experimentation and disorder and freedom should be allowed.

Extensibility imposes fewer and cleaner dependencies during development, as well as reduced coupling and more cohesive abstractions, plus well defined interfaces.

## Tradeoff Notes

Extensibility must be defined up front. Extensibility requires keeping pieces small and enabling additions to be added. Careful attention must be given to preventing coupling, so that changes in one area of the system do not impact other areas.

Extensible designs may come with performance tradeoffs.

## Related Qualities

[Reusability](0011-reusability.md) is boosted by extensibility. Reusability together with extensibility allows a technology to be transferred to another project with less development and maintenance time, as well as enhanced reliability and consistency.

The trend of increasing extensibility negatively affects [Security](0007-security.md), as extensions may introduce new flaws and vulnerabilities.

Extensibility makes a system more [Resilient](0003-resilience.md), as it can operate in an increasingly varying environment.

## Notes

A framework that addresses design challenges using design patterns is far more likely to achieve high levels of design and code reuse than one that doesn’t.
