# CRC-R: Driver

Basic driver definition which provides common required interfaces that apply to embedded device drivers.

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

* Provide base-class interfaces for device drivers
* Provide generic interfaces for a category/type of driver, decoupling users from specific driver implementations
* Register drivers with the [Driver Registry](driver_registry.md)

## Requirements

* Allow users to extend the driver model with additional driver categories and primitives
* No blocking calls are allowed - support event driven programming
* Utilize callbacks or publish/subscribe to aid in decoupling

## Collaborators

* A [Virtual Hardware Platform](virtual_hardware_platform.md)
* Drivers are automatically registered with the [Driver Registry](driver_registry.md)
* Driver objects representing timers/clocks are registered with the [Timer Manager](timer_manager.md) as well as the Driver Registry
* A driver may utilize [Registers](../core/register.md)

## Rationale

A common source of coupling in firmware applications is direct use of driver instances in the application software layer. In order to prevent coupling to specific instances, generic interfaces should be defined and used instead of specific implementations.

Since we want our framework to utilize an event-driven model, drivers should not have blocking interfaces.

## Source Links

* [Driver folder](../../../../src/core/driver/)
    * [driver_base.cpp](../../../../src/core/driver/driver_base.cpp) - base driver interface
    * [driver.hpp](../../../../src/core/driver/driver.hpp) - base driver interface
    * [driver_type.hpp](../../../../src/core/driver/driver_type.hpp) - defines default driver types
    * [driver_test.cpp](../../../../src/core/driver/driver_test.cpp)
    * Base interfaces:
        * [communication_bus.hpp](../../../../src/core/driver/communication_bus.hpp) - provides base interface for comunication bus drivers (SPI, I2C, etc.)
        * [gpio.hpp](../../../../src/core/driver/gpio.hpp)
        * [i2c.hpp](../../../../src/core/driver/i2c.hpp)
        * [led.hpp](../../../../src/core/driver/led.hpp)
        * [spi.hpp](../../../../src/core/driver/spi.hpp)
        * [system_clock.hpp](../../../../src/core/driver/system_clock.hpp)
        * [time_of_flight.hpp](../../../../src/core/driver/time_of_flight.hpp)
        * [timer.hpp](../../../../src/core/driver/timer.hpp)

## Related Documents

* [HAL Driver](hal_driver.md)

## Notes

* How do we handle drivers which provide periodic automatic measurements?
    * 1) device interrupts when data is ready, interrupt dispatches function to handle read & updates consumers who registered callbacks
    * 2) we need to query the device
        * use a timer, when expired take action - dispatch function to read/update
* Need guidelines for higher order drivers - guidelines for developers to help build them using primitives
* Use constructors to constrain the input bus and check for correctness at compile-tmie
    * BMA282: spi only, another might be I2C only, etc.
    * But what if I have a board which does USB interface for BMA282?
    * The constructor specifies the underlying drivers it will accept
* How can complex composite drivers register their distinct interfaces with the registry?
    * Consider the BMR456 - we want to provide temp1, temp2, voltage control…
    * Does this mean our primitive interfaces need to be separate from the Driver interface? Two trees?
    * Should we create the different interfaces as separate objects to they get registered separately?
    * Is teh composite just a façade to the subsystem of driver primitives? That would be interesting. Would need to use friend classes or some other mechanism to encapsulate behavior properly. Or maybe the complex interface just passes in its this pointer to the primitives.
* Should driver start/stop generate events?
    * power state changes - someone may want ot know when the driver is back so other events can be kicked off automatically (e.g.,, data consumer thread)
    * Someone may want to know that their lower-level hardware is unavailable
