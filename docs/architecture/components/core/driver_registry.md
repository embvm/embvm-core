# CRC-R: Driver Registry

Provides access to virtual driver interfaces for higher-level software, helping break implementation dependencies on the underlying hardware.

**Status:** Completed, improvements possible

**Table of Contents:**

1. [Responsibilities](#responsibilities)
2. [Requirements](#requirements)
3. [Collaborators](#collaborators)
4. [Rationale](#rationale)
5. [Source Links](#source-links)
6. [Related Documents](#related-documents)
7. [Notes](#notes)

## Responsibilities

* Provide a single location for accessing drivers supported by the platform
* Constrain the Software Layer to using generic driver interfaces, rather than specific driver implementations

## Requirements

* Provide interfaces to access a specific driver instance
* Provide interfaces to access a list of drivers by type

## Collaborators

* [Drivers](driver.md) (including [HAL Drivers](hal_driver.md)) automatically register themselves with the Driver Registry
* The Driver Registry is part of the [Virtual Platform](virtual_platform.md)

## Rationale

A common source of coupling in firmware applications is direct use of driver instances in the application software layer. In order to prevent coupling to specific instances, developers should be constrained to generic driver interfaces.

The Driver Registry provides a single point of access when developers need to work directly with a driver. Drivers are automatically registered during construction, and developers can query the system for a specific interface or for a list of drivers by type.

Driver registration happens automatically as part of the driver base class. By having the base class handle registration, we keep the Driver Registry logically decoupled from the processor layer, which works with HAL drivers. Drivers are registered without any action required by the processor.

## Source Links

* [driver_registry.hpp](../../../../src/core/driver/driver_registry.hpp)
* [driver_test.cpp](../../../../src/core/driver/driver_test.cpp)

## Related Documents

* [Use Case: Access a Specific Driver Instance](../../use_cases/0008-access_a_specific_driver_instance.md)
* [Use Case: Find All Drivers of a Specific Type](../../use_cases/0009-find_all-drivers_of_a_specific_type.md)

## Notes

Is there a way we can handle driver lookups at compile time with template meta-programming or preprocessing?

* If we know the list of drivers that is registered, we could easily supply the correct pointer & object at compile-time instead of run-time
	* Would be nice if your build failed because `accel0` isn't found
* Odin has a [filter function](https://github.com/kvasir-io/mpl/blob/development/src/kvasir/mpl/algorithm/filter.hpp) in the [Kvasir Meta-programming Library (MPL)](https://github.com/kvasir-io/mpl/)
	* Used in the Agent-based Class Design (ABCD) repository
* Reading list:
	* https://codereview.stackexchange.com/questions/102495/compile-time-list-in-c
	* https://blog.mattbierner.com/stupid-template-tricks-lazy-compile-time-lists/
