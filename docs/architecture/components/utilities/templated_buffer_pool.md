# CRC-R: Templated Buffer Pool

**Table of Contents:**

1. [Responsibilities](#responsibilities)
2. [Requirements](#requirements)
3. [Collaborators](#collaborators)
4. [Rationale](#rationale)
5. [Source Links](#source-links)
6. [Related Documents](#related-documents)
7. [Notes](#notes)

## Responsibilities

* Manage a pool of buffers
* Recycle buffers when they are no longer needed by the consumer
* Allocate new buffers when needed (if not using static allocation)

## Requirements

* Support static and dynamic memory schemes

## Collaborators

* Buffer pools manage [Buffers](templated_buffer.md)

## Rationale

Memory pools are frequently used in embedded systems to provide a fixed memory capacity while still supporting dynamic-esque allocations (e.g., event pools). The buffer pool serves that need.

## Source Links

TBD

## Related Documents

N/A

## Notes

N/A
