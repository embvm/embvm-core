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

* [Drivers](driver.md) (including [HAL Drivers](hal_driver.md)) can be registered in the [Virtual Hardware Platform](virtual_hardware_platform.md)
* The Driver Registry interfaces are provided as part of the [Virtual Platform](virtual_platform.md)
    - You may rightly wonder: does the driver registry really belong in the platform? Or the hardware platform?
        - The API for public use belongs in the platform, because application layer can't access the hardware platform directly. The HW platform has the definition of the driver registry, so that code can already access it without any trouble.

## Rationale

A common source of coupling in firmware applications is direct use of driver instances in the application software layer. In order to prevent coupling to specific instances, developers should be constrained to generic driver interfaces.

The Driver Registry provides a single point of access when developers need to work directly with a driver. Drivers are registered by the user in their HW Platform, and developers can query the system for a specific interface or for a list of drivers by type. 

> **Note:** Manual registration allows the user to have fine-grained control over what gets registered and how.

## Source Links

* [driver_registry.hpp](../../../../src/core/driver/driver_registry.hpp)
* [driver_test.cpp](../../../../src/core/driver/driver_test.cpp)

## Related Documents

* [Use Case: Access a Specific Driver Instance](../../use_cases/0008-access_a_specific_driver_instance.md)
* [Use Case: Find All Drivers of a Specific Type](../../use_cases/0009-find_all-drivers_of_a_specific_type.md)
* [ADR 0018: Driver Registration in HW Platform](../../decisions/0018-driver-registration-in-hw-platform.md)

## Notes

Is there a way we can handle driver lookups at compile time with template meta-programming or preprocessing?

* If we know the list of drivers that is registered, we could easily supply the correct pointer & object at compile-time instead of run-time
	* Would be nice if your build failed because `accel0` isn't found
* Odin has a [filter function](https://github.com/kvasir-io/mpl/blob/development/src/kvasir/mpl/algorithm/filter.hpp) in the [Kvasir Meta-programming Library (MPL)](https://github.com/kvasir-io/mpl/)
	* Used in the Agent-based Class Design (ABCD) repository
* Reading list:
	* https://codereview.stackexchange.com/questions/102495/compile-time-list-in-c
	* https://blog.mattbierner.com/stupid-template-tricks-lazy-compile-time-lists/
   
* How can complex composite drivers register their distinct interfaces with the registry?
    * Consider the BMR456 - we want to provide `temp1`, `temp2`, voltage control…
    * Does this mean our primitive interfaces need to be separate from the Driver interface? Two trees?
    * Should we create the different interfaces as separate objects to they get registered separately?
    * Is the composite just a façade to the subsystem of driver primitives? That would be interesting. Would need to use friend classes or some other mechanism to encapsulate behavior properly. Or maybe the complex interface just passes in its this pointer to the primitives. 
