# CRC-R: Active Object

Represents an object with its own thread of control.

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

* Provide an object with its own thread of control
* Provide an object with an event/operation queue (allowing operations to be enqueued)
* Provide derived classes with a function to define how operations will be processed

## Requirements

* Implemented as a base class for use via inheritance
* Should be orthogonal in definition so multiple inheritence can be used
* Provides an interface for external actors to notify the object of events to process
* Utilizes static memory allocation

## Collaborators

* Active Objects can process to [Events](event.md)
* Active Objects utilize a [static queue](static_queue.md) as the event queue

## Rationale

One of the primary goals of the framework is to reduce the number of manually created and managed threads. Active objects help achieve this goal. Rather than encouraging users to use threads directly, we are trying to provide abstracted concepts which reduce some of the threading complexities.

Active Objects represent a reactive object which has its own thread of control. An Active Object manages its own thread of control, a queue for events, and functions to handle incoming events. These components are reusable and common across usage scenarios. Derived classes which use the Active Object base benefit from reuse and ease of implementation.

## Source Links

* [active_object.hpp](../../../../src/utilities/active_object/active_object.hpp)

## Related Documents

* Active Objects could be considered as a localized version of the [Event Center](../core/event_center.md)
* Active Objects resemble single-threaded [dispatch queues](dispatch_queue.md).

## Notes

N/A
