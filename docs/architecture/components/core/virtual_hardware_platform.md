# CRC-R: Virtual Hardware Platform

Basic hardware platform definition which provides common required interfaces that apply to embedded hardware platforms.

**Status:** Completed, board rev support still needed

**Table of Contents:**

1. [Responsibilities](#responsibilities)
2. [Requirements](#requirements)
3. [Collaborators](#collaborators)
4. [Rationale](#rationale)
5. [Source Links](#source-links)
6. [Related Documents](#related-documents)
7. [Notes](#notes)

## Responsibilities

* Provide abstracted interfaces for working with the underlying system hardware
* Defines and instantiates device drivers provided by the hardware platform
* Initialize and configure the processor as to work as required by the hardware platform

## Requirements

* Needs to provide a mechanisms for supporting multiple revisions of a single hardware platform

## Collaborators

* Contains a [Driver Registry](driver_registry.md), which is used to register drivers for access in higher program layers.
* Contains a [Virtual Processor](virtual_processor.md)
* Contains any number of [Drivers](driver.md) which communicate with circuitry on the hardware platform
* Is a component of a [Virtual Platform](virtual_platform.md)
* May contain a [Timer Manager](timer_manager.hpp) to provide the Virtual Platform with software timers

## Rationale

Hardware platforms are always extremely different and a source of tight coupling in embedded systems. We want to isolate tightly-coupled hardware-related code in a single location. This promotes cleaner design and an easier time finding things.

The Virtual Hardware Platform utilizes the [Mediator Pattern](../../../patterns/mediator.md) to encapsulate processor and device driver interactions. The Virtual Hardware Platform coordinates the configuration of the underlying processor and peripherals. The Virtual Hardware Platform contains device driver initialization code. It also provides the Virtual Platform (and upper-level software) with any necessary board-level interfaces.

The Virtual Hardware Platform is the location to decide on whether chip peripherals will be used with DMA on/off, interrupts on/off, etc.

Start order is controlled by the implementation of the `init()` function. Drivers are configured & started in the correct order dictated by the platform.

## Source Links

* [virtual_hw_platform.hpp](../../../../src/core/hw_platform/virtual_hw_platform.hpp)
* [virtual_hw_platform.cpp](../../../../src/core/hw_platform/virtual_hw_platform.cpp)
* [Unit Tests](../../../../src/core/hw_platform/virtual_hw_platform_tests.cpp)
* [Unit Test HW Platform](../../../../src/hw_platforms/unit_test/)
* [Simulator HW Platform Base](../../../../src/hw_platforms/simulator/)

## Related Documents

* [ADR 0019: Virtual Platform Takes in THWPlatform Type](../../decisions/0019-virtual-platform-takes-in-thwplatform-type.md)
* [Mediator Pattern](../../../patterns/mediator.md)

## Notes

For compile-time configuration purposes, a `hw_platform_options.hpp` file is defined in the header

### Supporting Multiple Board Revisions

* How can we support multiple board revisions?
    - One consequence of [ADR 0019](../../decisions/0019-virtual-platform-takes-in-thwplatform-type.md): Only one hardware platform can be used with a platform, which would *potentially* break board ID and board revision selection for instantiating one of many hardware platforms. 
    + However, this can be handled in the hardware platform logic itself, if multiple revisions need to be supported.
* Configure: run-time vs static differentiation for board rev / ID
* If board revision IDs don't exist in hardware, just use a software reported number
* Kvasir - check into pin location, `register::bitlocation`
	* Sadly it is common practice to litter code with `ifdef BOARD_REVA` etc. `Kvasir::GPIO::PinLocation` allows you to encode you port an pin information into a compile time accessible container. You can then pass this container a compile time factory function which will return you the corresponding register action.
