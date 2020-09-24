# Quality Requirement: Portability

* Name: Portability
* Reference Number: 0008
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

Portability is the ability to use the same software in different environments. The prerequirement for portability is the generalized abstraction between the application logic and system interfaces.

## Scenario

* A developer compiles the same application for two separate platforms which provide the required interfaces, and the application works as expected
* A developer swaps one driver for another of the same type, and the application works as expected
* A developer utilizes a subsystem in two different applications
* A developer swaps one RTOS for another and the application works as expected

## Test Cases

TBD

### Landing Zones

N/A

## Context

The framework should help developers write firmware applications which are portable across RTOSes, processors, components, and hardware platforms.

The framework is being designed because most firmware is not written in a portable manner. This results in scheduling delays when harware changes, as well as developers re-writing previously implemented modules from scratch.

**When software with the same functionality is produced for several computing platforms, portability is the key issue for development cost reduction.**

## Tradeoff Notes

Portability requires reduced efficiency and optimization in many cases, as the software is written to be generally supported.

Portability also requires decoupling and proper interface designs. This can add additional time/storage overhead, as well as increased latency.

## Related Qualities

* The parent quality of Portability is [Resilience](0003-resilience.md)
* Portabliity makes framework developers more [Productive](0001-productivity.md) by reducing the overhead required for moving to new platforms.

## Notes

Need to take endianness into account!

Further Reading:

* [Wikipedia: Software Portability](https://en.wikipedia.org/wiki/Software_portability)
