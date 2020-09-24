# CRC-R: Static Function

Static implementation of `std::function` (no dynamic allocation, no exceptions).

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

* Operate like `std::function`, as a container for a callable object

## Requirements

* No dynamic memory allocation
* No exceptions
* Cannot grow past fixed-size limit

## Collaborators

N/A

## Rationale

Embedded systems often do not utilize dynamic memory allocation. An equivalent structure using static memory should be provided.

## Source Links

The inplace_function library has been used to satisfy this requirement:

* [inplace_function.hpp](../../../../src/stdlibs/libcpp/extensions/inplace_function/inplace_function.hpp)

## Related Documents

N/A

## Notes

N/A
