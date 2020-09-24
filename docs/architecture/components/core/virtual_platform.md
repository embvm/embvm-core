# CRC-R: Virtual Platform

The Virtual Platform serves as the abstraction between the Software Layer and the underlying hardware platform and operating system. The Virtual Platform defines generic interfaces that the Software Layer requires, ensuring that software remains decoupled from hardware specifics.

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

* Abstracting functionality and capabilities required by the Software Layer and provided by the underlying hardware and operating system
* Providing generic mechanisms for accessing drivers through their abstract interfaces
* Providing generic mechanisms for timers
* Handling event dispatching and subscription
* Sequencing system boot-up

## Requirements

* Provides interfaces which access DriverRegistry functionality, without exposing DriverRegistry
* Provides interfaces which access Dispatch Queue functionality, without exposing the dispatch queue itself
* Provides interfaces which access Event Center functionality, without exposing `embvm::EventCenter`
* Features should be optional and opt-in:
    * Dispatch
    * Event management

## Collaborators

* The [Client Application](../client_application.md) can call Virtual Platform interfaces
* The Virtual Platform contains:
	* A [Virtual Hardware Platform](virtual_hardware_platform.md)
	* If used, the [Virtual RTOS](virtual_rtos.md) is part of the Virtual Platform
	* The [Driver Registry](driver_registry.md)
	* The [Timer Manager](timer_manager.md)
	* The [Boot Sequencer](boot_sequener.md)
    * The [Event Center](event_center.md)

## Rationale

The Virtual Platform serves as a [Mediator](../../../patterns/mediator.md) that encapsulates the underlying hardware/OS platform. All platform-specific interactions should be contained within the Virtual Platform and its child components. The Software Layer communicates with the underlying hardware and OS through the generic Virtual Platform interfaces.

A single Virtual Platform Interface can be support multiple implementations, requiring use of the [Bridge Pattern](../../../patterns/bridge.md). This enables separation between the platform abstraction and the platform implementation. Additionally, the virtual platform serves as a Bridge by isolating the client software from direct knowledge of supporting structures such as the [Driver Registry](driver_registry.md)

The Virtual RTOS is contained within the Virtual Platform layer because support is optional. If the Software Layer depends on RTOS concepts, then it requires underlying platforms which provide that support.

## Source Links

* [virtual_platform.hpp](../../../../src/core/platform/virtual_platform.hpp)
* [virtual_platform_dispatch.hpp](../../../../src/core/platform/virtual_platformdispatch.hpp)
* [virtual_platform_event.hpp](../../../../src/core/platform/virtual_platform_event.hpp)
* [Unit Tests](../../../../src/core/platform/virtual_platform_tests.cpp)

## Related Documents

* [Mediator Pattern](../../../patterns/mediator.md)
* [Bridge Pattern](../../../patterns/bridge.md)
* [Simulator Hardware Platform](simulator_hardware_platform.md)

## Notes

N/A
