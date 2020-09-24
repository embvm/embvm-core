# CRC-R: Client Application

**Table of Contents:**

1. [Responsibilities](#responsibilities)
2. [Requirements](#requirements)
3. [Collaborators](#collaborators)
4. [Rationale](#rationale)
5. [Source Links](#source-links)
6. [Related Documents](#related-documents)
7. [Notes](#notes)

## Responsibilities

* Implement application logic in a platform-independent manner

## Requirements

N/A

## Collaborators

* Interacts with underlying hardware & software capabilities using [Virtual Platform](core/virtual_platform.md) interfaces
* Can utilize [Utilities](utilities/) and [Subsystems](subsystems/)
* Can utilize [Virtual RTOS](core/virtual_rtos.md) constructs
* Can utilize C/C++ language features
* Can work directly with generic [Driver](core/driver.md) interfaces via the [Driver Registry](core/driver_registry.md) (provided by the Virtual Platform)

## Rationale

* Client application represents the higher-level Software Layer
* Application code should be reusable across platforms that provide equivalent interfaces & requirements

## Source Links

Each client project will provide its own application code.

Framework demo applications live in [src/applications/](../../../src/applications).

## Related Documents

N/A

## Notes

Application code should prefer interactions with the standard modules over direct driver actions where possible. For example:

* Use the [System Power Manager](subsystems/system_power_manager.md) to control driver power state, rather than talking directly to drivers
* Prefer descriptive APIs in the [Virtual Platform](core/virtual_platform.md) rather than implementing logic in the application layer, especially if the behavior can change when the underlying platform changes
