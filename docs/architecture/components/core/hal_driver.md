# CRC-R: HAL Driver

Extension of the [Driver](driver.md) concept which provides common functionality for processor peripheral drivers.

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

* Provide a generalized interaction model for processor peripheral drivers
* Provide extended initialization and customization options for HAL drivers

## Requirements

* Provide method for enabling/disabling interrupts

## Collaborators

* A [Virtual Processor](virtual_processor.md) uses HAL drivers
* HAL Drivers can be registered with the [Driver Registry](driver_registry.md)
* HAL Drivers representing timers/clocks can be registered with the [Timer Manager](timer_manager.md)
* A HAL Driver probably uses [Registers](../core/register.md)

## Rationale

HAL drivers, which represent low-level drivers such as processor peripherals, have a different set of functionality that may not be required by higher-level drivers.

Examples include interrupts: peripheral drivers need to enable/disable interrupts, as well as to invoke callbacks from an interrupt context. These interrupt callbacks are handled through an IRQ queue.

## Source Links

* [hal_driver.hpp](../../../../src/core/driver/hal_driver.hpp) - base HAL interface

## Related Documents

* [Driver](driver.md)
* [DriverRegistry](driver_registry.md)
* [Timer Manager](timer_manager.md)

## Notes

N/A


