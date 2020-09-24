# CRC-R: Virtual Processor

The Virtual Processor provides an abstract model which can decouple a hardware platform from the underlying processor.

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

* Defines common & generic processor interfaces

## Requirements

* Provide a method for configuring the underlying clocks and peripherals for a given platform
* Allows mechanisms to access the specific underlying processor for advanced controls at the cost of tighter coupling

## Collaborators

* A Processor is implemented using a [Virtual Processor Architecture](virtual_processor_architecture.md)
* A [Virtual Hardware Platform](virtual_hardware_platform.md) contains a processor
* A Virtual Processor contains [HAL Drivers](hal_driver.md) representing the peripherals
* A Virtual Processor has an [Interrupt Queue](../utilties/interrupt_queue.md) if interrupts are supported
* A Virtual Processor contains [Registers](../core/register.md)

## Rationale

The Virtual Processor attempts to provide an abstract layer for interfacing with an underlying processor. In general scenarios, this will allows platforms to be decoupled. Processor families can provide additional functionality through sub-classing.

Users who require specialized configuration of the processor can ignore the virtual abstractions at the cost of tighter coupling.

Each processor implementation has a header which implements the virtual functions, provides optional extensions, declares constants, and defines/declares the peripherals that it makes available.

Note that processors are isolated from the greater system due to their containment within the [Virtual HW Platform](virtual_hardware_platform.md). This means that any number of processor-specific interfaces may be created without impacting the greater system. Any changes that are needed when migrating from one processor to another are contained within the Virtual HW Platform.

## Source Links

* [virtual_processor.hpp](../../../../src/core/processor/virtual_processor.hpp)

## Related Documents

* [Simulator Processor](simulator_processor)

## Notes

N/A
