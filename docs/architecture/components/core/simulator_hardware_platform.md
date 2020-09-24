# CRC-R: Simulator Hardware Platform

The Simulator Hardware Platform is a special case of the [Virtual Hardware Platform](virtual_hardware_platform.md). A Simulator Hardware Platform enables writing/testing embedded software directly on a host machine.

**Status:** Completed, framework provides two demo simulator hardware platforms

**Table of Contents:**

1. [Responsibilities](#responsibilities)
2. [Requirements](#requirements)
3. [Collaborators](#collaborators)
4. [Rationale](#rationale)
5. [Source Links](#source-links)
6. [Related Documents](#related-documents)
7. [Notes](#notes)

## Responsibilities

* Simulates a hardware platform which includes a [Simulator Processor](simulator_processor.md)
* Defines peripherals which a host developer can work with on their machine (e.g., CAN adapter, SPI adapter)
* Is usable with the application layer to enable host-machine testing

## Requirements

* Works on a host machine
* Can utilize all of the same constructs as regular hardware platforms

## Collaborators

* A Simulator Hardware Platform contains a [Simulator Processor](simulator_processor.md)

## Rationale

We want to enable developers to write and test framework software on their host machine. This requires abstractions that work on the host machine. The Simulator Hardware Platform provides developers with the ability to create host-machine abstractions for their target platform.

The Simulator Hardware Platform can instantiate the same device drivers which would run on the target hardware. By using the [Simulator Processor's](simulator_processor.md) HAL with a USB-to-X adapter, drivers can be written and/or tested directly on the host machine. Higher-level modules which depend on this hardware can also be written and/or tested by talking directly to hardware.

## Source Links

* [Blinky Simulator HW Platform](../../../../src/hw_platforms/blinky_simulator/)
* [Framework Demo Simulator HW Platform](../../../../src/hw_platforms/fwdemo_simulator/)

## Related Documents

* [Virtual Hardware Platform](virtual_hardware_platform.md)
* [Use Case: Compile in Simulator Mode](../../use_cases/0010-compile_in_simulator_mode.md)

## Notes

N/A
