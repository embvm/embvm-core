# Quality Requirement: Serviceability

* Name: Serviceability
* Reference Number: 0012
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

Serviceability refers to the ability of personnel to:

* Monitor the state of a framework system
* Identify exceptions or faults
* Debug or isolate issues
* Provide software or hardware maintenance
* Upgrade the software

Our framework should enable developers and testers to access debug information and error reports to facilitate root cause analysis.

## Scenario

* When an assert is triggered, the developer receives a printout with:
	* Assert line
	* Assert message
	* Backtrace
* When a panic is triggered, the developer receives a printout with:
	* Backtrace
	* Processor state
* When a fault is triggered, the developer receives a printout with:
	* Backtrace
	* Processor state
* When a developer requests a log buffer dump, the contents of the buffer are displayed
* CI Server: When a build error occurs, a message is sent to developers. Could be:
	* Email
	* Text
	* Slack
	* Issue filing on GitHub
* CI Server: When a test failure occurs, a message is sent to developers. Could be:
	* Email
	* Text
	* Slack
	* Issue filing on GitHub

## Test Cases

The scenarios above describe the expected result of the various Serviceability scenarios.

### Landing Zones

N/A

## Context

We must enable users of the framework to access all necessary debugging information that is required by their team.

By keeping the framework serviceable, we:

* Reduce Embedded Artistry's service burdens
* Enable developers to debug issues quickly
* Enable ourselves to diagnose an fix issues quickly

## Tradeoff Notes

Serviceability mechanisms increase debuggability, but also require additional binary storage and RAM. Embedded systems with limited memory (in either form) may not be able to afford all Serviceability mechanisms.

## Related Qualities

* Serviceability improves the [Maintainability](0013-maintainability.md) of the framework
* Serviceability improves developer [Productivity](0001-productivity.md) when debugging issues
* [Documentation](0010-documentation.md) aids Serviceability

## Notes

Further Reading:

* [Wikipedia: Serviceability](https://en.wikipedia.org/wiki/Serviceability_(computer))

Future ideas for Serviceability improvements:

* Graceful degradation of service when an exceptional hardware error occurs
* Event logging / tracing
* Procedure entry/exit logging with incoming and return values
