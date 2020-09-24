# CRC-R: Interrupt Lock

A locking object type which disables and enables interrupts.

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

* Disable and enable interrupts using a `BasicLockable` interface

## Requirements

* `lock()` disables interrupts
* `unlock()` enables interrupts
* Users can supply enable/disable functions for use across a variety of platforms

## Collaborators

* The [Interrupt Queue](interrupt_queue.md) uses an Interrupt Lock rather than a standard mutex

## Rationale

C++ supplies nice locking types, such as `std::unique_lock`. Creating a `BasicLockable` class which can enable and disable interrupts allows us to take advantage of some of these types.

## Source Links

* [Interrupt Lock Folder](../../../../src/utilities/interrupt_lock)
* Interrupt Lock
	* [interrupt_lock_test.cpp](../../../../src/utilities/interrupt_lock/interrupt_lock_test.cpp)
	* [interrupt_lock.hpp](../../../../src/utilities/interrupt_lock/interrupt_lock.hpp)

## Related Documents

N/A

## Notes

N/A
