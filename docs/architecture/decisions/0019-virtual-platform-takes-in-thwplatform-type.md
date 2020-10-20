# 19. Virtual Platform Takes in THWPlatform Type

Date: 2020-10-20

## Status

Accepted

Caused by [18. Driver Registration in HW Platform](0018-driver-registration-in-hw-platform.md)

## Context

As a consequence of [ADR 0018](0018-driver-registration-in-hw-platform.md), we moved the [Driver Registry](../components/core/driver_registry.md) definition to the hardware platform and removed the global singleton from the platform. We also want the platform APIs to forward to the hw platform. However, we needed a way to access the hw platform object for successful forwarding. This requires the platform base class to know about the type.

## Decision

The hardware platform type is now a template parameter for the Virtual Platform base class. A local variable will be declared (`hw_platform_`), and a `protected` API will be provided to access that variable as well.

## Consequences

- Users do not have control over how the hardware platform variable is named
- Users may not know that they need to access the hardware platform through specific variables/functions: now there is an education challenge
    + We can show example code to mitigate this
- Only one hardware platform can be used with a platform, which would *potentially* break board ID and board revision selection for instantiating one of many hardware platforms. 
    + However, this can be handled in the hardware platform logic itself, if multiple revisions need to be supported.

## Related Documents

Please see the [associated development log](../../development/logs/20201020_driver_registry_redesign.md) for detailed information about the changes that were made as part of this effort.
