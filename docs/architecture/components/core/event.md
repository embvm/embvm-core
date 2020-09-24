# CRC-R: Event

Basic object prototype for a system event.

**Status:** Completed, issues exist for dynamic memory allocations

**Table of Contents:**

1. [Responsibilities](#responsibilities)
2. [Requirements](#requirements)
3. [Collaborators](#collaborators)
4. [Rationale](#rationale)
5. [Source Links](#source-links)
6. [Related Documents](#related-documents)
7. [Notes](#notes)

## Responsibilities

* Represent an event in the system

## Requirements

* Extensible so that users can derive additional types of events

## Collaborators

* Events can be consumed by [Active Objects](active_object.md) and the [Event Center](../core/event_center.md)
* Any part of the system can generate and post an event

## Rationale

A central event representation is needed to represent signals with & without data.

Event categories are unique and created according to a system's purpose. The event type should be extensible so that users can provide their own event definitions.

## Source Links

* [event.hpp](../../../../src/core/platform/event.hpp)
* [Unit Tests](../../../../src/core/platform/event_tests.cpp)

## Related Documents

N/A

## Notes

N/A
