# Quality Requirement: Reliability

* Name: Reliability
* Reference Number: 0002
* Parent: N/A
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

Reliability describes the ability of a system or component to function under stated conditions for a specified period of time. Reliability can also indicate the resistance to failure of a system over time.

Reliability is a main characteristic that contains:

* Maturity: Frequency of failure of the software
* Fault tolerance: The ability of software to withstand (and recover) from component or environmental failures
* Recoverability: Ability to bring back a failed system to full operation, including data and network connections

## Scenario

* When compiled with the following settings, zero warnings will be produced:
	* -Weverything for Clang
	* /W4 for MSVC
	* -Wall -Wextra -pedantic
* When an issue with the framework is discovered, the root cause will be identified and logged, a test will be added, and the root cause will be addressed

## Test Cases

* Test applications exercising the framework core structures should run continuously for a minimum of 14 days
* Framework reboot cycle tests should result in no panics/assertions

### Landing Zones

| Quality | Measurement | Minimum | Target | Oustanding |
|---------|-------------|---------|--------|------------|
| Bug Reports per Release | # bugs | 8 | 2 | 0 |
| Duration Core can run w/o failure/reboot (MTBF) | days | 14 | 30 | 365 |
| Successful reboots | % | 95 | 98 | 100 |

## Context

Users will not trust the framework if it is buggy and unreliable.

We want to use the framework to apply our knowledge, experience, and techniques knowledge to prevent or to reduce the likelihood and frequency of failures.

When failures are identified, we want to root cause them to understand the issue and prevent similar issues from happening in the future.

## Tradeoff Notes

TBD

## Related Qualities

* [Testability](0009-testability.md) improves reliability
* [Robustness](0006-robustness.md) improves reliability

## Notes

Further Reading:

* [Wikipedia: Reliability Engineering](https://en.wikipedia.org/wiki/Reliability_engineering)

Reliability is closely related to availability, which is typically described as the ability of a component or system to function at a specified moment or interval of time.
