# 21. Eliminate name variables from Core base classes

Date: 2020-10-22

## Status

Accepted

## Context

Under the initial design of the `embvm-core`, the following types stored a name in the base class:

- Driver
- Processor
- Hardware Platform
- Platform

In some cases, like drivers, this data was duplicated (through the driver registry) or unnecessary (because you use the default name and provide the system name in the driver registry). We don't need to store two different instances of the same name!

In other cases, like the processor and platforms, these names were never used.

## Decision

These names will no longer be required by the base classes.

## Consequences

- Reducing storage overhead per-class
- Users can still specify names for their objects, but they must do it on their own
- Canonical names for public drivers are handled through the Driver Registry (as the key)
