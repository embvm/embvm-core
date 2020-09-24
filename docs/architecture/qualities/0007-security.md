# Quality Requirement: Security

* Name: Security
* Reference Number: 0007
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

Security is the protection of the system from theft, leaking of data, and disruption/misdirection of services.

## Scenario

* When a developer wants to check the open source projects used in the framework, they can review the software inventory to determine whether their project is at risk for a given security issue

## Test Cases

* When compiled with the following settings, zero warnings will be produced:
	* -Weverything for Clang
	* /W4 for MSVC
	* -Wall -Wextra -pedantic
* When static analysis is run on framework components, no errors should be reported
* When SaferC++ types are enabled, no errors will be reported

CI Server test cases:

* Unauthorized users should not be able to login to the CI server
* Unauthorized users should not be able to view CI server status
* Authorized users should only see a project if they have the correct permissions
* Only administrators can modify CI server settings

### Landing Zones

N/A

## Context

IoT devices are frequently riddled with security holes, buffer overflows, and lack of encryption. The framework should provide developers with the tools needed to create a secure program design.

## Tradeoff Notes

N/A

## Related Qualities

Security is related to [Resilience](0003-resilience.md) and [Robustness](0006-robustness.md). The framework can operate under a wider variety of conditions if it handles security issues reliably.

## Notes

XN (Execute Never) marks certain areas of memory as non-executable and was introduced in ARMv6. This is the least you can do against Buffer Overflows. It shouldn’t be the only enabled mitigation, but heck, that’s the least you should do.
