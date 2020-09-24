# Constraint: No Dynamic Memory Allocation in Core

* Name: No Dynamic Memory Allocation in Core
* Reference Number: 0001

**Table of Contents:**

1. [Description](#description)
2. [Rationale](#rationale)
3. [Source](#source)
4. [Notes](#notes)

## Description

The [Framework Core](../components/core/) components shall not utilize dynamic memory allocation. All core data structures will utilize static buffer sizes, which are configurable by the end user.

Users can decide in the higher levels of code to use dynamic memory, but the core functionality must support the strictest embedded use case of only utilizing static memory allocations.

## Rationale

Embedded systems developers and teams have strong opinions on dynamic vs static memory allocation. Many platforms will not use dynamic memory allocation schemes, and some industries require that all memory must be statically allocated. In order to keep the framework flexible and applicable to the widest variety of systems possible, we will support the strictest case with our required framework components.

Users are allowed to use dynamic memory allocation if desired, but we must support those who will only utilize static allocations.

## Source

Phillip set this as a design constraint at the beginning of the project.

## Notes

* This constraint applies to development
* This constraint applies to the [Framework Core](../components/core/) components only
* Users can decide to use dynamic memory in the [Platform](../components/core/virtual_platform.md) or Software Layer
* The Framework Core must provide static allocation methods at a minimum, though dynamic allocation alternatives can be provided if required
	* Users must configure this behavior for their platform
