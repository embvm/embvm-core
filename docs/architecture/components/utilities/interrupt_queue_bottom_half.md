# CRC-R: Interrupt Queue Bottom Half

Receives interrupts from peripheral hardware, handles them, and forwards work to the [Interrupt Queue Top Half](interrupt_queue_top_half.md)

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

* Receives interrupts and adds the handling to a queue based on priority

## Requirements

* Does not use any form of locking (mutex)
* Usable from interrupt context
* Fixed size to avoid allocations in IRQ context

## Collaborators

* To prevent the IRQ queue from blocking for long periods, work is forwarded to the high-priority [Interrupt Top Half Queue](interrupt_queue_top_half.md)
* A [Virtual Processor](../core/virtual_processor.md) configures the Interrupt Queue

## Rationale

Interrupts are prioritized and only one can be active at a time. When an interrupt fires, the handler is added to the interrupt queue, which utilizes a `priority_queue` to handle the highest-priority interrupt.

After processing the interrupt, any additional work is passed to the [Interrupt Top Half Queue](interrupt_queue_top_half.md) to prevent excessive time spent in interrupt context.

## Source Links

* Interrupt Queue Bottom Half
	* [interrupt_queue_tests.cpp](../../../../src/utilities/dispatch/interrupt_queue_tests.cpp)
	* [interrupt_queue.hpp](../../../../src/utilities/dispatch/interrupt_queue.hpp)

## Related Documents

* A [Dispatch Queue](dispatch_queue.md) represents a similar concept

## Notes

N/A
