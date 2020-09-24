# CRC-R: Simulator Processor

The Simulator Processor is a special case of the [Virtual Processor](virtual_processor.md).

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

* Simulates a processor to allow developers to test framework software on their host machine
* Defines peripherals which a host developer can work with on their machine (e.g., CAN adapter, SPI adapter)

## Requirements

* Runs on a host machine (e.g., `x86` architecture)
* Enable users to define their own simulator configurations

## Collaborators

* A Simulator Processor must still invoke the [Boot Sequencer](boot_sequencer.md)
* A Simulator Processor may use a [Processor Architecture](virtual_processor_architecture.md), such as `x86` or `x86_64`
* A Simulator Processor is contained in a [Simulator Hardware Platform](simulator_hardware_platform.md)

## Rationale

We want to enable developers to write and test framework software on their host machine. This requires abstractions that work on the host machine. The Simulator Processor provides developers with the ability to create host-machine abstractions for their platform.

## Source Links

* [Simulator Processor](../../../../src/procesors/simulator/)

## Related Documents

* [Virtual Processor](virtual_processor.md)
* [Use Case: Compile in Simulator Mode](../../use_cases/0010-compile_in_simulator_mode.md)

## Notes

N/A
