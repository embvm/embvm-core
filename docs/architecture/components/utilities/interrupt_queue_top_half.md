# CRC-R: Interrupt Queue Top Half

Receives work requests from the [Interrupt Queue Bottom Half](interrupt_queue_bottom_half.md)

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

* Handles work requests associated with an interrupt

## Requirements

* Does not use any form of locking (mutex)
* Work can be added from an interrupt context
* Fixed size to avoid allocations in IRQ context

## Collaborators

* Receives work requests from the [Interrupt Queue Bottom Half](interrupt_queue_bottom_half.md)
* A [Virtual Processor](../core/virtual_processor.md) configures the Interrupt Queue

## Rationale

* Prevents the IRQ bottom-half queue from spending too much time in the interrupt context
* Handles work requests in normal operating contexts, where mutexes and other runtime concepts are allowed

## Source Links

TBD

## Related Documents

* A [Dispatch Queue](dispatch_queue.md) represents a similar concept

## Notes

Our current interrupt queue design takes in a top-half handler dispatch function during object creation. This prevents our bottom-half handler from needing to know about any implementation strategies
