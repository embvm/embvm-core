# CRC-R: Boot Flags

**Status:** Not started

**Table of Contents:**

1. [Responsibilities](#responsibilities)
2. [Requirements](#requirements)
3. [Collaborators](#collaborators)
4. [Rationale](#rationale)
5. [Source Links](#source-links)
6. [Related Documents](#related-documents)
7. [Notes](#notes)

## Responsibilities

* Set flags while running (e.g., prior to reboot)
* Check flags during boot
* Clear flags

## Requirements

TBD

## Collaborators

TBD

## Rationale

We often need to set various flags to indicate our intentions during a reboot/boot process. By setting flags, we can know:

* Whether we've booted successfully or not
* Whether a crash occurred
* Whether a firmware update failed
* Whether a panic occurred

We can check for these flags and implement specific behaviors after booting.

## Source Links

TBD

## Notes

We can handle specific flags & behaviors within the platsform file. Perhaps we call platform_init() with boot args supplied?
