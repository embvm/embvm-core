# CRC-R: Event Queue

Manage a queue of events.

**Status:** Completed

**Table of Contents:**

1. [Responsibilities](#responsibilities)
2. [Requirements](#requirements)
3. [Collaborators](#collaborators)
4. [Rationale](#rationale)
5. [Source Links](#source-links)
6. [Related Documents](#related-documents)
7. [Notes](#notes)

## Responsibilities

* Keep track of a queue of events
* Provide an option for handling an event queue with its own thread of control (Active Event Queue)

## Requirements

* Push events onto queue
* Pop events from queue, returning the event
* Provide a `front()` function which allows users to peek at the first event

## Collaborators

* Manages [events](event.md)
* `embvm::ActiveEventQueue`s utilize the [`ActiveObject`](../utilities/active_object.md) construct

## Rationale

The framework should provide a common structure and interface that can be used to manage a queue of events. The Event Queue should supporting both static and dynamic memory allocation for maximum flexibility.. This enables code to use the same interface without knowing specific implementation details (e.g., underlying memory allocation strategy).

An active event queue is common for embedded systems. If an object is event-based and has its own thread of control it can inherit from the `embvm::ActiveEventQueue`. The derived class only needs to implement a single function to set up event processing.

## Source Links

* [event_queue.hpp](../../../../src/core/platform/event_queue.hpp)
* [Unit Tests](../../../../src/core/platform/event_tests.cpp)

## Related Documents

N/A

## Notes

N/A
