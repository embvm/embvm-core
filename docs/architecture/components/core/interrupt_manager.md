# CRC-R: Interrupt Manager

Central location to manage interrupts & interrupt handlers.

**Status:** Completed

## Responsibilities

* Enable and disable interrupts globally
* Configure interrupt handlers
* Configure interrupts and priorities

## Requirements

Interrupt manager handles registering interrupt handler functions, enabling/disabling interrupts, and managing interrupt priorities.

## Collaborators

* Works with the [Interrupt Queue](../utilities/interrupt_queue.md)
* Part of the [Virtual Processor](virtual_processor.md)

## Rationale

The processor has one global interrupt table and one global interrupt state. Additionally, interrupts can be configured individually and given differing priorities. There should be one location in the system which manages the processor's interrupts.

The interrupt manager handles interrupts directly. Interrupts are forwarded to registered handlers. Any function should be able to serve as a top-half of the interrupt handler (lambda, C-linkage function, class member).

## Source Links

* [Virtual interrupt manager](../../../../src/core/processor/interrupt_manager.hpp)

## Notes

N/A
