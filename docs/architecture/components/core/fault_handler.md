# CRC-R: Fault Handler

Captures low-level aborts & faults and provides debug information to developers.

**Status:** Not started

**Table of Contents:**

1. [Responsibilities](#responsibilities)
2. [Requirements](#requirements)
3. [Collaborators](#collaborators)
4. [Rationale](#rationale)
5. [Source Links](#source-links)
6. [Related Documents](#related-documents)
7. [Notes](#notes)

## Responsibilities

* Provide a user-friendly message explaining the fault type, and if known, the cause
* Provide a backtrace of function calls that lead to the fault
* Provide a printout of processor registers to give the user a snapshot of the system

## Requirements

* Works for faults, panics, and assertions
* Should be in a format that can be symbolicated easily using a tool

## Collaborators

* Fault handling is likely tied to the [Virtual Processor](virtual_processor.md)

## Rationale

Fault handling is a common occurrence when working with low-level software. Developers often re-write fault handling capabilities on each platform, as SDKs rarely ship with sufficient fault handling support.

In order to aid in debugging, developers should be armed with the necessary information to figure out what happened.

## Source Links

TBD

## Related Documents

* [Developer User Stories](../../use_cases/0001-user_stories_developer.md)
* [Quality: Serviceability](.../../qualities/0012-serviceability.md)

## Notes

* What layer does this belong in?
	* What pieces are processor-dependent, and what pieces are generic?
	* Can we isolate the processor-dependent pieces through a generic API that is required in the base processor class?
* Is this a utility?
* If using logging, should we dump the buffer?
