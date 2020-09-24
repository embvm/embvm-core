# 6. Differentiate Drivers and HAL

Date: 2018-07-06

Relates to decision [0002: Layering Scheme](0002-layering-scheme.md)

## Status

Accepted

## Context

Processor peripherals often require additional interfaces and information that generic peripheral interfaces don't. For instance, we need to initiate our ARM chip's SPI driver with an SFR base address and some configuration options. If I am writing an accelerometer attached to the SPI bus, I only care about read/write and providing my CS address. I don't need any knowledge of the processor details for the SPI bus.

## Decision

We will provide two sets of interfaces:

* HAL (drivers) which manage low-level processor hardware devices
* Drivers with generalized interfaces which abstract away the low-level details
	* The software layer doesn't need to know about the clock settings or location of the SPI0 SFRs

The platform and software layers will interact with the generic drivers for application features. The platform layer will create the generic drivers with references to the processor HAL.

## Consequences

* Duplication of some interfaces
	* The HAL and generic drivers will share a common set of abstractions in many ways
	* Need to minimize duplication as much as possible
	* Need to ensure our HAL is a minimal set of processor peripherals and busses only
* Additional overhead
	* Adding a layer in between the hardware and the software may impact performance
	* Careful design consideration needs to be given in order to reduce this performance
