# Quality Requirement: Template

* Name: (Replace template above)
* Reference Number: Optional
* Parent: N/A
* Children: N/A
* Category: Execution Qualities

**Table of Contents:**

1. [Description](#description)
2. [Scenario](#scenario)
3. [Test Cases](#test-cases)
4. [Landing Zones](#landing-zones)
5. [Context](#context)
6. [Tradeoff Notes](#tradeoff-notes)
7. [Related Qualities](#related-qualities)

## Description

Software is considered robust if it continues to be well-behaved when unusual or unexpected conditions occur. Robustness also encompasses a system's ability to deal with erroneous input.

## Scenario

* When a user encounters an error, they should receive an unambiguous error message rather than an error code
	* The error message should try to be as accurate as possible without being misleading to the user, so that the problem can be fixed with ease
	* If a user encounters an error code, they receive instructions on how to look up the code
* When an impossible input scenario is detected, the framework asserts/panics
* When invalid input is received from an external system, the framework rejects the input and continues operation
* When compiled in release mode, if a fault/panic is encounterd, the framework will automatically reboot to resume normal operations

## Test Cases

TBD

### Landing Zones

| Quality | Measurement | Minimum | Target | Oustanding |
|---------|-------------|---------|--------|------------|
| Functions returning error codes | % | 25% | 10% | 1% |
| Error messages that are unambiguous | % | 75% | 85% | 95% |
| Functions using pre/post conditions | % | 50% | 75% | 90% |

## Context

Errors in computing happen, and the framework should be able to handle them gracefully without locking up. The framework should also reject invalid input as a measure to increase [Security](0007-security.md)

## Tradeoff Notes

In general, building robust systems that encompass every point of possible failure is difficult because of the vast quantity of possible inputs and input combinations.

It is possible to decide that a certain input value is unexpected, when in fact it is valid in to other users in a different situation.

## Related Qualities

* Robustness increases [Reliability](0002-reliability.md) and [Resilience](0003-resilience.md)
* Robustness can be checked using [Testing](0009-testability.md)
* Robustness can help increase [Security](0007-security.md), especially in the context of rejecting erroneous input
* Providing understandable errors rather than obstruse error codes improves the [Productivity](0001-productivity.md) of developers and the [Serviceability](0012-serviceability.md) of the framework

## Notes

Further Reading:

* [Wikipedia: Robustness (Computer Science)](https://en.wikipedia.org/wiki/Robustness_(computer_science))

Note on "dangerous implements":

> Dangerous implements - Users should not gain access to libraries, data structures, or pointers to data structures. This information should be hidden from the user so that the user doesn't accidentally modify them and introduce a bug in the code. When such interfaces are correctly built, users use them without finding loopholes to modify the interface. The interface should already be correctly implemented, so the user does not need to make modifications. The user therefore focuses solely on his or her own code.
