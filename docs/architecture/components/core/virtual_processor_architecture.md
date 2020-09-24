# CRC-R: Virtual Processor Architecture

The Virtual Processor Architecture encapsulates the common set of functionality provided by a given processor architecture.

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

* Defines functionality specific to an architecture family

## Requirements

* Provides a virtual Memory Management Unit (MMU) interface with default passthrough implementations on architectures without an MMU
* Provide a virtual caching interface with default passthrough implementations on architectures without a cache

## Collaborators

* A [Virtual Processor](virtual_processor.md) has a Virtual Architecture

## Rationale

Processors have to primary sources of functionality:

1. Processor architecture
2. Vendor-specific Functionality & Peripherals

An architecture can be shared by multiple processors (e.g., there are tons of Cortex-M devices). Common architecture functionality should be abstracted out of the processor implementation and made available for reuse.

Processors include an architecture implementation in their inheritance tree to gain access to its functionality.

## Source Links

* [virtual_processor_arch.hpp](../../../../src/core/processor/virtual_processor_arch.hpp)

## Notes

Should MMU and Caching be separate interfaces, rather than contained within the processor architecture abstraction?
