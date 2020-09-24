# CRC-R: Timer Manager

The Timer Manager provides a generic interface for platforms and applications to create and configure timers.

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

* Creation of new software timers
* Re-configuration of existing software timers
* Deletion of existing software timers
* Access to and reuse of existing software timers

## Requirements

* Provide one-shot software timers
* Provide periodic software timers

## Collaborators

* The Timer Manager is contained within the [Virtual Hardware Platform](virtual_hardware_platform.md)

## Rationale

The Platform and Software Layers must have a mechanism to work with timers in a generic way. Each processor implements timers in different ways, so a generic interface must be used to prevent upper layers from depending on the processor timer implementation.

A single hardware timer is managed by the `embvm::TimerManager`. A centralized timer manager structure allows existing timers to be re-used for other purposes. For instance, there is no need to have two separate 1 second period timers, one timer could trigger two actions when it expires. Additionally, a 500 millisecond timer and 1 second timer can share the same timer hardware.

## Source Links

* [timer_manager.hpp](../../../../src/core/hw_platform/timer_manager.hpp)
* [Unit Tests](../../../../src/core/hw_platform/timer_manager_tests.cpp)

## Notes

N/A

