# 8. Virtual Platform as Mediator Pattern

Date: 2018-07-06

## Status

Accepted

## Context

We are trying to build a reusable embedded system framework. However, many embedded system implementation details are particular to the underlying hardware details. Systems become tightly coupled because the various software elements interact directly with RTOS, processor, and hardware functionality. If these areas were contained behind generic interfaces and kept from knowing about each other, we increase the odds of creating reusable software designs.

## Decision

The virtual platform abstraction will serve as the [mediator](../patterns/mediator.md) for the system. All platform-specific behavior will be implemented within the virtual platform.

The platform will contain all of the hardware implementation details, ensuring that the rest of the application is decoupled from these details. This decoupling will work through generic drivers (set up by the platform) and platform APIs (basic set defined by us, extensible by user).

The software layer will interact with hardware through the generic driver interfaces and the platform-provided APIs. This ensures that the software application is portable across components and across platform implementations (assuming API guarantees are met on the new platform).

## Consequences

* We keep our individual components loosely coupled by having the mediator coordinate activities between them
	* We don't need our different components to know about each other directly
* The virtual platform will be tightly coupled to the specific hardware implementation (it is already)
* A new hardware implementation requires a new platform, requiring re-definition of the interfaces and behavior for new hardware
* If a new platform meets the API guarantees, porting will be simplified and the end application will still work
* We must be careful about the APIs that we provide at the platform layer
	* It's still possible to create poor platform abstractions that make guarantees that can only be met by a specific set of hardware

## Further Reading

* [Mediator Pattern](../../patterns/mediator.md)
