# CRC-R: Logger

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

* Log data requested by a user to a specified location
* Filter log calls by level

## Requirements

* Logging level configurable at compile time
* Logging level configurable at run-time (within compile-time limits)
* Provide a way for users to specify a custom logging strategy (e.g., file, buffer, UART + buffer)

## Collaborators

N/A

## Rationale

Logging is a common debugging requirement in embedded systems. Why should we make developers spin their own loggers?

In order to allow users to implement their own logging methods, the Logger will be implemented using the [Factory Method Pattern](../../patterns/factory_method.md) and the [Strategy Pattern](../../patterns/strategy.md).

## Source Links

TBD

## Related Documents

* [Factory Method Pattern](../../patterns/factory_method.md)
* [Strategy Pattern](../../patterns/strategy.md)

## Notes

Can we take advantage of Aspect Oriented Programming to insert logger code?


Make log_if_true() and log_if_false() macros for my framework. make the logic disabled.
