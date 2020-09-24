# CRC-R: Static Queue

Static (fixed size) queue for use on systems without dynamic memory allocation.

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

* Operate like `std::queue`, as a FIFO queue
* Manage a fixed-size data buffer

## Requirements

* Memory is statically allocated during creation
* Cannot grow past the fixed-size limit
* No exceptions

## Collaborators

N/A

## Rationale

Embedded systems often do not utilize dynamic memory allocation. An equivalent structure using static memory should be provided.

## Source Links

This requirement is satisfied by the ETL datatypes:

* [`etl::queue`](../../../../src/stdlibs/etl/include/etl/queue.h)
* [`etl::list`](../../../../src/stdlibs/etl/include/etl/list.h)
* [`etl::vector`](../../../../src/stdlibs/etl/include/etl/vector.h)

## Related Documents

N/A

## Notes

N/A
