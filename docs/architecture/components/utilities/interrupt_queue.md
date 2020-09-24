# CRC-R: Interrupt Queue

A specialized dispatch queue which can queue operations from interrupt context

This component decomposes into two sub-components:

* [Interrupt Queue Top Half](interrupt_queue_top_half.md)
* [Interrupt Queue Bottom Half](interrupt_queue_bottom_half.md)

**Status:** WIP

**Table of Contents:**

1. [Responsibilities](#responsibilities)
2. [Requirements](#requirements)
3. [Collaborators](#collaborators)
4. [Rationale](#rationale)
5. [Source Links](#source-links)
6. [Related Documents](#related-documents)
7. [Notes](#notes)

## Responsibilities

* Receives interrupts and adds the handling to a queue based on priority ([Interrupt Queue Bottom Half](interrupt_queue_bottom_half.md))
* Dispatches handlers to a secondary high-priority queue in normal operating context ([Interrupt Queue Top Half](interrupt_queue_top_half.md))

## Requirements

* Does not use any form of locking (mutex)
* Usable from interrupt context
* Fixed size to avoid allocations in IRQ context

## Collaborators

* To prevent the [IRQ bottom-half queue](interrupt_queue_bottom_half.md) from blocking for long periods, work is forwarded to high-priority [Top Half queue](interrupt_queue_top_half.md)

## Rationale

Interrupts are prioritized and only one can be active at a time. When an interrupt fires, the handler is added to the interrupt queue, which utilizes a `priority_queue` to handle the highest-priority interrupt.

After processing the interrupt, any additional work is passed to a high-priority dispatch queue to prevent excessive time spent in interrupt context.

## Source Links

* [dispatch Folder](../../../../src/utilities/dispatch/)
* Interrupt Queue Bottom Half
	* [interrupt_queue_tests.cpp](../../../../src/utilities/dispatch/interrupt_queue_test.cpp)
	* [interrupt_queue.hpp](../../../../src/utilities/dispatch/interrupt_queue.hpp)

## Related Documents

* A [Dispatch Queue](dispatch_queue.md) represents a similar concept

This component decomposes into two sub-components:

* [Interrupt Queue Top Half](interrupt_queue_top_half.md)
* [Interrupt Queue Bottom Half](interrupt_queue_bottom_half.md)

## Notes

N/A
