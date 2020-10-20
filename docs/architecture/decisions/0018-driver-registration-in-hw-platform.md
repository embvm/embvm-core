# 18. Driver Registration in HW Platform

Date: 2020-10-20

## Status

Accepted

Caused [19. Virtual Platform Takes in THWPlatform Type](0019-virtual-platform-takes-in-thwplatform-type.md)

Caused [20. Hardware Platform Options File](0020-hardware-platform-options-file.md)

## Context

Previously, the [Driver base class](../components/core/driver.md) interacted with the [Driver Registry](../components/core/driver_registry.md). This eliminated work that was needed from the user's side, but it also complicated the base class and introduced a dependency of drivers on the registry.

## Decision

The Driver Registry dependency will be removed from the Driver base class. Drivers will instead be manually registered in the hardware platform.

This comes with a few related changes:

- There is no more global driver registry instance - it's part of the hardware platform, and the virtual platform interfaces just forward the call to the hardware platform
- The virtual hardware platform base class takes a driver registry as a template parameter, which allows us to defer the selection of our driver registry until we declare one in the platform. 
    - From the hardware-platform's perspective, we don't care if we are using static or dynamic registry creation. We can't have our hardware platform deciding that it needs dynamic memory, when our platform is set up to use no dynamic memory.
- The virtual platform base class will be adjusted to forward the calls to the hardware platform APIs
- There is no more driver registry global singleton (in the virtual platform), because we don't need to access the registry *before* the hardware platform is created. Now, the instance is kept in the virtual hardware platform.

## Consequences

- Users must now manually handle registration when a driver is created
- Users must now manually handle de-registration when a driver is destroyed
    + This is mostly a problem for unit test platforms and dynamic discovery.
- Users can control what drivers get added to the registry
- Drivers can be registered multiple times with different interfaces.
- We needed to create a [hardware platform options file](0020-hardware-platform-options-file.md) to enable users to configure the desired Driver Registry type at compile-time. This introduces build complexity, and users need to know how to do things the right way. It would be better to find a way to hide this from the user.

## Related Documents

Please see the [associated development log](../../development/logs/20201020_driver_registry_redesign.md) for detailed information about the changes that were made as part of this effort.
