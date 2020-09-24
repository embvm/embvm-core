# CRC-R: Templated Buffer

**Table of Contents:**

1. [Responsibilities](#responsibilities)
2. [Requirements](#requirements)
3. [Collaborators](#collaborators)
4. [Rationale](#rationale)
5. [Source Links](#source-links)
6. [Related Documents](#related-documents)
7. [Notes](#notes)

## Responsibilities

* Provide a generic buffer container
* Provide an offset-view into the buffer without copying data
	* E.g. frame with header -> parse header, advance pointer to frame
	* Size and offset automatically adjusted
* Resize the buffer without copying contents
* Release the buffer pointer to transfer ownership to another object/pointer
* Report if the buffer is valid or not

## Requirements

* Support dynamic and static memory variants

## Collaborators

* Buffers can be managed by a [Buffer Pool](templated_buffer_pool.md)

## Rationale

Standard containers provide useful behavior, but sometimes we need additional guarantees.

We can utilize the memory safety of RAII and without relying on raw pointer math. We provide methods for setting the offset into the buffer, which mimics pointer advancement.

We can transfer buffer ownership to another process if it's needed, again preventing copies in embedded systems. This also allows interoperability with code that requires memory ownership but does not work with the Buffer type.

## Source Links

TBD

## Related Documents

N/A

## Notes

N/A
