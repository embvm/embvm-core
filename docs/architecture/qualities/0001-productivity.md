# Quality Requirement: Productivity

* Name: Productivity
* Reference Number: 0001
* Parent: N/A
* Children:
	* [Stability](0005-stability.md)
	* [Reusability](0011-reusability.md)
	* [Documentation](0010-documentation.md)
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

Developers should be able to easily use the system. Developers should be able to create, debug, and test firmware more effectively with the framework than without it.

## Scenario

* New user compiles and runs an example framework program successfully with no errors or problems
* New user reads through the framework introduction and understands the basic organization and operational theory
* New user reads through a tutorial and creates a project skeleton which compiles successfully
* Developer enables a subsystem in the software layer and it works correctly on the first try

## Test Cases

* New user reads/watches introductory material and explains the framework concepts to us successfully
* Time to add a new driver that corresponds to an existing category
* Time to add a new driver category
* Time to add a new platform
* Developer enables and uses a subsystem for a platform/application

### Landing Zones

<center>

| Quality | Measurement | Minimum | Target | Oustanding |
|---------|-------------|---------|--------|------------|
| Setup and run example program | minute | 120 | 30 | 10 |
| Add new driver to existing category | hour | 40 | 20 | 12 |
| Enable & use subsystem | minutes | 120 | 30 | 10 |
| New project setup | minutes | 120 | 30 | 10 |
| New platform setup | hours | 80 | 40 | 12 |
| Familiarity with Framework | day | 14 | 5 | 1 |

</center>

## Context

Embedded developers must be able to ship products faster than the framework than they could without it. If the framework does not make developers more productive, we do not have a winning proposition.

A real risk for the framework is that we create a system that only experts can understand and use effectively.

## Tradeoff Notes

* Productivity can be at odds with [Resilience](0003-resilience.md), which may require complex mechanisms.

## Related Qualities

* [Extensibility](0004-extensibility.md) enables developers to extend the system to meet their needs
* [Serviceability](0012-serviceability.md) enables developers to debug more effectively
* Child qualities directly contribute to developer productivity:
    * [Stability](0005-stability.md)
    * [Reusability](0011-reusability.md)
    * [Documentation](0010-documentation.md)

## Notes

N/A
